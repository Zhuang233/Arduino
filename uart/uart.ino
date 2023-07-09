#include <SoftwareSerial.h> //软串口库

// 串口引脚定义
#define RX 2
#define TX 3

SoftwareSerial mySerial(RX, TX);

char buf[3] = {0x55,0x66,0x77};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  mySerial.listen();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(mySerial.available()){
    // Serial.print(mySerial.read());
    // Serial.write(buf,3);
    Serial.println("a");
    mySerial.read(buf,3);
  }
}
