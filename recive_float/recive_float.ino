#include <SoftwareSerial.h> //软串口库

// 串口引脚定义
#define RX 2
#define TX 3

SoftwareSerial mySerial(RX, TX);

union Data{
  uint8_t bytes[4];
  float f;
};

Data recive_data;
uint8_t buf[4];

void setup() {
  Serial.begin(9600);
  mySerial.begin(115200);
  mySerial.listen();
}

void loop() {
  if(mySerial.available()){

    mySerial.readBytes(buf,4);
    // 填充
    for(int i = 0;i < 4; i++){
      recive_data.bytes[i] = buf[i];
      Serial.println(buf[i],HEX);
    }
    Serial.println(recive_data.f);
    Serial.println("------------");
  }
}
