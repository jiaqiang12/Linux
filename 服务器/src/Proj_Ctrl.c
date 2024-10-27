/**********************************************************************
                 Maxwell GuangZhou   2023.06-2023.07

      void *get_xy(void * arg)                          ��ȡ����
      void *judge_location(void * arg)                  ������ܷ���
      void *read_location_ways_attendence(void * arg)   ��ȡ�򿨷�ʽ����
      void lcd_init()                                   LCD��ʾ����ʼ��
      int  read_JPEG_file ()                            ��ʾLCDͼƬ

      int  ControlMianUI()                              ����ģ��,��ȡ���겢����
      void open_machine_carton()                        ��������
      void *state_judge(void * arg)                                ����flag�ж�״̬
      void state_ctrl()                                 ����״̬��Ӧ�ù���

***********************************************************************/
#include "Project.h"

int *lcd_mp =  NULL; //�洢�ڴ�ӳ����׵�ַ
int  lcd_fd ; //���ڴ洢LCD���ļ�����
int fd_touch;
int x,y;
struct input_event touch;


/*************************״̬����־λ****************************************/
int desk = 1;                //�Ƿ������� 0�� 1��
int teacher_ways_flag = 0;   // == 1 ������ʦ
int student_ways_flag = 0;   // == 1 ����ѧ��
int stu_atten_flag = 0;      //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
int tea_atten_flag = 0;      //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
int again_re_select_flag = 0;//ʧ������ʾ

/*************************״̬��****************************************/
int state_machine = 0;

/*************************һЩ��־λ****************************************/
int face_succs_flag = 0;     //����ʶ���־λ  0����ʧ�� ==1���� �ɹ�




int face_reconizing = 0;      //��ֹ��������ͻ
int pass_mutex = 0;          //ͬ��

pthread_t thread_id_desk;                 //���涯̬ͼ




int ControlMianUI()
{
    open_machine_carton();                      //��������
    // wave2();
    desk = 1;                                   //������

    pthread_t thread_id_xy_fun;                 // �ж������Ӧ�Ĺ���
    pthread_t thread_id_xy;                     //���ڻ�ȡ����
    pthread_t thread_id_state_judge;            //����flag�ж�״̬
    pthread_t thread_id_state;
    pthread_t thread_id_server;
    // pthread_t thread_id_wave;
    /*****************************�̴߳���**********************************/
    int ret1, ret2, ret3, ret4, ret5,ret6,ret7;
    ret2 = pthread_create(&thread_id_xy,NULL,get_xy,NULL);                 //�߳�--��ȡ����
    if (ret2) {fprintf(stderr, "�̴߳���ʧ��%s\n", strerror(ret2)); exit(-1); }
    ret1 = pthread_create(&thread_id_xy_fun,NULL,judge_location,NULL);     //�߳�--��������
    if (ret1) {fprintf(stderr, "�̴߳���ʧ��%s\n", strerror(ret1)); exit(-1); }
    ret4 = pthread_create(&thread_id_state_judge,NULL,state_judge,NULL);   //�߳�--����flag�ж�״̬
    if (ret4) {fprintf(stderr, "�̴߳���ʧ��%s\n", strerror(ret4)); exit(-1); }
    ret3 = pthread_create(&thread_id_state,NULL,state_ctrl,NULL);          //�߳�--״̬������
    if (ret3) {fprintf(stderr, "�̴߳���ʧ��%s\n", strerror(ret3)); exit(-1); }
    ret5 = pthread_create(&thread_id_server,NULL,server_start,NULL);          //�߳�--������
    printf("�������̴߳����ɹ�\n");
    if (ret5) {fprintf(stderr, "�̴߳���ʧ��%s\n", strerror(ret5)); exit(-1); }
    ret6 = pthread_create(&thread_id_desk,NULL,desk_gif,NULL);                 //�߳�--����
    if (ret6) {fprintf(stderr, "�̴߳���ʧ��%s\n", strerror(ret6)); exit(-1); }
    // ret7 = pthread_create(&thread_id_wave,NULL,wave,NULL);               //�߳�--������
    // if (ret7) {fprintf(stderr, "�̴߳���ʧ��%s\n", strerror(ret7)); exit(-1); }
pthread_detach(thread_id_xy);

    return 0;
}



