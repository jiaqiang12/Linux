/**********************************************************************
                 Maxwell GuangZhou   2023.06-2023.07

    线程1 获取触摸屏坐标xy ok
    线程2 动态gif 2张做老师学生考勤方式的界面
    线程3 摄像头捕捉
    线程4 服务器端检测消息
    线程5 获取RFID卡号
    线程5 蓝牙
    线程6 手势识别    -----------至少六个线程

    语音指令2 ："我是学生"   "我是老师"
    语音指令1 ："刷卡考勤"   "蓝牙考勤"  "人脸考勤"  "密码考勤"  "手势"
    手势识别
    超声波传感器: 和TCP，STT、TTS结合
    开机： 播放两遍boot


GDB调试：调试的



***********************************************************************/
#include "Project.h" //自己写的头文件

/*******************************Flags***********************************/

int cap_start = 0;           //开启捕获标志位


int main(int argc, char const *argv[])
{

    int lcd_init_ret = lcd_init(); //初始化 显示 和 触摸
    if (lcd_init_ret == -1)   {printf("初始化失败！\n"); return -1; }
    else                      {printf("触摸屏初始化成功！\n");            }
    wave2();

    int ret = ControlMianUI();//控制模块--坐标获取、分析
    if (0 ==ret)
    printf("线程创建成功！\n");



    while(1)
    {



    }



	return 0;
}

















