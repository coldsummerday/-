#include "SCoop.h"
#include "ros.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Float64.h"
ros::NodeHandle nh;
#define arduinoOut1 24
#define arduinoOut2 25

int led_msg=10;


#define NOTE_D0 -1
#define NOTE_D1 262
#define NOTE_D2 293
#define NOTE_D3 329
#define NOTE_D4 349
#define NOTE_D5 392
#define NOTE_D6 440
#define NOTE_D7 494

#define NOTE_DL1 147
#define NOTE_DL2 165
#define NOTE_DL3 175
#define NOTE_DL4 196
#define NOTE_DL5 221
#define NOTE_DL6 248
#define NOTE_DL7 278



#define NOTE_DH1 523
#define NOTE_DH2 586
#define NOTE_DH3 658
#define NOTE_DH4 697
#define NOTE_DH5 783
#define NOTE_DH6 879
#define NOTE_DH7 987
#define WHOLE 1

#define HALF 0.5

#define QUARTER 0.25

#define EIGHTH 0.25
#define SIXTEENTH 0.625
#define pinLightBlocking 27
const int buttonPin = A0; 
int inputValue = 0;  
int preState=0;
int curState=0;
//这部分是用英文对应了拍子，这样后面也比较好看

int tune[] =

{

  NOTE_DH1, NOTE_D6, NOTE_D5, NOTE_D6, NOTE_D0,

  NOTE_DH1, NOTE_D6, NOTE_D5, NOTE_DH1, NOTE_D6, NOTE_D0, NOTE_D6,

  NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D6, NOTE_D0, NOTE_D6,

  NOTE_DH1, NOTE_D6, NOTE_D5, NOTE_DH1, NOTE_D6, NOTE_D0,



  NOTE_D1, NOTE_D1, NOTE_D3,

  NOTE_D1, NOTE_D1, NOTE_D3, NOTE_D0,

  NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D6,

  NOTE_D5, NOTE_D1, NOTE_D3, NOTE_D0,

  NOTE_DH1, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D6,

  NOTE_D5, NOTE_D1, NOTE_D2, NOTE_D0,

  NOTE_D7, NOTE_D7, NOTE_D5, NOTE_D3,

  NOTE_D5,

  NOTE_DH1, NOTE_D0, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D5, NOTE_D6, NOTE_D6,

  NOTE_D0, NOTE_D5, NOTE_D1, NOTE_D3, NOTE_D0,

  NOTE_DH1, NOTE_D0, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D5, NOTE_D6, NOTE_D6,

  NOTE_D0, NOTE_D5, NOTE_D1, NOTE_D2, NOTE_D0,

  NOTE_D3, NOTE_D3, NOTE_D1, NOTE_DL6,

  NOTE_D1,

  NOTE_D3, NOTE_D5, NOTE_D6, NOTE_D6,

  NOTE_D3, NOTE_D5, NOTE_D6, NOTE_D6,

  NOTE_DH1, NOTE_D0, NOTE_D7, NOTE_D5,

  NOTE_D6,

};//这部分就是整首曲子的音符部分，用了一个序列定义为tune，整数



float duration[] =

{

  1, 1, 0.5, 0.5, 1,

  0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5,

  0.5, 1, 0.5, 1, 0.5, 0.5,

  0.5, 0.5, 0.5, 0.5, 1, 1,



  1, 1, 1 + 1,

  0.5, 1, 1 + 0.5, 1,

  1, 1, 0.5, 0.5, 1,

  0.5, 1, 1 + 0.5, 1,

  0.5, 0.5, 0.5, 0.5, 1 + 1,

  0.5, 1, 1 + 0.5, 1,

  1 + 1, 0.5, 0.5, 1,

  1 + 1 + 1 + 1,

  0.5, 0.5, 0.5 + 0.25, 0.25, 0.5 + 0.25, 0.25, 0.5 + 0.25, 0.25,

  0.5, 1, 0.5, 1, 1,

  0.5, 0.5, 0.5 + 0.25, 0.25, 0.5 + 0.25, 0.25, 0.5 + 0.25, 0.25,

  0.5, 1, 0.5, 1, 1,

  1 + 1, 0.5, 0.5, 1,

  1 + 1 + 1 + 1,

  0.5, 1, 0.5, 1 + 1,

  0.5, 1, 0.5, 1 + 1,

  1 + 1, 0.5, 0.5, 1,

  1 + 1 + 1 + 1

};//这部分是整首曲子的接拍部分，也定义个序列duration，浮点（数组的个数和前面音符的个数是一样的，一一对应么）

