/**********************************************************************
                 Maxwell GuangZhou   2023.06-2023.07

       RFID



***********************************************************************/
#include "Project.h"

char recv_buf_rfid[128];//Ϊ�˽���RFIDģ�����Ӧ��Ϣ
char rt_buf[10];//��Ž�������
int uart1_fd;
char request_buff[] = {0x07,0x02,0x41,0x01,0x52,0x00,0x03};   //����ָ�������֡ �±���5����BCCУ��ͣ�
int RFID_read_sucess_flag = 0;  //ID���Ƿ��ȡ�ɹ��ı�־λ  û������0 ������1


int err_rfid = 0;


//��������ָ��ͷ���ײָ��Ϳ��ԴӴ����ж�ȡ����

void rfid_start()
{

	UART1_Init();
	while(1)
  {
	RFID_read_sucess_flag = UART1_Init2();


		if(RFID_read_sucess_flag == 1)
		{

			sleep(2);
			read_JPEG_file("success.jpg",0,0);
			sleep(2);
			desk = 1;
			teacher_ways_flag = 0;  // == 1 ������ʦ
			student_ways_flag = 0;  // == 1 ����ѧ��
			stu_atten_flag = 0;     //ѧ���򿨷�ʽ   1���� 2ˢ��  3����  4����
			tea_atten_flag = 0;     //��ʦ�򿨷�ʽ   1���� 2ˢ��  3����  4����
			again_re_select_flag = 0;//ʧ������ʾ
			// state_machine = 0;
			RFID_read_sucess_flag = 0;
			break;
		}

	//    if(RFID_read_sucess_flag == 2)
	// 	{
	// 		 printf("fail to read the card!\n");

	// 		 read_JPEG_file("error.jpg",0,0);
	// 		 sleep(2);
	// 		 err_rfid++;
	// 		 if (err_rfid == 3)
	// 		 {
	// 		  printf("����ˢ�����󣬻ص�����\n");
	// 		  err_rfid = 0;

	// 		  break;
	// 		 }
	// 	}


  }



}




 void UART1_Init()			//�Ѿ���ctrl�г�ʼ������
 {
	//1.��UART
    uart1_fd = open("/dev/ttySAC1", O_RDWR|O_NOCTTY);
    if (uart1_fd == -1)
    {
        printf("uart�ļ���ʧ��\n");
        // return -1;
    }




	//1.�����������ڽṹ��
	struct termios  old_attr; //���洮�ڵľ�����
	struct termios  new_attr; //���洮�ڵ�������

	//2.���洮�ڵ�ԭ��������
	tcgetattr (uart1_fd,&old_attr);

	//3.��������λ  8bit
	new_attr.c_cflag &= ~CSIZE; //�������λ
	new_attr.c_cflag |=  CS8;   //8λ����λ

	//4.����ֹͣλ  1bit
	new_attr.c_cflag &= ~CSTOPB; //1λ����λ

	//5.����У��λ  none
	new_attr.c_cflag &= ~PARENB; //��ʹ��У��λ

	//6.���ò�����  9600bps
	cfsetospeed (&new_attr,B9600);
	cfsetispeed (&new_attr,B9600);

	//7.���ñ������Ӻͽ���ʹ��
	new_attr.c_cflag |= CREAD | CLOCAL;

	//8.���õȴ�ʱ������ٽ����ַ�
	new_attr.c_cc[VTIME] = 0;
	new_attr.c_cc[VMIN]  = 1;

	//9.����Ϊԭʼģʽ��ָ��������������ַ����ᱻ������ͨ�ַ���
	cfmakeraw (&new_attr);

	//10.ȷ��������Ч ��������
	tcsetattr(uart1_fd, TCSANOW,&new_attr);

	//11.����������������
	tcflush (uart1_fd,TCIOFLUSH);


}

 socket���  ����io �̳߳�

int UART1_Init2()
{

    //��RFID�Ķ�����������ָ�������Ӧ��Ϣ
    RFID_GetBCC(request_buff);//����У���
	int err_rfid = 0;
    for(;;)
    {
        //ˢ�´��ڻ�����
        tcflush (uart1_fd,TCIOFLUSH);
        // ����ָ���RFIDģ��
        write (uart1_fd,request_buff,sizeof(request_buff));
        usleep(50*1000);
        //  ����,��������Ӧ��Ϣ
        read(uart1_fd,recv_buf_rfid,sizeof(recv_buf_rfid));

        //�ж���Ӧ��Ϣ�Ƿ�ɹ�
        if(recv_buf_rfid[2] == 0x00)
        {
            printf("sucess read the card!\n");
            bzero(recv_buf_rfid,sizeof(recv_buf_rfid));		//�������
            RFID_read_sucess_flag =  PiccAnticoll(uart1_fd);
             break;
        }


    }

	return RFID_read_sucess_flag;

}



//����ײ
int PiccAnticoll(int fd)
{
	int ret;
	unsigned char tx_buf[8];//���Ҫ���͵�����
	int card_ID=0;
	tx_buf[0]=0x08;//�����
	tx_buf[1]=0x02;//��������
	tx_buf[2]='B';//����
	tx_buf[3]=0x02;//���ݳ���
	tx_buf[4]=0x93;//��ײ����ѡ��1��
	tx_buf[5]=0x00;
	tx_buf[6]=CalBCC(tx_buf,tx_buf[0]-2);//У��
	tx_buf[7]=0x03;//����

	write(uart1_fd,tx_buf,8);//�������ݸ��ӻ�
	usleep(50000);//��ʱ
	ret=read(uart1_fd,rt_buf,10);//���մӻ�Ӧ����Ϣ
	if(ret<0)//���շ���ֵС��0����ʧ��
	{
		printf("read err\n");
		return 0;
	}
	if(ret==10)//���յ�����10������
	{
		if(rt_buf[2]==0x00)//�ж�statusλΪ0��ʾ�ɹ�
		{
			card_ID=(rt_buf[7]<<24)|(rt_buf[6]<<16)|(rt_buf[5]<<8) |rt_buf[4];//��ȡbuf������
			printf("ID=%x\n",card_ID);
			return 1;
		}
	}
	return 2;
}






//����BCC SerBfr��0x07  BCC ǰ��������ȡ��
//��������֡��У���
void RFID_GetBCC(char *buf)
{
	int i;
	char BCC = 0;

	for(i=0; i<(buf[0]-2); i++)
	{
		BCC ^= buf[i];
	}
	buf[buf[0]-2] = ~BCC;
}



//У��
unsigned char CalBCC(unsigned char *buf,int n)
{
	int i;
	unsigned char bcc=0;
	for(i=0;i<n;i++)
	{
		bcc^=*(buf+i);
	}
	return (~bcc);
}