void *desk_gif(void * arg)
{
    int i = 0;
    char gif_name[60]={0};
    for(;;)
    {
       label1:
    //    usleep(5000);
        // ��ʾ�������� ѭ����ʾ
        if(desk == 1)
        {

            for(i = 0;i<20;i++)
            {     //��һ���̶���ʽ���ַ������������
                sprintf(gif_name,"gdesk%d.jpg",i);
                read_JPEG_file(gif_name,0,0);
                // usleep(50000);
                  nanosleep(&ts, NULL);
                // if(38 == i)
                // i = 0;
                // break;
                 if(desk == 0)
                  goto label1;
            }



        }



    }

}

// ״̬��-Ƕ��ʽ-��Ƭ��  ��̼���
// 800*480  xianshi  ��ʾ�ʹ����Ƿֿ���
//1.����������ʾ�Ƿֿ���  2.ȫ�ֱ�����Ҳ���������õĸ���״̬���ı������̵߳�ͬ���뻥�⣩���ź���..�����ٽ���Դ�ķ���
 //������̷߳���ͬ��ȫ�ֱ���--������mutex������ͬ���뻥��-->����
 // int a;
 // a += 1; //�߳�c
 // a = 2; //xiancehn�߳�
 // �߳�e����ȡa��ֵ
 // �ڻ��ĽǶ��ϣ�r1 ,