int length;//这里定义一个变量，后面用来表示共有多少个音符

int tonePin = 9; //蜂鸣器的pin

int val = 0;
int digitalPin = 36;

int ledPin = 13;
int sensorPin = 0;

double alpha = 0.75;
int period = 20;
double change = 0.0;


void led_messageCb(const std_msgs::Int32&msg)
{
  led_msg = msg.data;
}
void buzzer_messageCb(const std_msgs::Int32&msg)
{
    val = 1;
}
std_msgs::Int32 light_block_msg;
ros::Publisher light_block_talker("light_block", &light_block_msg);
ros::Subscriber<std_msgs::Int32> ledsuber("led", &led_messageCb );

ros::Subscriber<std_msgs::Int32> buzzersuber("buzzer",&buzzer_messageCb);

void setup()
{
  nh.initNode();
  nh.advertise(light_block_talker);
   nh.subscribe(ledsuber);
   nh.subscribe(buzzersuber);
   mySCoop.start(); 

    
}
void talk_light(int data)
{
   
   light_block_msg.data = data;
    light_block_talker.publish(&light_block_msg);
}

void loop()
{

  yield();
  nh.spinOnce();

}
//led灯的控制
defineTask(Task2);
  void Task2::setup()
  {

    pinMode(arduinoOut1, OUTPUT); 
    pinMode(arduinoOut2, OUTPUT);
    digitalWrite(arduinoOut1, LOW);  
    digitalWrite(arduinoOut2, LOW); 
  }
  void Task2::loop()
  {
    
    //闪烁提醒
    if(led_msg == 1){
      digitalWrite(arduinoOut1, HIGH);    
      digitalWrite(arduinoOut2, LOW); 
      sleep(200); 

      digitalWrite(arduinoOut1, LOW);    
      digitalWrite(arduinoOut2, LOW); 
      sleep(200); 
     
      digitalWrite(arduinoOut1, LOW); 
      digitalWrite(arduinoOut2, HIGH);   
      sleep(200); 

      digitalWrite(arduinoOut1, LOW);    
      digitalWrite(arduinoOut2, LOW); 
      sleep(200); 
    }
    //熄灭
    else if (led_msg==2){
      digitalWrite(arduinoOut1, LOW);
      digitalWrite(arduinoOut2, LOW);
    }
    //色1
    else if (led_msg==3){
      digitalWrite(arduinoOut1, HIGH);
      digitalWrite(arduinoOut2, LOW);
    }
    //色2
    else if (led_msg==4){
      digitalWrite(arduinoOut1, LOW);
      digitalWrite(arduinoOut2, HIGH);
    }
    //混色色
    else if (led_msg==5){
      digitalWrite(arduinoOut1, HIGH);
      digitalWrite(arduinoOut2, HIGH);
    }
  }



defineTask(Task1);
void Task1::setup()
{
  
  pinMode(tonePin, OUTPUT); //设置蜂鸣器的pin为输出模式

  length = sizeof(tune) / sizeof(tune[0]); //这里用了一个sizeof函数，可以查出tone序列里有多少个音符
}
void Task1::loop()
{
  if (val == 1) {
    for (int x = 0; x < 5; x++)
    {
      tone(tonePin, tune[x]); //此函数依次播放tune序列里的数组，即每个音符

      sleep(400 * duration[x]); //每个音符持续的时间，即节拍duration，400是调整时间的越大，曲子速度越慢，越小曲子速度越快，自己掌握吧

      noTone(tonePin);//停止当前音符，进入下一音符
    }
  }
  val = 0;
}

defineTask(LightBlocking);
void LightBlocking::setup()
{
	
	pinMode(pinLightBlocking, INPUT);
}
void LightBlocking::loop()
{
	 curState = digitalRead(pinLightBlocking);
	if(preState==0 && curState==1){
		talk_light(state);
	}else if(preState==1 && curState==0){
		talk_light(state);
	}
	preState = curState;

}

