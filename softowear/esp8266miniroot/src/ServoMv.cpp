#include "Arduino.h"
#include "Servo.h"
#include "ServoMv.h"

// 声明8个舵机对象
static Servo ser1;
static Servo ser2;
static Servo ser3;
static Servo ser4;
static Servo ser5;
static Servo ser6;
static Servo ser7;
static Servo ser8;

/**
 * @brief 量程转换
 * @param input 输入的值
 * @param osh 输出的高量程
 * @param osl 输出的低量程
 * @param ish 输入的高量程
 * @param isl 输入的低量程
 * @retval 转换的值
 */
int revvaule(float input, float osh, float osl, float ish, float isl)
{
  return (((osh - osl) * (input - isl) / (ish - isl)) + osl);
}

// 初始化
void ServoInit(void)
{
  ser1.attach(SE1, 500, 2500);
  ser2.attach(SE2, 500, 2500);
  ser3.attach(SE3, 500, 2500);
  ser4.attach(SE4, 500, 2500);
  ser5.attach(SI1, 500, 2500);
  ser6.attach(SI2, 500, 2500);
  ser7.attach(SI3, 500, 2500);
  ser8.attach(SI4, 500, 2500);
}

// 去初始化
void ServoDeInit(void)
{
  ser1.detach();
  ser2.detach();
  ser3.detach();
  ser4.detach();
  ser5.detach();
  ser6.detach();
  ser7.detach();
  ser8.detach();
}

/**
 * @brief  每个关节单独设置us
 * @param PIN
 * @param AG
 */
void Setangelus(RootPin PIN, int us)
{
  switch (PIN)
  {
  case SE1:
    ser1.writeMicroseconds(us);
    break;
  case SE2:
    ser2.writeMicroseconds(us);
    break;
  case SE3:
    ser3.writeMicroseconds(us);
    break;
  case SE4:
    ser4.writeMicroseconds(us);
    break;

  case SI1:
    ser5.writeMicroseconds(us);
    break;
  case SI2:
    ser6.writeMicroseconds(us);
    break;
  case SI3:
    ser7.writeMicroseconds(us);
    break;
  case SI4:
    ser8.writeMicroseconds(us);
    break;

  default:
    break;
  }
}

/**
 * @brief  每个关节单独设置角度
 * @param PIN
 * @param AG
 */
void Setangelag(RootPin PIN, int AG)
{
  switch (PIN)
  {
  case SE1:
    ser1.write(AG);
    break;
  case SE2:
    ser2.write(AG);
    break;
  case SE3:
    ser3.write(AG);
    break;
  case SE4:
    ser4.write(AG);
    break;

  case SI1:
    ser5.write(AG);
    break;
  case SI2:
    ser6.write(AG);
    break;
  case SI3:
    ser7.write(AG);
    break;
  case SI4:
    ser8.write(AG);
    break;

  default:
    break;
  }
}

// 获取上一次us
int Getangelus(RootPin PIN)
{
  int ag = 0;
  switch (PIN)
  {
  case SE1:
    ag = 2500 - ser1.readMicroseconds() + 500;
    break;
  case SE2:
    ag = 2500 - ser2.readMicroseconds() + 500;
    break;
  case SE3:
    ag = ser3.readMicroseconds();
    break;
  case SE4:
    ag = ser4.readMicroseconds();
    break;

  case SI1:
    ag = 2500 - ser5.readMicroseconds() + 500;
    break;
  case SI2:
    ag = 2500 - ser6.readMicroseconds() + 500;
    break;
  case SI3:
    ag = ser7.readMicroseconds();
    break;
  case SI4:
    ag = ser8.readMicroseconds();
    break;

  default:
    break;
  }

  return ag;
}

// 获取上一次角度
int Getangelag(RootPin PIN)
{
  int ag = 0;
  switch (PIN)
  {
  case SE1:
    ag = ser1.read();
    break;
  case SE2:
    ag = ser2.read();
    break;
  case SE3:
    ag = ser3.read();
    break;
  case SE4:
    ag = ser4.read();
    break;

  case SI1:
    ag = ser5.read();
    break;
  case SI2:
    ag = ser6.read();
    break;
  case SI3:
    ag = ser7.read();
    break;
  case SI4:
    ag = ser8.read();
    break;

  default:
    break;
  }

  return ag;
}

/**
 * @brief  设置单个关节 可以调速
 * @param PIN 那个关节
 * @param AG  要到达的角度
 * @param ms  过程时间
 */