void *judge_location(void * arg)    // �ж������Ӧ�Ĺ���
{

    while(1)
        {
            while(1)    /*******************�����棨״̬һ���µ��**************************/
            {
                    if((x>490 && x< 730)&&(y>100 && y < 250)&&(desk == 1)&&(pass_mutex ==0))
                                /********ѡ������ʦ*********/
                {
                    printf("��ӭ��ʦ���ϿΣ���ѡ��򿨷�ʽ  ������(x=%d,y=%d)\n",x,y);

                    desk = 0;               //����������
                    teacher_ways_flag = 1;  // == 1 ������ʦ
                    student_ways_flag = 0;  // == 1 ����ѧ��
                    stu_atten_flag = 0;     //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
                    tea_atten_flag = 0;     //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
                    again_re_select_flag = 0;//ʧ������ʾ
                    x = 0; y =0;
                    break;
                }
                    if((x>60 && x< 300)&&(y>100 && y < 250)&&(desk == 1))  /********ѡ����ѧ��*********/
                {
                    printf("��ӭѧ����ѧϰ����ѡ��򿨷�ʽ  ������(x=%d,y=%d)\n",x,y);
                    desk = 0;               //����������
                    teacher_ways_flag = 0;  // == 1 ������ʦ
                    student_ways_flag = 1;  // == 1 ����ѧ��
                    stu_atten_flag = 0;     //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
                    tea_atten_flag = 0;     //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
                    again_re_select_flag = 0;//ʧ������ʾ
                    x = 0; y =0;
                    break;
                }
                break;
            }



            while(1)              /*****************��ѡ���ڷ�ʽ�µ��******************/
            {
                    if((x>1 && x< 200)&&(y>70 && y < 250)&&(desk == 0)&&
                    (student_ways_flag == 1 || teacher_ways_flag == 1)&&
                    (face_reconizing ==0)&&(pass_mutex == 0))
                {

                    if(student_ways_flag == 1)  {                        /***********ѡ����ˢ��*********/
                    printf("��ͬѧˢ��  ������(x=%d,y=%d)\n",x,y);
                    desk = 0;               //����������
                    teacher_ways_flag = 0;  // == 1 ������ʦ
                    student_ways_flag = 0;  // == 1 ����ѧ��
                    stu_atten_flag = 2;     //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
                    tea_atten_flag = 0;     //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
                    again_re_select_flag = 0;//ʧ������ʾ
                    // x = 0; y =0;
                    // break;
                    }

                    else if(teacher_ways_flag == 1){
                    printf("����ʦˢ��  ������(x=%d,y=%d)\n",x,y);
                    desk = 0;               //����������
                    teacher_ways_flag = 0;  // == 1 ������ʦ
                    student_ways_flag = 0;  // == 1 ����ѧ��
                    stu_atten_flag = 0;     //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
                    tea_atten_flag = 2;     //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
                    again_re_select_flag = 0;//ʧ������ʾ
                    x = 0; y =0;
                    break;
                    }

                }

                    if((x>600 && x< 800)&&(y>70 && y < 250)&&
                    (student_ways_flag == 1 || teacher_ways_flag == 1)&&
                    (face_reconizing ==0)&&(pass_mutex == 0))
                {
                    if(student_ways_flag == 1)  {
                    printf("��ͬѧ����������  ������(x=%d,y=%d)\n",x,y);   /**********ѡ��������*********/
                    desk = 0;               //����������
                    teacher_ways_flag = 0;  // == 1 ������ʦ
                    student_ways_flag = 1;  // == 1 ����ѧ��
                    stu_atten_flag = 1;     //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
                    tea_atten_flag = 0;     //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
                    again_re_select_flag = 0;//ʧ������ʾ
                    x = 0; y =0;
                    break;
                    }
                    else if(teacher_ways_flag == 1)  {
                    printf("����ʦ����������  ������(x=%d,y=%d)\n",x,y);
                    desk = 0;               //����������
                    teacher_ways_flag = 1;  // == 1 ������ʦ
                    student_ways_flag = 0;  // == 1 ����ѧ��
                    stu_atten_flag = 0;     //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
                    tea_atten_flag = 1;     //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
                    again_re_select_flag = 0;//ʧ������ʾ
                    x = 0; y =0;
                    break;
                    }

                }

                    if((x>10 && x< 350)&&(y>300 && y < 480)&&
                    (student_ways_flag == 1 || teacher_ways_flag == 1)&&
                    (face_reconizing ==0)&&(pass_mutex == 0))
                    /***********ѡ��������*********/
                {
                    if(student_ways_flag == 1)  {
                    printf("��ͬѧ�����뿼��  ������(x=%d,y=%d)\n",x,y);
                    desk = 0;               //����������
                    teacher_ways_flag = 0;  // == 1 ������ʦ
                    student_ways_flag = 1;  // == 1 ����ѧ��
                    stu_atten_flag = 4;     //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
                    tea_atten_flag = 0;     //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
                    again_re_select_flag = 0;//ʧ������ʾ
                    pass_mutex = 1;         //�������������Ĵ���������
                    x = 0; y =0;
                    break;
                    }
                    else if(teacher_ways_flag == 1){
                    printf("����ʦ�����뿼��  ������(x=%d,y=%d)\n",x,y);
                    desk = 0;               //����������
                    teacher_ways_flag = 1;  // == 1 ������ʦ
                    student_ways_flag = 0;  // == 1 ����ѧ��
                    stu_atten_flag = 0;     //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
                    tea_atten_flag = 4;     //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
                    pass_mutex = 1;         //�������������Ĵ���������
                    again_re_select_flag = 0;//ʧ������ʾ
                    x = 0; y =0;
                    break;
                    }

                }

                    if((x>430 && x< 800)&&(y>300 && y < 480)&&
                    (student_ways_flag == 1 || teacher_ways_flag == 1)&&
                    (face_reconizing ==0)&&(pass_mutex == 0))
                                                                /***********ѡ��������*********/
                {
                    if(student_ways_flag == 1)  {
                    printf("��ͬѧ����������  ������(x=%d,y=%d)\n",x,y);
                    desk = 0;               //����������
                    teacher_ways_flag = 0;  // == 1 ������ʦ
                    student_ways_flag = 1;  // == 1 ����ѧ��
                    stu_atten_flag = 3;     //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
                    tea_atten_flag = 0;     //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
                    face_reconizing = 1;    //����ʶ������
                    again_re_select_flag = 0;//ʧ������ʾ
                    x = 0; y =0;
                    break;
                    }
                    else if(teacher_ways_flag == 1){
                    printf("����ʦ����������  ������(x=%d,y=%d)\n",x,y);
                    desk = 0;               //����������
                    teacher_ways_flag = 1;  // == 1 ������ʦ
                    student_ways_flag = 0;  // == 1 ����ѧ��
                    stu_atten_flag = 0;     //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
                    tea_atten_flag = 3;     //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
                    face_reconizing = 1;    //����ʶ������
                    again_re_select_flag = 0;//ʧ������ʾ
                    x = 0; y =0;
                    break;
                   }

                }
                break;
            }

            /*��ʵ֤����main����λ�û��Ҫ����Ϊsleep����������*/
            /*���ǻ����˼��Ͳ�֪������ô����*/

                // while(1)  /*************************�ڵȴ����ڽ����µ����Ч*****************************/
                // {
                //     if((x >=0 && x<=800)&&(y>=0 && y<=480)&&(desk == 0)&&  /*�ڵȴ��򿨵�ʱ��, ������û����*/
                //     ((teacher_ways_flag == 1)|| (student_ways_flag == 1))&&
                //     ( stu_atten_flag != 0 || tea_atten_flag != 0)&&(again_re_select_flag == 0))
                //     {
                //         printf("���У�����Ļ��Ч  ������(x=%d,y=%d)\n",x,y);
                //         x = 0; y =0;
                //         break;
                //     }
                //     break;
                // }




            while(1) /************************����ʶ������µ��*********************************/
            {
                if((tea_atten_flag == 3||stu_atten_flag == 3)&&(face_reconizing ==1)&&(pass_mutex ==0))
                {
                    if((x >160 && x<290)&&(y>370 && y<470))      /***����˿�������ť������ͷ����***/
                    {
                        printf("����ͷ�������뿴������ͷ  ������(x=%d,y=%d)\n",x,y);
                        face_succs_flag = 0;      //����0����ʧ�� ==1�ǳɹ�--->��������ˢ��һ�α�־λ

                        cap_start = 1;           //ʹ<���������־λ>��ת  0�ǲ�������1�ǿ���

                        x = 0; y =0;
                        break;
                    }

                }


                break;      //������break���ε���˼
            }







            // while(1) /*******************��again_re_seclet�����µ�� try_again/re-select**********************/
            // {
            //     if((x >230 && x<400)&&(y>340 && y<430)&&(again_re_select_flag == 1)) /***try again***/
            //     {
            //         printf("������  ������(x=%d,y=%d)\n",x,y);
            //         again_re_select_flag == 0;
            //         x = 0; y =0;
            //         break;

            //     }


            //     if((x >400 && x<600)&&(y>340 && y<430)&&(again_re_select_flag == 1)) /***re-select***/
            //     {
            //         printf("������ѡ��򿨷�ʽ  ������(x=%d,y=%d)\n",x,y);

            //         if(stu_atten_flag != 0)  {
            //         printf("�ص�ѧ���򿨷�ʽѡ�����  ������(x=%d,y=%d)\n",x,y);
            //         student_ways_flag = 1;
            //         x = 0; y =0;
            //         break;
            //         }

            //         if(tea_atten_flag != 0)  {
            //         printf("�ص���ʦ�򿨷�ʽѡ�����  ������(x=%d,y=%d)\n",x,y);
            //         teacher_ways_flag = 1;
            //         x = 0; y =0;
            //         break;
            //         }


            //     }
            //     break;
            // }


        }

}


