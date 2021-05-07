void setup() {
  // put your setup code here, to run once:
  //初始化
   Serial.begin(115200);

}
//A0-->AO
void loop() {
  // put your main code here, to run repeatedly:
   //读取A0口的值
   int sensorValue = analogRead(A0);
   
   //输出  "lux"
  Serial.println(sensorValue );
   delay(1000);
}