void SetangelStep(RootPin PIN, int16_t AG, uint16_t ms)
{

  int16_t count = ms / 20; // 20ms(pwm周期) 一个间隔

  int16_t lastag = Getangelus(PIN); // 获取上一次的占空比时间us
  int16_t lastagbk = lastag;        // 备份时间

  AG = revvaule(AG, 2500, 500, 180, 0); // 量程转换 目标角度转换成目标占空比时间us

  int16_t dertag = 0; // 目标时间与当前时间的误差

  // 算出最大误差
  if (AG > lastag)
  {
    dertag = AG - lastag;
  }
  else
  {
    dertag = lastag - AG;
  }

  int16_t inc = (dertag - 1) / count + 1; // 计算出每次减小误差的 步进值 向上取整

  while (1)
  {

    // 每次循环减小一次误差
    if (AG > lastagbk)
    {
      lastag += inc;
      if (lastag >= AG)
        return;
    }
    else
    {
      lastag -= inc;
      if (lastag <= AG)
        return;
    }

    // 设定计算出的占空比时间
    Setangelus(PIN, lastag);
    delay(20); // 保持一个周期
  }
}

#if 0 /* 设定肩部关节调速运动  已经重写*/

/*          控制结构体
0 ag / AG  //设定角度
1 ms / inc //设定速度
2 lastus
3 lastusbak
4 enable / disable
5 BandPin  //绑定关节顺序
 */



typedef struct
{
  uint16_t StepCtr[5][4] = {0};
  RootPin BandPin[4];
} StepCtrHandle;

StepCtrHandle g_step1 =
    {
        .BandPin = {SE1,
                    SE2,
                    SE3,
                    SE4,
                   }};

/**
 * @brief  设置任何肘部关节 可以调速
 * @param *StepCtrHandle  控制结构体
 */
void SetangelStepAllSe(StepCtrHandle *StepCtrHandle)
{
  uint16_t len = 4;

  for (uint16_t i = 0; i < len; i++)
  {

    // 计算目标us数
    StepCtrHandle->StepCtr[0][i] = revvaule(StepCtrHandle->StepCtr[0][i], 2500, 500, 180, 0);
    // 获取当前us数并备份
    StepCtrHandle->StepCtr[3][i] = StepCtrHandle->StepCtr[2][i] = Getangelus(StepCtrHandle->BandPin[i]);



    if (StepCtrHandle->StepCtr[0][i] > StepCtrHandle->StepCtr[2][i])
    {
      //计算出增量 注意要向上取整，增量为0，在误差判断里会无限循环。
      StepCtrHandle->StepCtr[1][i] = ((StepCtrHandle->StepCtr[0][i] - StepCtrHandle->StepCtr[2][i])-1) / (StepCtrHandle->StepCtr[1][i] / 20) +1;
    }
    else
    {
      StepCtrHandle->StepCtr[1][i] = ((StepCtrHandle->StepCtr[2][i] - StepCtrHandle->StepCtr[0][i])-1) / (StepCtrHandle->StepCtr[1][i] / 20) +1;
    }
  }

  while (1)
  {
    for (uint16_t i = 0; i < len; i++)
    {
      // 每次循环减小一次误差
      if (StepCtrHandle->StepCtr[0][i] > StepCtrHandle->StepCtr[3][i])
      {
        StepCtrHandle->StepCtr[2][i] += StepCtrHandle->StepCtr[1][i];
        if (StepCtrHandle->StepCtr[2][i] >= StepCtrHandle->StepCtr[0][i])
        {
          StepCtrHandle->StepCtr[4][i] = 0;
        }

      }
      else
      {
        StepCtrHandle->StepCtr[2][i] -= StepCtrHandle->StepCtr[1][i];
        if (StepCtrHandle->StepCtr[2][i] <= StepCtrHandle->StepCtr[0][i])
        {
         StepCtrHandle->StepCtr[4][i] = 0;
        }
         
      }
     
    }

     uint8_t isret = 1;
      for (uint16_t i = 0; i < 4; i++)
      {
        if (StepCtrHandle->StepCtr[4][i] == 1)
        {
          Setangelus(StepCtrHandle->BandPin[i], StepCtrHandle->StepCtr[2][i]);// 设定计算出的占空比时间
          isret = 0;
        }
        
      }
      if(isret == 1)
      {
        return;//所有关节到位退出
      }

    delay(20); // 保持一个周期
  }
}

#endif

#if 1 /* 设定肩部关节调速运动*/

/**
 * @brief  通过定义结构体来实现舵机的调速控制，支持自定义舵机数量，支持不同速度
 * @param  myservo_t *sr 舵机控制结构体，设定目标角度、到达目标的过程时间，是否使能，以及绑定引脚
 * @param  uint8_t nums  舵机数组的数量（长度）
 * @param  setfun fun 舵机的执行函数
 */
