
#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "ESPAsyncWebServer.h" //异步服务
#include <LittleFS.h>		   //使用littleFS文件系
#include "wifiWeb.h"
#include "oled.h"

// 是否开启串口调试输出
#define Debug 0

void DebugPrt(const char *str)
{
#if Debug
	Serial.print(str);
#else
	(void)str;
#endif
}

void DebugPrt(String str)
{
#if Debug
	Serial.print(str);
#else
	(void)str;
#endif
}

//

const char *wifiname = "Melody";
const char *wifipassword = "12345678";

static volatile int num = 0;

// 函数申明
static void handleUserRequest(AsyncWebServerRequest *request);
static bool handleFileRead(String resource, AsyncWebServerRequest *request);
static String getContentType(String filename);
static void handlSetNum(AsyncWebServerRequest *request);
static void webserveInit(void);
static void wifiinit(void);

// wifi设置

static AsyncWebServer esp8266_server(80); // 声明WebServer对象 esp8266_server(80); // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）

static String IP;

int GetRevNum(void)
{
	return num;
}

const char* GetWifiName(void)
{
	return wifiname ;
}

const char* GetWifipwd(void)
{
	return wifipassword;
}

String GetWifiIp(void)
{
	return IP;
}

void WifiAndServeInit(void)
{

	wifiinit();

	INF_ipdis();

	webserveInit();
}

//------------------------------------------------ 静态函数---------------------------------------------
// 选择一种wifi模式打开
#define ApMod 1
#define WifiMod 0

static void wifiinit(void)
{
#if WifiMod
	WiFi.mode(WIFI_STA);
	static ESP8266WiFiMulti wifiMulti;		 // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti
	wifiMulti.addAP(wifiname, wifipassword); // ESP8266-NodeMCU再启动后会扫描当前网络

	DebugPrt("Connecting ...");
	INF_wifi_wiat();
	if (wifiMulti.run() != WL_CONNECTED)
	{
		INF_wifi_err();
		while (1)
			;
	}
#elif ApMod
	WiFi.mode(WIFI_AP);
	WiFi.softAP(wifiname, wifipassword);
	IP = WiFi.softAPIP().toString();
#endif

	// WiFi连接成功后将通过串口监视器输出连接成功信息
	DebugPrt("Connected to ");
	DebugPrt(WiFi.SSID()); // 通过串口监视器输出连接的WiFi名称
	DebugPrt("\n");

	DebugPrt("IP address:\t");
	DebugPrt(IP); // 通过串口监视器输出ESP8266-NodeMCU的IP
	DebugPrt("\n");
}

static void webserveInit(void)
{
	if (LittleFS.begin())
	{ // 启动闪存文件系统
		DebugPrt("LittleFS Started\n");
	}
	else
	{
		DebugPrt("LittleFS Failed to Start.\n");
	}

	esp8266_server.on("/choosNum", HTTP_GET, handlSetNum); 	//绑定函数 /choosNum 在网页get请求的资源地址
	esp8266_server.onNotFound(handleUserRequest); 			//处理其它网络请求 来发送网页数据到客户端
	// 启动网站服务
	esp8266_server.begin();
	DebugPrt("HTTP server started");
}

int resflag = 0;

static void handlSetNum(AsyncWebServerRequest *request)
{
	String res = request->getParam("num")->value();
	num = res.toInt(); 	// 参考静态网页javascrpt    xmlhttp.open("GET", "choosNum?num="+num, true);  get带参数请求
	resflag = 1; 		// 接收到数据标志位
}

// 处理用户浏览器的HTTP访问 - ----固定函数------------
static void handleUserRequest(AsyncWebServerRequest *request)
{
	// 获取用户请求资源地址(Request Resource）
	String reqResource = request->url();

	// 通过handleFileRead函数处处理用户请求资源
	bool fileReadOK = handleFileRead(reqResource, request);

	// 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
	if (!fileReadOK)
	{
		request->send(404, "text/plain", "404 Not Found");
	}
}

static bool handleFileRead(String resource, AsyncWebServerRequest *request)
{ // 处理浏览器HTTP访问

	if (resource.endsWith("/"))
	{							  // 如果访问地址以"/"为结尾
		resource = "/index.html"; // 则将访问地址修改为/index.html便于SPIFFS访问
	}

	String contentType = getContentType(resource); // 获取文件类型

	if (LittleFS.exists(resource))
	{													// 如果访问的文件可以在SPIFFS中找到
		LittleFS.open(resource, "r");					// 则尝试打开该文件
		request->send(LittleFS, resource, contentType); // 并且将该文件返回给浏览器
		return true;									// 返回true
	}
	return false; // 如果文件未找到，则返回false
}

// 获取文件类型
static String getContentType(String filename)
{
	if (filename.endsWith(".htm"))
		return "text/html";
	else if (filename.endsWith(".html"))
		return "text/html";
	else if (filename.endsWith(".css"))
		return "text/css";
	else if (filename.endsWith(".js"))
		return "application/javascript";
	else if (filename.endsWith(".png"))
		return "image/png";
	else if (filename.endsWith(".gif"))
		return "image/gif";
	else if (filename.endsWith(".jpg"))
		return "image/jpeg";
	else if (filename.endsWith(".ico"))
		return "image/x-icon";
	else if (filename.endsWith(".xml"))
		return "text/xml";
	else if (filename.endsWith(".pdf"))
		return "application/x-pdf";
	else if (filename.endsWith(".zip"))
		return "application/x-zip";
	else if (filename.endsWith(".gz"))
		return "application/x-gzip";
	return "text/plain";
}