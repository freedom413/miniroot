/*
 * @Author: your name
 * @Date: 2024-09-11 12:31:51
 * @LastEditors: your name
 * @LastEditTime: 2024-09-12 18:10:24
 * @Description: 
 * @FilePath: \esp8266miniroot\src\wifiWeb.h
 */
#ifndef __WIFIWEB_H__
#define __WIFIWEB_H__




const char* GetWifiName(void);
String GetWifiIp(void);
const char* GetWifipwd(void);

void WifiAndServeInit(void);


void DebugPrt(const char *str);
void DebugPrt(String str);

//接收到新数据标志位
extern int resflag;
//实际接收的数
int GetRevNum(void);	
#endif