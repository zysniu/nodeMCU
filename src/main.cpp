
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
 
static WiFiClient espClient;
#include <AliyunIoTSDK.h>  //引入阿里云 IoT SDK，需要安装crypto库、PubSubClient库
#include <ArduinoJson.h>
AliyunIoTSDK iot;

//设置产品和设备的信息，从阿里云设备信息里查看
#define PRODUCT_KEY "a1mILnIgKB1"  //替换自己的PRODUCT_KEY
#define DEVICE_NAME "led01"  //替换自己的DEVICE_NAME
#define DEVICE_SECRET "ffab820469474f574cb466216a88fac6"  //替换自己的DEVICE_SECRET
#define REGION_ID "cn-shanghai"  //默认cn-shanghai
 
unsigned long lastMsMain = 0;
int pinRelay = 2; //esp01 上可以使用 3引脚 也就是RX引脚
int liangdu = 50; //
int kaiguan=0;//开关

ESP8266WebServer server(80);
void homepage() {
  server.send(200, "text/plain", "test homepage !");
  Serial.println("用户访问了主页");
}

//电源的属性修改的回调函数1
void powerstate(JsonVariant L)//固定格式，修改参数l
{
  int powerstate = L["powerstate"];//参数l
  if (powerstate == 0)
  {
    digitalWrite(pinRelay, LOW);
    kaiguan=0;
    analogWrite(pinRelay,0); 
    //digitalWrite(0, LOW);
  }
  else 
  {

    kaiguan=1;
    analogWrite(pinRelay,liangdu*10+24); 

  }
  Serial.printf("收到的powerstate是："); Serial.println(powerstate);
}

void brightness(JsonVariant L)//固定格式，修改参数l
{
  liangdu = L["brightness"];//参数l
  analogWrite(pinRelay,liangdu*10+24); 
  Serial.printf("收到的亮度是："); Serial.println(liangdu);
}


void setup() {
  // 将您的设置代码放在这里，一次运行：
  Serial.begin(9600);
  Serial.println("开机..");

  //WiFiManager 
  //本地初始化。一旦完成业务，就无需保留它 
  WiFiManager wifiManager;

  //退出配置而不是连接 
  wifiManager.setBreakAfterConfig(true);

  //重置设置-用于测试
  //wifiManager.resetSettings();


  //尝试连接到最新的已知设置
  //如果未连接，则以指定的名称启动访问点
  //此处为“ AutoConnectAP”，密码为“ password”
  //进入等待配置的阻塞循环 
  if (!wifiManager.autoConnect("TieNiu_ESP_NODEMCU", "zysniu")) {
    Serial.println("连接失败，我们应该重置以查看是否连接");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("连接成功 :)");
  Serial.print("SSID:");
  Serial.println(WiFi.SSID());
  Serial.print("IP:");
  Serial.println(WiFi.localIP());

  server.on("/", homepage);
  server.begin();//启动服务
  Serial.println("HTTP server 启动成功!");

  pinMode(pinRelay, OUTPUT); 
  //初始化 iot，需传入 wifi 的 client，和设备产品信息
  AliyunIoTSDK::begin(espClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);

  //绑定一个设备属性回调，当远程修改此属性，会触发powerstate函数;天猫精灵控制的就是powerstate的高低电平（高 = 1，低 = 0）
  AliyunIoTSDK::bindData("powerstate", powerstate);
  AliyunIoTSDK::bindData("brightness", brightness);

  
  //绑定一个设备属性回调，当远程修改此属性，会触发allPowerstate函数
  //AliyunIoTSDK::bindData("allPowerstate", allPowerstate);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();//监听客户请求并处理
  AliyunIoTSDK::loop(); //必要函数
  
    //发送数据的间隔，2秒发送一次，“云智能APP”上电源按钮的UI实时显示会有2秒延时。
    if (millis() - lastMsMain >= 2000)   
    {
        lastMsMain = millis();       
        //发送电源状态到云平台（高电平：1；低电平：0）
        AliyunIoTSDK::send("powerstate", kaiguan); 
        AliyunIoTSDK::send("brightness", liangdu);//亮度
        
        //AliyunIoTSDK::send("allPowerstate", digitalRead(pinRelay));       
    }
  delay(100);
}

