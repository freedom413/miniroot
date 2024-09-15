#include <Arduino.h>
#include "oled.h"
#include "wifiWeb.h"
#include "ServoMv.h"
#include <U8g2lib.h>
#include "action.h"

#define ledPin D4

void VbatAdc(void);
void ALoop(void);
void ledRun(void);

#if 0 /*电池采样开始*/

#define dt 0.2

int yijei(int in)
{
  static int pre = 0;
  static bool onece = 1; 
  int now = (in*dt) + (pre*(1-dt));
 
  if (onece)
  {
    pre = now;
    onece = 0;
  }
  pre = now;
  return now;
}



void VbatAdc(void)
{
 int adcVale =  yijei(analogRead(A0));
 if (adcVale<400)
 {

  ServoDeInit();

  getoledhand().clearBuffer();
  getoledhand().setCursor(0, 15); // 方式2，设置光标到达0 15位置
  getoledhand().print("adclow");
  getoledhand().sendBuffer();


  /* code */
 }else
 {
  ServoInit();
 }
 
}

#endif /*电池采样结束*/

// led灯翻转
void ledRun(void)
{
  digitalWrite(ledPin, digitalRead(ledPin) ? 0 : 1);
}

// 根据设定的值设置动作和表情
void ActionSet(int num)
{

  switch (num)
  {
  case 1:
    BQ_zc();
    MV_zcc();
    break;
  case 2:
    BQ_nh();
    MV_go();
    break;
  case 3:
    BQ_gp();
    MV_ly();
    break;
  case 4:
    BQ_la();
    MV_zz();
    break;
  case 5:
    BQ_sj();
    MV_zs();
    break;
  case 6:
    BQ_TT();
    MV_yz();
    break;
  case 7:
    BQ_yb();
    MV_yba();
    break;
  case 8:
    BQ_gp();
    MV_haha();
    break;
  case 9:
    BQ_tx();
    MV_yo();
    MV_Jump();
    break;
  case 10:
    BQ_tx();
    MV_fwc();
    break;
  case 11:
    BQ_yh();
    MV_D1();
    break;
  case 12:
    BQ_yb();
    MV_D2();
    ;
    break;
  default:
    break;
  }
}

// 调度运转
void ALoop(void)
{
  if (resflag)
  {
    ActionSet(GetRevNum());
    resflag = 0;
  }
}

// 简易的分时调度

unsigned long LedpreviousMillis = 0;
unsigned long WebpreviousMillis = 0;

void setup()
{

  Serial.begin(115200);
  // 初始化oled
  OledInit();
  // 初始化wifi服务器
  WifiAndServeInit();

  Serial.end();

  // 初始化舵机
  ServoInit();

  // 设置初值姿态
   MV_zcc();

 

  pinMode(ledPin, OUTPUT);
  pinMode(A0, INPUT);

  // 获取初始值
  WebpreviousMillis = LedpreviousMillis = millis();
}

// 多少ms进行一次调度
#define ledtime (1000)
#define webtime (50)

void loop()
{

  if (millis() - LedpreviousMillis >= ledtime)
  {
    LedpreviousMillis = millis();

    // 运行内容
    {
      ledRun();//翻转led
    }
  }
  else if (millis() - WebpreviousMillis >= webtime)
  {
    WebpreviousMillis = millis();

    // 运行内容
    {
      ALoop();//调度动作执行
    }
  }


}