void *state_judge(void * arg)
{
    while (1)
    {
        if(desk == 1){
                    teacher_ways_flag = 0;  // == 1 ������ʦ
                    student_ways_flag = 0;  // == 1 ����ѧ��
                    stu_atten_flag = 0;     //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
                    tea_atten_flag = 0;     //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
                    again_re_select_flag = 0;//ʧ������ʾ
            state_machine = 1;                                       /*״̬һ����������desk*/
        }

        if((teacher_ways_flag == 0) && (student_ways_flag == 1)) {   /*״̬����ѧ��ѡ���ڷ�ʽ*/

            state_machine = 2;
        }
        if((teacher_ways_flag == 1) && (student_ways_flag == 0)){
            state_machine = 3;                                       /*״̬������ʦѡ���ڷ�ʽ*/
        }



        if((stu_atten_flag == 1))    {                               /*״̬4���ȴ�ѧ����������*/

            state_machine = 4;
        }
        if((stu_atten_flag == 2))    {                               /*״̬5���ȴ�ѧ��RFID����*/

            state_machine = 5;
        }
        if((stu_atten_flag == 3))    {                               /*״̬6���ȴ�ѧ��ˢ������*/

            state_machine = 6;
        }
        if((stu_atten_flag == 4))    {                               /*״̬7���ȴ�ѧ�����뿼��*/

            state_machine = 7;
        }




        if((tea_atten_flag == 1))    {                               /*״̬8���ȴ���ʦ��������*/

            state_machine = 8;
        }
        if((tea_atten_flag == 2))    {                               /*״̬9���ȴ���ʦRFID����*/

            state_machine = 9;
        }
        if((tea_atten_flag == 3))    {                               /*״̬10���ȴ���ʦˢ������*/

            state_machine = 10;
        }
        if((tea_atten_flag == 4))    {                               /*״̬11���ȴ���ʦ���뿼��*/

            state_machine = 11;
        }


                                                                     /*״̬ʮ����again_re_selectҳ��*/
        if((again_re_select_flag == 1))    {                         /*״̬11���ȴ���ʦ���뿼��*/

            state_machine = 12;
        }


    }
}




