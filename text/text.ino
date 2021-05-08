#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <aliyun_mqtt.h>

//定义LED灯
#define LED0 D0 
//人体红外
#define SENSOR_PIN  D7

//初始化次态
int redLedState0 = 0;
//初始化现态
int last0 = 0;
//上云时间现态
unsigned long lastSend = 0;

#define WIFI_SSID        "HONOR20"//替换自己的WIFI
#define WIFI_PASSWD      "2222222222"//替换自己的WIFI
#define PRODUCT_KEY      "a1XzU15ltF6"//替换自己的PRODUCT_KEY
#define DEVICE_NAME      "Unit01" //替换自己的DEVICE_NAME
#define DEVICE_SECRET    "2fa495e383239022632d1b5c2451296b"//替换自己的DEVICE_SECRET

//订阅和发布时所需的主题
#define ALINK_BODY_FORMAT "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
#define ALINK_TOPIC_PROP_POSTRSP "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post_reply"
#define ALINK_TOPIC_PROP_SET "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
#define ALINK_METHOD_PROP_POST "thing.event.property.post"

//创建WiFiClient实例
WiFiClient espClient;
//创建MqttClient实例
PubSubClient mqttClient(espClient);

//连接Wifi
void initWifi(const char *ssid, const char *password)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi does not connect, try again ...");
    delay(3000);
  }

  Serial.println("Wifi is connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//监听云端下发指令并处理 
void callback(char *topic, byte *payload, unsigned int length)
{
     
    Serial.println();
    Serial.println();
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    Serial.println();
    //云端下发属性设置topic = "[/sys/a10PnXZBgsl/7L9EUbLv113EkYkF7fpD/thing/service/property/set]"

    payload[length] = '\0';
    Serial.println((char *)payload);
    //const char *payload = "{"method":"thing.service.property.set","id":"282860794","params":{"LightSwitch":1},"version":"1.0.0"}"


  if (strstr(topic, ALINK_TOPIC_PROP_SET))
    {
        //json解析payload
        StaticJsonBuffer<400> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(payload);

        //判断json解析是否成功
        if (!root.success())
        {   
            Serial.println("parseObject() failed");
        } 
        else
        {
            Serial.println("parseObject() success");

            //redLedState0  = json解析后的"S_D0"的值
            //redLedState0 = root["params"]["S_D0"];
            redLedState0 = root["params"]["LightSwitch"];

            Serial.print(redLedState0);
            Serial.println(" set  sucess");
            Serial.println();
     
            Serial.print("LED0 State: ");
            Serial.println(redLedState0);
            digitalWrite(LED0, redLedState0);

        }
    }
}

//连接Mqtt订阅属性设置Topic
void mqttCheckConnect()
{
    bool connected = connectAliyunMQTT(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET);

    if (connected)
    {
        Serial.println("MQTT connect succeed!");

        //订阅属性设置Topic
        mqttClient.subscribe(ALINK_TOPIC_PROP_SET); 
        Serial.println("subscribe done");
    }
}

// 上报属性Topic数据
void mqttIntervalPost()
{
    char param[256];
    char jsonBuf[512];
   
    sprintf(param, "{\"S_D0\":%d}", digitalRead(LED0));
    sprintf(param, "{\"idle\":%d}", digitalRead(SENSOR_PIN));

    sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    //jsonBuf = "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":"\{\"S_D0\":%d}\"}"
    Serial.println(jsonBuf);
    mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf); 
}

void setup() 
{
    Serial.begin(115200);

    
    pinMode(SENSOR_PIN,  INPUT);//人体红外
    /* initialize serial for debugging */

    pinMode(LED0, OUTPUT); 
    digitalWrite(LED0, 0); 

    initWifi(WIFI_SSID, WIFI_PASSWD); 

    Serial.println();
    Serial.println();

    mqttCheckConnect(); //初始化首次链接
    mqttIntervalPost(); //上报初始化数据

    Serial.println();
    mqttClient.setCallback(callback); // 回调，监听云端下发指令，当ESP8266收到订阅Topic后调用callback函数
}


void loop()
{
  //每隔1s尝试连接一次云
  if (millis() - lastSend >= 10000)
    {
        mqttCheckConnect(); 
        lastSend = millis();
    }
    //判断三灯现态与次态的关系，减少无意义的上报  
    if (last0 != redLedState0)
    {
        last0 = redLedState0;
        mqttIntervalPost(); 
        Serial.println();
    }
    mqttClient.loop();
    
    //人体红外
     if (digitalRead(SENSOR_PIN) == HIGH){
    Serial.println("Motion detected!");
    delay(2000);
      }
    else {
    Serial.println("Motion absent!");
    delay(2000);
    }
}