void SetangelStepSe(myservo_t *sr, uint8_t nums, setfun fun)
{
  for (uint16_t i = 0; i < nums; i++)
  {

    // 计算目标us数
    sr[i].us = revvaule(sr[i].us, 2500, 500, 180, 0);
    // 获取当前us数并备份
    sr[i].nowus = sr[i].usbak = Getangelus(sr[i].BandPin);

    // 计算出增量 注意要向上取整，增量为0，在误差判断里会无限循环。
    if (sr[i].us > sr[i].usbak) // 如果目标us大于当前us
    {
      sr[i].dertms = (sr[i].us - sr[i].usbak - 1) / (sr[i].dertms / 20 /* 计算可以循环多少个周期 */) + 1;
    }
    else
    {
      sr[i].dertms = (sr[i].usbak - sr[i].us - 1) / (sr[i].dertms / 20) + 1;
    }
  }

  while (1)
  {
    for (uint8_t i = 0; i < nums; i++)
    {
      // 每次循环减小一次误差
      if (sr[i].us > sr[i].usbak)
      {
        sr[i].nowus += sr[i].dertms;
        if (sr[i].nowus >= sr[i].us)
        {
          sr[i].en = 0;
        }
      }
      else
      {

        sr[i].nowus -= sr[i].dertms;
        if (sr[i].nowus <= sr[i].us)
        {
          sr[i].en = 0;
        }
      }
    }

    uint8_t isret = 1; // 可以返回标志
    for (uint8_t i = 0; i < nums; i++)
    {
      if (sr[i].en == 1) // 还没到达目标us
      {
        fun(sr[i].BandPin, sr[i].nowus); // 通过函数指针的函数 设定计算出的占空比时间
        isret = 0;
      }
    }
    if (isret == 1) // 所有关节都到位退出
    {
      return;
    }

    delay(20); // 保持一个周期
  }
}

#endif

#if 0 /* 通过时间比例切片控制 有Bug没法用*/

/**
 * @brief  设置所有肘部关节
 * @param PIN 那个脚
 * @param AG  要到达的角度
 * @param ms  过程时间
 */
void SetangelTime(RootPin PIN, int16_t AG, uint16_t ms)
{

  int16_t us = ms*1000 ;

  int16_t lastag = Getangelag(PIN);
  int16_t lastagbk = lastag;

  int16_t dertag = 0;


  if (AG > lastag)
  {
    dertag = AG - lastag;
  }
  else
  {
    dertag = lastag - AG;
  }

  int16_t dtimeus = us/dertag;

  while (1)
  {

    if (AG > lastagbk)
    {
      lastag ++;
      if (lastag>=AG) return;
    }
    else
    {
      lastag -- ;
      if (lastag<=AG) return;
    }

    Setangelag(PIN, lastag);
    delayMicroseconds(dtimeus);
  }
}

#endif /* 通过时间比例切片控制 有Bug*/

/********************肘部关节控制start**********************/

/**
 * @brief   设置所有肘部关节 无调速
 * @param us  目标us
 */
void SetAllSePinUs(RootPin pin, uint16_t us)
{
  if (us > 2500 || us < 500)
  {
    return;
  }
  if (pin == SE1)
    Setangelus(SE1, 2500 - us + 500);
  else if (pin == SE2)
    Setangelus(SE2, 2500 - us + 500);
  else if (pin == SE3)
    Setangelus(SE3, us);
  else if (pin == SE4)
    Setangelus(SE4, us);
}

/**
 * @brief   设置所有肘部关节 无调速
 * @param us  目标us
 */
void SetAllSePinUs(uint16_t us, uint8_t en1, uint8_t en2, uint8_t en3, uint8_t en4)
{
  if (us > 2500 || us < 500)
  {
    return;
  }
  if (en1)
    Setangelus(SE1, 2500 - us + 500);
  if (en2)
    Setangelus(SE2, 2500 - us + 500);
  if (en3)
    Setangelus(SE3, us);
  if (en4)
    Setangelus(SE4, us);
}

/**
 * @brief   设置所有肘部关节 无调速
 * @param ag  目标角度
 */
void SetAllSePin(uint8_t ag)
{
  if (ag > 180 || ag < 0)
  {
    return;
  }

  Setangelag(SE1, 180 - ag);
  Setangelag(SE2, 180 - ag);
  Setangelag(SE3, ag);
  Setangelag(SE4, ag);
}

/**
 * @brief  设置肘部关节 可选择是否使能
 * @param ag
 */
void SetAllSePin(uint8_t ag, uint8_t en1, uint8_t en2, uint8_t en3, uint8_t en4)
{
  if (ag > 180 || ag < 0)
  {
    return;
  }
  if (en1)
    Setangelag(SE1, 180 - ag);
  if (en2)
    Setangelag(SE2, 180 - ag);
  if (en3)
    Setangelag(SE3, ag);
  if (en4)
    Setangelag(SE4, ag);
}