void* state_ctrl(void* arg)
{
while(1)
    {

        switch (state_machine)                      /*******����״̬������Ӧ�Ĺ���**********/
        {

        case 1:                                             /*״̬һ����������desk*/
                       //�߳�--����

                //   read_JPEG_file("desk.jpg",0,0);
            break;
        case 2:                                             /*״̬����ѧ��ѡ���ڷ�ʽ*/
                read_JPEG_file("student_ways.jpg",0,0);
            break;
        case 3:                                             /*״̬������ʦѡ���ڷ�ʽ*/
                read_JPEG_file("teacher_ways.jpg",0,0);
            break;


        case 4:                                             /*״̬4���ȴ�ѧ����������*/
                read_JPEG_file("student_blue.jpg",0,0);
                blue_start();
            break;
        case 5:                                             /*״̬5���ȴ�ѧ��RFID����*/
                read_JPEG_file("student_swipe.jpg",0,0);
                rfid_start();
            break;
        case 6:                                             /*״̬6���ȴ�ѧ��ˢ������*/
                read_JPEG_file("student_face.jpg",0,0);
                camera_start();
            break;
        case 7:                                             /*״̬7���ȴ�ѧ�����뿼��*/
                read_JPEG_file("student_password.jpg",0,0);
                password_start();
            break;



        case 8:                                             /*״̬8���ȴ���ʦ��������*/
                read_JPEG_file("teacher_blue.jpg",0,0);
                blue_start();
            break;
        case 9:                                             /*״̬9���ȴ���ʦRFID����*/
                read_JPEG_file("teacher_swipe.jpg",0,0);
                rfid_start();
            break;
        case 10:                                             /*״̬10���ȴ���ʦˢ������*/
                read_JPEG_file("teacher_face.jpg",0,0);
                camera_start();
            break;
        case 11:                                             /*״̬11���ȴ���ʦ���뿼��*/
                read_JPEG_file("teacher_password.jpg",0,0);
                password_start();
            break;



        case 12:                                             /*״̬ʮ����again_re_selectҳ��*/
                read_JPEG_file("again_re_select.jpg",0,0);
            break;
        default: break;
        }

    //    printf("********�����ж�Ӧִ���ĸ�״̬��********\n");


    }

}







//  ��ȡ����
void *get_xy(void * arg)
{
      while(1)
    {
        while(1)
        {
            read(fd_touch, &touch ,sizeof(touch));          //���϶�ȡ�ļ�
            if(touch.type== EV_ABS && touch.code == ABS_X)   x = touch.value*800/1024;

            if(touch.type== EV_ABS && touch.code== ABS_Y)    y = touch.value*480/600;

            //������ȥ����һ��,����ѭ��;ֻץȡ����ȥ������ֵ
                if(touch.type== EV_KEY && touch.code == BTN_TOUCH &&  touch.value==0) //ѹ���ж�
             {

                    printf("�����κν����µ�������ӡ�ģ����� (x=%d,y=%d)\n",x,y);
                    break;

             }

        }
             x = 0; y =0;

    }
}






