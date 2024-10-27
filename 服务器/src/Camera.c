/**********************************************************************
                 Maxwell GuangZhou   2023.06-2023.07

      30HZ CAMERA       ��׽���棬����ʾ�ڿ������LCD��----��ͼ�����ת��
      TCPЭ��            ʵ����������     server��



        * ע�⣺��������ϵͳ����֮���û��������½�������ͷ������ò�������ͷ��
        * �豸�ļ�  /dev/video7



***********************************************************************/
#include "Project.h"


extern int*lcd_mp;

struct buffer {
        void *                  start;
        size_t                  length;
};
struct buffer * buffers = NULL;
static unsigned int  n_buffers = 0;
static int  camera_fd  = -1; //�����洢�򿪵�����ͷ���ļ�������
unsigned int  * rgb32buf;//Ϊ�˴洢ת�����һ֡����



char recv_buf[50] = {0};    //�������յ���ָ��

int fd_1_frame;             //��׽����һ֡����

int face_succes_flag = 0;




/******************************����ͷ�ɼ�����****************************/

void camera_start()
{
    sleep(1);                                                //��face.jpg��ʱһ���
    camera_fd = open ("/dev/video7", O_RDWR | O_NONBLOCK, 0);//O_NONBLOCK����ָ�����Է���������ʽ��
    init_device();                                           //��ʼ������ͷ
    start_capturing();                                       //��ʼ�����ǲ���ʾ��LCD
    read_JPEG_file("student_face2.jpg",0,0);                 //��ʾface2.jpg

    pthread_t cam_capture;                 //��������ͷ��ʾ�߳�
    // pthread_t id_cam_lock;
    int ret5 = pthread_create(&cam_capture,NULL,mainloop,NULL);     //�߳�--����ͷ
    if (ret5) {fprintf(stderr, "�̴߳���ʧ��%s\n", strerror(ret5)); exit(-1); }

    sleep(3);
    pthread_cancel(cam_capture);



    while(1)
    {
       face_succes_flag = camera_lock();
      if(face_succes_flag ==1)
      {

       read_JPEG_file("success.jpg",0,0);
       break;
      }
    if(face_succes_flag ==0)
      {
       read_JPEG_file("error.jpg",0,0);
       break;
      }

    }

    // if()//������ȷ
    // {pthread_cancel(cam_capture);} //mainloop�߳���ֹ��
    // else {}//������ʾ����

    //  pthread_exit(NULL);//��ֹ��������������߳�


    sleep(2);
    cap_start = 0;          //�ر�����ͷ��ʾ-->�����߳�
    teacher_ways_flag = 0;  // == 1 ������ʦ
    student_ways_flag = 0;  // == 1 ����ѧ��
    stu_atten_flag = 0;     //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
    tea_atten_flag = 0;     //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
    face_reconizing = 0;    //���ⴥ������ͻ ��������
    again_re_select_flag = 0;//ʧ������ʾ

    desk = 1;
}



 // linux 1 ϵͳ�²ɼ���������� 2.Ҫ֪���������ͷ����Ļ�����ʲô��ʽ�ģ�3.�����Ļ֧��ʲô��ʽ  ��ʽ���ʽ֮����ô
 //ת����ת��Ҫ��Ҫ�õ���İ���������˵���Լ�д����ת����



//����ɼ�����
void *mainloop (void* argv)
{
    struct v4l2_buffer buf;
    unsigned int i;

    //��ѭ���Ƕ�����ͷ���м���
    for (;;)
    {
        fd_set fds;
        struct timeval tv;
        int r;

        FD_ZERO (&fds);
        FD_SET (camera_fd, &fds);

        /* Timeout. */
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        select (camera_fd + 1, &fds, NULL, NULL, &tv);


        //ѭ���İѻ��������г���
        for(i = 0; i < n_buffers; ++i) //n_buffers�ǻ��������
        {
          CLEAR (buf);
          buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
          buf.memory = V4L2_MEMORY_MMAP;

          ioctl (camera_fd, VIDIOC_DQBUF, &buf); //�ѻ���������

          //��ͼ�����ת��
          yuyv_to_rgb(buffers[i].start);

          label2:

          // if((strncmp(recv_buf, "������ͷ",4) )== 0 ) {
            if(cap_start == 1){           //����ѭ������Ƿ��п�������
            camera_to_lcd(70,120);}
            if (cap_start != 1)
            goto label2;
            // else {  read_JPEG_file("student_face2.jpg",0,0);  }//��ʾface2.jpg
          // }
          // fd_1_frame = open("empty.jpg",O_RDWR|O_CREAT,S_IRWXU); //���ļ���û���򴴽���  ���ļ����ڴ洢...
          //                                                         //��׽��������ͼƬ
          // write(fd_1_frame,rgb32buf,sizeof(WIDTH*HEIGHT*3));
          // read_JPEG_file("empty.jpg",70,120);


          ioctl (camera_fd, VIDIOC_QBUF, &buf);

          //  if(face_succes_flag == 1)
          // {
          //   face_succes_flag = 0;
          //  pthread_exit(NULL);//��ֹ��������������߳�
          // }
        }



    }

}




