/*
 * @Author: your name
 * @Date: 2024-09-11 12:31:51
 * @LastEditors: your name
 * @LastEditTime: 2024-09-12 13:03:48
 * @Description:
 * @FilePath: \esp8266miniroot\src\ServoMv.h
 */
#ifndef __SERVOMV_H__
#define __SERVOMV_H__

/* 定义八个关节io口 */

typedef enum
{
    SE1 = D2,
    SE2 = D6,
    SE3 = D1,
    SE4 = D7,
    SI1 = D3,
    SI2 = D5,
    SI3 = D0,
    SI4 = D8,

} RootPin;

typedef struct
{
/*控制变量*/
  uint16_t us;     // 目标角度 -在运算时时转换后成us
  uint16_t dertms; // 设定速度(ms) - 在运算时为增量
  uint8_t en;      // 是否使能
  RootPin BandPin; // 绑定的关节
/*运算过程变量*/
  uint16_t usbak; // 最初的us备份
  uint16_t nowus; // 当前的us

} myservo_t;

typedef void (*setfun)(RootPin, uint16_t); // 函数指针取别名 自己的舵机角度设定函数

/**
 * @brief 量程转换
 * @param input 输入的值
 * @param osh 输出的高量程
 * @param osl 输出的低量程
 * @param ish 输入的高量程
 * @param isl 输入的低量程
 * @retval 转换的值
 */
int revvaule(float input, float osh, float osl, float ish, float isl);

void ServoInit(void);
void ServoDeInit(void);

void Setangelus(RootPin PIN, int us);
void Setangelag(RootPin PIN, int AG);
int Getangelus(RootPin PIN);
int Getangelag(RootPin PIN);

// 不可调速
void SetAllSePin(uint8_t ag);
void SetAllSePin(uint8_t ag, uint8_t en1, uint8_t en2, uint8_t en3, uint8_t en4);
void SetAllSiPin(uint8_t ag);
void SetAllSiPin(uint8_t ag, uint8_t en1, uint8_t en2, uint8_t en3, uint8_t en4);

// 可调速
void SetangelStep(RootPin PIN, int16_t AG, uint16_t ms);

void SetAllSePinStep(uint16_t ag, uint16_t ms);
void SetAllSePinStep(uint16_t ag, uint16_t ms, uint8_t en1, uint8_t en2, uint8_t en3, uint8_t en4);
void SetAllSiPinStep(uint16_t ag, uint16_t ms);
void SetAllSiPinStep(uint16_t ag, uint16_t ms, uint8_t en1, uint8_t en2, uint8_t en3, uint8_t en4);


#endif