int lcd_init()
{

    //��LCD��������
    lcd_fd = open("/dev/fb0", O_RDWR);
        if (lcd_fd == -1)
        {
            printf("lcd�ļ���ʧ��\n");
             return -1;
        }
    fd_touch = open("/dev/input/event0",O_RDWR);
        if(-1 == fd_touch)
        {
            perror("open touch error");
            return -1;
        }

    //�ڴ�ӳ��
    lcd_mp = mmap(
    				NULL,
    				800 * 480 * 4 ,
    				PROT_READ | PROT_WRITE,
                    MAP_SHARED ,
                    lcd_fd ,
                    0
                 );
        if (lcd_mp == MAP_FAILED) //�ɹ�����0�� ʧ�ܷ��� -1����MAP_FAILED��
        {
            perror("mmap failed");
            return -1;
        }

}


 //c�������ַ�������""���ε�  jpgͼƬ��·�����ַ�����ʽ "xxx.jpg"
int read_JPEG_file (char * filename,int start_x,int start_y)
{

  //����һ������������ڴ洢JPEGͼ��Ľ������
  struct jpeg_decompress_struct cinfo;

  //�����˾ֲ�����
  FILE * infile;			//��һ��ָ�룬ָ�򱻴򿪵�Jpg�ļ�
  unsigned char * buffer;	//��һ��ָ�룬ָ�������һ�д�С�Ļ�����
  int row_stride;			//�ñ������ڴ洢���������һ��ͼ����ֽڴ�С  һ�д�С = ͼ��� * ����ɫ�� / 8  ��λ���ֽ�


  //����fopen����Ҫ���н����jpgͼƬ
  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return 0;
  }

  /* Step 1: ���䲢��ʼ��������� */

  //�Ѵ����������ڽ�����󣬵���������г��ִ��󣬿��Խ��д���
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);

  //��ʼ���������
  jpeg_create_decompress(&cinfo);

  /* Step 2: ָ������Դ��Ŀǰ����Դ����jpgͼƬ */
  jpeg_stdio_src(&cinfo, infile);

  /* Step 3: ��ȡjpg�ļ���������ʵ�Ƕ�ȡjpgͼƬ����Ϣ*/
  jpeg_read_header(&cinfo, TRUE);

  /* Step 4: ���ý����������������ǿ�ѡ�� */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */

  /* Step 5: ��ʼ���н������ */
  jpeg_start_decompress(&cinfo);


  //�ڽ��ж�ȡjpgͼ������ص�֮ǰ����Ҫ��ǰ����洢���ݵĻ���������Ҫ�����ڴ�
  row_stride = cinfo.output_width * cinfo.output_components; //������ڴ��С==ͼ��һ�����ص��ֽڴ�С

  //����malloc�����������ڴ�
  buffer = malloc(row_stride);

  /* Step 6: ѭ���Ĵ�jpgͼƬ�ж�ȡ�������ݣ�ÿ�ζ�һ��*/

  int i;
  int data; //���ڴ洢һ��ת���õ����ص�����
  while (cinfo.output_scanline < cinfo.output_height)
  {
   		//���ýӿ�����ȡһ�н�����ɵ�ͼ������  rgb rgb rgb rgb....  jpg(3�ֽ� rgb) --> lcd(4�ֽ� argb)
    	jpeg_read_scanlines(&cinfo, &buffer, 1);

    	//��Ҫ�Ѷ�ȡ��һ��jpgͼ�����ɫ��������д�뵽LCD�����ص���  0 1 2  3 4 5  6 7 8
    	for (i = 0; i < cinfo.output_width ; ++i)
    	{
    		data |= buffer[i*3]   << 16; //r
    		data |= buffer[i*3+1] << 8;  //g
    		data |= buffer[i*3+2] ;      //b

    		*(lcd_mp + start_y*800 + start_x + (cinfo.output_scanline-1)*800 + i) = data;  //��ʾ���ص�

    		data = 0;
    	}
  }

  /* Step 7: ��ɽ������ */

  jpeg_finish_decompress(&cinfo);

  /* Step 8: �ͷŽ������ */

  jpeg_destroy_decompress(&cinfo);

  fclose(infile); //�ر�jpg�ļ�

  return 1;
}


void open_machine_carton()
{
      char gif_name[60]={0};
     // ��ʾ�������� ѭ����ʾ
        for(int i = 8;i>1;--i)
       {     //��һ���̶���ʽ���ַ������������
         sprintf(gif_name,"/boot/boot%d.jpg",i);
         read_JPEG_file(gif_name,0,0);
          usleep(100000);
       }


}
