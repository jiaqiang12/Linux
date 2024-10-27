/**********************************************************************
                 Maxwell GuangZhou   2023.06-2023.07 

      *����ļ�ֻ��includeͷ�ļ����궨������
  
      *��Ҫȫ�ֱ��������

***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <strings.h>
#include "jpeglib.h" 
#include <string.h>
#include <pthread.h>  //���̱߳����Ҫ������
#include <termios.h>//ͷ�ļ� ���ô��ڲ����������
#include <sys/mman.h>
#include <linux/input.h>        /*������ϵͳͷ�ļ�*/
#include <arpa/inet.h>          /* TCP   */         
#include <sys/socket.h>         /* TCP   */      
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include <getopt.h>             /* getopt_long() */
#include <fcntl.h>              /* low-level i/o */
#include <errno.h>
#include <linux/videodev2.h>
#include <asm/types.h>          /* for videodev2.h */
#include <sys/ioctl.h>
 

/*******************************�̺߳���**********************************/
void *judge_location(void * arg);                        //�ж����깦�� 
void *get_xy(void * arg);                                // �õ���������
void *server_thread(void *argv);                         //�������߳�
void *mainloop (void* argv);                             //����ɼ������
void *state_judge(void* arg) ;                           //����flag�ж�״̬
void *state_ctrl(void* arg);                             //Proj_ctrl.c�����
void *server_start(void* arg);
void *desk_gif(void * arg);
/*******************************LCD����������JPEG*************************/
int lcd_init();
int read_JPEG_file (char * filename,int start_x,int start_y);
/*******************************RFID*************************************/
void UART1_Init();//RFID���߳�
int UART1_Init2();
void RFID_GetBCC(char *buf);
int PiccAnticoll(int uart1_fd);
unsigned char CalBCC(unsigned char *buf,int n);
void rfid_start();
/*******************************Blueteeth********************************/
void blue_start();
void UART2_Init();//����2�ĳ�ʼ��

/*******************************Password*********************************/
void password_start();
int passwork_lock();
void keyshow_pass();

/*******************************CAMERA*********************************/
void init_device(void); //��ʼ������ͷģ��
void init_mmap  (void); //������ͷ������������
void start_capturing(void);//������Ƶ�ɼ�
int  yuyv_to_rgb_pix(int Y,int Cb,int Cr);//ת��һ�����ص�
int  yuyv_to_rgb(char * yuyvbuf);//ת��һ֡ARGB���ݣ�YUYVתRGB32��
int  camera_to_lcd(int start_x,int start_y);//��ת���������д��LCD
void camera_start();//��main����  ����ֱ������ͷ��ʼ������
int  main_camera( );

#define CLEAR(x) memset (&(x), 0, sizeof (x))
#define  WIDTH  320  //�ɼ���ͼ��Ŀ��
#define  HEIGHT 240  //�ɼ���ͼ��ĸ߶�


int keyshow(); //����ʱ��ͼƬ�л�
int  camera_lock(void );

/*****************************����ʶ��+��������*******************************/






/*****************************������*********************************/
 
void *wave(void * sdd);
void wave2();
/*******************************����*********************************/
void open_machine_carton();
int  ControlMianUI(); //����ģ��


/*************************����ȫ�ֱ���״̬����־λ****************************************/
extern   int desk;                //�Ƿ������� 0�� 1��
extern   int teacher_ways_flag;   // == 1 ������ʦ
extern   int student_ways_flag;   // == 1 ����ѧ��
extern   int stu_atten_flag;      //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
extern   int tea_atten_flag;      //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����    
extern   int again_re_select_flag;//ʧ������ʾ
extern   int state_machine;




extern   int cap_start;
extern   int face_succes_flag;


extern   int x,y;

extern   int face_reconizing; 
extern   int pass_mutex;


extern  int pass_key_num  ;

extern  int pass_success_flag;
// int del_flag = 0;

// int pass_input_key[4] = {0};



extern  int ditance;
extern struct timespec ts;

// extern struct timespec ts2;