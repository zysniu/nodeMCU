# ESP8266 接入 阿里物联网 天猫精灵

**控制小灯开关，亮度，定时等等**


###IDE 使用 vscode PlatformIO  框架 arduino 硬件支持 esp01 esp01s nodemcu 等

 
引用的库
>https://github.com/tzapu/WiFiManager.git
>https://github.com/yu-tou/arduino-aliyun-iot-sdk.git


####esp01 上可以使用 3引脚 也就是RX引脚 替换以下代码即可

	//设置产品和设备的信息，从阿里云设备信息里查看
	#define PRODUCT_KEY "a1mILnIgKB1"  //替换自己的PRODUCT_KEY
	#define DEVICE_NAME "led01"  //替换自己的DEVICE_NAME
	#define DEVICE_SECRET "ffab820469474f574cb466216a88fac6"  //替换自己的DEVICE_SECRET
	#define REGION_ID "cn-shanghai"  //默认cn-shanghai

	unsigned long lastMsMain = 0;
	int pinRelay = 2; //esp01 上可以使用 3引脚 也就是RX引脚

####QQ:7031996