//��ʼ������ͷģ��
void init_device(void)
{

  struct v4l2_format fmt;      //Ϊ����������ͷ��׽����Ĳ���

  CLEAR (fmt); //��սṹ��

  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE; //�ɼ���Ƶ
  fmt.fmt.pix.width       = WIDTH;       //��
  fmt.fmt.pix.height      = HEIGHT;      //��
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;           //ͼ���ʽ


  ioctl (camera_fd, VIDIOC_S_FMT, &fmt);

  //���뻺���������û���������
  init_mmap();

  rgb32buf  =  (unsigned int  *) malloc( WIDTH*HEIGHT*4 ); //����ѿռ�洢RGB��ʽ

}


void init_mmap     (void)
{
  struct v4l2_requestbuffers req;  //�����ڴ滺����

  CLEAR (req);

  req.count               = 4;                            //����4��
  req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;  //��Ƶ�ɼ�
  req.memory              = V4L2_MEMORY_MMAP;             //�ڴ�ӳ��

  ioctl (camera_fd, VIDIOC_REQBUFS, &req);

  //Ϊÿ��ͼ�񻺳��������ڴ�
  buffers = calloc (req.count, sizeof (*buffers));

  //ѭ����ÿ������Ļ����������в�������
  for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
  {
      struct v4l2_buffer buf; //���û���������

      CLEAR (buf);

      buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;  //��������ʽ
      buf.memory      = V4L2_MEMORY_MMAP;             //�ڴ�ӳ��
      buf.index       = n_buffers;                    //���������

      ioctl (camera_fd, VIDIOC_QUERYBUF, &buf);

      buffers[n_buffers].length = buf.length;         //�������Ĵ�С
      buffers[n_buffers].start  = mmap (
                                          NULL,
                                          buf.length,
                                          PROT_READ | PROT_WRITE,
                                          MAP_SHARED ,
                                          camera_fd,
                                          buf.m.offset
                                        );

  }
}

//������Ƶ�ɼ�
void start_capturing(void)
{
  unsigned int i;
  enum v4l2_buf_type type;


  for (i = 0; i < n_buffers; ++i)
  {

    struct v4l2_buffer buf;

    CLEAR (buf);

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = i;

    ioctl (camera_fd, VIDIOC_QBUF, &buf); //�����еĻ��������
  }

  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  ioctl (camera_fd, VIDIOC_STREAMON, &type); //���Ϳ����ɼ�ָ��

}




//ת��һ�����ص�
int yuyv_to_rgb_pix(int Y,int Cb,int Cr)
{
  unsigned int pix_point = 0;//�洢һ�����ص������
  unsigned int R,G,B;      //�����洢RGB��ɫ����

  //ͨ��V4L2�ٷ������ĵ��еĹ�ʽ���м���
  R = (255/219)*(Y - 16) + 1.402 * (127/112)*(Cr - 128);
  G = (255/219)*(Y - 16) - 0.344 * (127/112)*(Cb - 128) - 0.714*(127/112)*(Cr - 128);
  B = (255/219)*(Y - 16) + 1.772 * (127/112)*(Cb - 128);

  //��ֹԽ��  RGB�ֱ�Ϊռ��һ���ֽ� 0~255��������Ҫ����Խ�紦��
  if(R > 255) R = 255;
  if(G > 255) G = 255;
  if(B > 255) B = 255;
  if(R < 0) R = 0;
  if(G < 0) G = 0;
  if(B < 0) B = 0;

  pix_point = (R<<16)|(G<<8)|B;//��RGB�ϳ�һ�����ص�
  return pix_point;
}



//ת��һ֡ARGB���ݣ�YUYVתRGB32��
int yuyv_to_rgb(char * yuyvbuf)
{
  int i,j;
  for(i=0,j=0;i<WIDTH*HEIGHT;i+=2,j+=4)
  {
    rgb32buf[i]=yuyv_to_rgb_pix(yuyvbuf[j],yuyvbuf[j+1],yuyvbuf[j+3]);//argb--y1u1y2v1
    rgb32buf[i+1] = yuyv_to_rgb_pix(yuyvbuf[j+2],yuyvbuf[j+1],yuyvbuf[j+3]);
  }
}


//��ת���������д��LCD
int  camera_to_lcd(int start_x,int start_y)
{
  int x,y;

  for(y=0;y<HEIGHT;y++)
  {
    for(x=0;x<WIDTH;x++)
    {
      *(lcd_mp+(start_y*800)+start_x + (y*800) + x) = rgb32buf[y*WIDTH+x];
    }
  }
}


/**************************����ͷ�ɼ�����END***********************/










