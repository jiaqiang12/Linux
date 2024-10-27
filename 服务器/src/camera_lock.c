/**********************************************************************
                 Maxwell GuangZhou   2023.06-2023.07

       ��������ҳ���£������������



***********************************************************************/
#include "Project.h"

int true_key[4] = {3,8,9,0}; //��ȷ����
unsigned int key_num = 0;    //�����������: >=0
int input_key[5] = {0};      //��������Ĵ���

int panduan = 0;             //�����ж�

int erro_num = 0;            //��¼�������

int del_work_flag = 0;            //==0ɾ������Ч


int  camera_lock(void )
{

  while (1) //��������
   {
            while (1)
        {
            // label1 :panduan = 0;
            if (x > 500 && x < 590 && y > 400 && y < 480&&key_num<=3)      //��ť0
            {
                printf("input_key0\n");
                key_num++;
                keyshow();
                input_key[key_num] = 0;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 500 && x < 590 && y > 150 && y < 230&&key_num<=3)       //��ť1
            {
                printf("input_key1\n");
                key_num++;
                keyshow();
                input_key[key_num] = 1;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 590 && x < 680 && y > 150 && y < 230&&key_num<=3)       //��ť2
            {
                printf("input_key2\n");
                key_num++;
                keyshow();
                input_key[key_num] = 2;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 680 && x < 780 && y > 150 && y < 230&&key_num<=3)        //��ť3
            {
                printf("input_key3\n");
                key_num++;
                keyshow();
                input_key[key_num] = 3;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 500 && x < 590 && y > 230 && y < 313&&key_num<=3)       //��ť4
            {
                printf("input_key4\n");
                key_num++;
                keyshow();
                input_key[key_num] = 4;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 590 && x < 680 && y > 230 && y < 313&&key_num<=3)       //��ť5
            {
                printf("input_key5\n");
                key_num++;
                keyshow();
                input_key[key_num] = 5;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 680 && x < 780 && y > 230 && y < 313&&key_num<=3)       //��Ŧ6
            {
                printf("input_key6\n");
                key_num++;
                keyshow();
                input_key[key_num] = 6;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 500 && x < 590 && y > 313 && y < 390&&key_num<=3)       //��ť7
            {
                printf("input_key7\n");
                key_num++;
                keyshow();
                input_key[key_num] = 7;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 590 && x < 680 && y > 313 && y < 390&&key_num<=3)       //��ť8
            {
                printf("input_key8\n");
                key_num++;
                keyshow();
                input_key[key_num] = 8;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }

            if (x > 680 && x < 780 && y > 313 && y < 390&&key_num<=3)       //��ť9
            {
                printf("input_key9\n");
                key_num++;
                keyshow();
                input_key[key_num] = 9;
                del_work_flag = 1;
                x = 0; y =0;
                        break;
            }


            if(key_num == 0 )

            {
                del_work_flag = 0;
                break;
            }


            if (x > 590 && x < 670 && y > 400 && y < 480 && del_work_flag == 1&&(key_num>0))   // ɾ����, flag = 1 ������
            {
                printf("ɾ����\n");
                key_num--;
                keyshow();
                x = 0; y =0;
                break;
            }

            // if (key_num == 4) //�˳�����
            // {
            //     printf("����ﵽ4λ\n");
            //     break;
            // }

            if (x > 670 && x < 800 && y > 400 && y < 480 && (key_num == 4)
                 &&(face_reconizing ==1)&&(pass_mutex == 0) )   // ENTER��
            {
                 printf("ENTER��\n");
                 printf("����ﵽ%dλ\n",key_num);


                // for (panduan = 0; panduan < 4; panduan++)//�ж�������ȷ��
                //     {
                //         if (input_key[panduan] != true_key[panduan])            //�����������������
                //         {
                //             erro_num += 1;    //�������+1
                //             printf("������󣬴��������%d\n",erro_num);
                //             memset(input_key,0,sizeof(input_key));

                //                   //�������������
                //            key_num = 0;
                //            keyshow();
                //             x = 0; y =0;
                //             break;
                //                                         //һ���д���������ǰѭ��
                //         }

                //     }//&&(input_key[1] == 8)
                memset(input_key,0,5);

                 del_work_flag = 0;            //==0ɾ������Ч
                key_num = 0;
                cap_start == 0;

                x = 0; y =0;

                // if((input_key[0] == true_key[0]))
                return 1;
                // else return 0;


                break;
              //    pthread_exit(NULL);   //��������ɹ� ��ֹ�߳�

            }



        }   //Сѭ������

        //   if(erro_num == 3)  {

        //                 sleep(2);

        //                 return 0;

        //                 }


        //     if (panduan == 4)             //��ȷ
        //     {

        //         erro_num = 0;            //�����������

        //         panduan = 0;
        //         face_succes_flag = 1;
        //         cap_start = 0;
                // return 1;

            // }

    }       //��ѭ������


}








int keyshow() //����ʱ��ͼƬ�л�
    {
        switch (key_num)
        {
        case 0:
            read_JPEG_file("pass_disp0.jpg",470,0);
            break;

        case 1:
           read_JPEG_file("pass_disp1.jpg",470,0);
            break;

        case 2:
           read_JPEG_file("pass_disp2.jpg",470,0);
            break;

        case 3:
            read_JPEG_file("pass_disp3.jpg",470,0);
            break;

        case 4:
            read_JPEG_file("pass_disp4.jpg",470,0);
            break;

        default:
            break;
        }
    }