// 声明控制结构体
static myservo_t g_myservose[4] = {
    {.BandPin = SE1},//绑定引脚
    {.BandPin = SE2},
    {.BandPin = SE3},
    {.BandPin = SE4},
};

void SetAllSePinStep(uint16_t ag, uint16_t ms, uint8_t en1, uint8_t en2, uint8_t en3, uint8_t en4)
{
  for (uint16 i = 0; i < 4; i++)
  {
    g_myservose[i].us = ag;
    g_myservose[i].dertms = ms;
  }
  g_myservose[0].en = en1;
  g_myservose[1].en = en2;
  g_myservose[2].en = en3;
  g_myservose[3].en = en4;

  SetangelStepSe(g_myservose, 4, SetAllSePinUs); // 传入肘部关节控制函数
}

void SetAllSePinStep(uint16_t ag, uint16_t ms)
{
  for (uint16 i = 0; i < 4; i++)
  {
    g_myservose[i].us = ag;
    g_myservose[i].dertms = ms;
    g_myservose[i].en = 1;
  }
  SetangelStepSe(g_myservose, 4, SetAllSePinUs); // 传入肘部关节控制函数
}

/********************肘部关节控制end**********************/

/********************肩部关节控制start**********************/

/**
 * @brief   设置所有肘部关节 无调速
 * @param us  目标us
 */
void SetAllSiPinUs(RootPin pin, uint16_t us)
{

  if (us < 1833 && us > 833) // ag/180*2000+500
  {
    if (pin == SI2)
      Setangelus(SI2, 2500 - us + 500);
    if (pin == SI4)
      Setangelus(SI4, us);
  }

  if (us < 2277 && us > 1277)
  {
    if (pin == SI1)
      Setangelus(SI1, 2500 - us + 500);
    if (pin == SI3)
      Setangelus(SI3, us);
  }
}

/**
 * @brief   设置所有肩部关节 无调速
 * @param us  目标us
 */
void SetAllSiPinUs(uint16_t us, uint8_t en1, uint8_t en2, uint8_t en3, uint8_t en4)
{

  if (us < 1833 && us > 833)
  {
    if (en2)
      Setangelus(SI2, 2500 - us + 500);
    if (en4)
      Setangelus(SI4, us);
  }

  if (us < 2277 && us > 1277)
  {
    if (en1)
      Setangelus(SI1, 2500 - us + 500);
    if (en3)
      Setangelus(SI3, us);
  }
}

/**
 * @brief  设置所有肩部关节
 * @param ag 目标角度
 */
void SetAllSiPin(uint8_t ag)
{
  if (ag < 120 && ag > 30)
  {
    Setangelag(SI2, 180 -ag);
    Setangelag(SI4, ag);
  }

  if (ag < 160 && ag > 70)
  {
    Setangelag(SI1, 180 -ag);
    Setangelag(SI3, ag);
  }
}

/**
 * @brief  设置肩部关节 可选择 可选择是否使能
 * @param ag
 */
void SetAllSiPin(uint8_t ag, uint8_t en1, uint8_t en2, uint8_t en3, uint8_t en4)
{

  if (ag < 120 && ag > 30)
  {
    if (en2)
      Setangelag(SI2, 180 - ag);
    if (en4)
      Setangelag(SI4, ag);
  }

  if (ag < 160 && ag > 70)
  {
    if (en1)
      Setangelag(SI1,180 - ag);

    if (en3)
      Setangelag(SI3, ag);
  }
}

// 声明控制结构体
static myservo_t g_myservosi[4] = {
    {.BandPin = SI1},
    {.BandPin = SI2},
    {.BandPin = SI3},
    {.BandPin = SI4},
};

void SetAllSiPinStep(uint16_t ag, uint16_t ms, uint8_t en1, uint8_t en2, uint8_t en3, uint8_t en4)
{
  for (uint16 i = 0; i < 4; i++)
  {
    g_myservosi[i].us = ag;
    g_myservosi[i].dertms = ms;
  }
  g_myservosi[0].en = en1;
  g_myservosi[1].en = en2;
  g_myservosi[2].en = en3;
  g_myservosi[3].en = en4;

  SetangelStepSe(g_myservosi, 4, SetAllSiPinUs); // 传入肘部关节控制函数
}

void SetAllSiPinStep(uint16_t ag, uint16_t ms)
{
  for (uint16 i = 0; i < 4; i++)
  {
    g_myservosi[i].us = ag;
    g_myservosi[i].dertms = ms;
    g_myservosi[i].en = 1;
  }
  SetangelStepSe(g_myservosi, 4, SetAllSiPinUs); // 传入肘部关节控制函数
}

/********************肩部关节控制end**********************/