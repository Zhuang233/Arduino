#include <DHT.h>
#include <DHT_U.h>
#include <LCD_I2C.h>
#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include "SR04.h"


#define DHTPIN 4     // 温湿度传感器数据引脚
#define DHTTYPE DHT11   // DHT 11

// 左右轮
#define LEFT 0
#define RIGHT 1

// L298N 接线
#define RIGHT_P 5 // IN1
#define RIGHT_N 6 // IN2
#define LEFT_P 9 // IN3
#define LEFT_N 10 // IN4

// 正反转
#define POSITIVE 1
#define NEGATIVE 0

// 串口
#define RX 0
#define TX 1

// RGB
#define R 1
#define G 2
#define B 3

// 超声波
#define TRIG_PIN 2
#define ECHO_PIN 3

// 蜂鸣器
#define BI_PIN 7

typedef struct angles{
  int x;
  int y;
  int z;
}angles_t;

angles_t angles;
char buffer[3];

DHT dht(DHTPIN, DHTTYPE);
LCD_I2C lcd(0x27, 16, 2);
SoftwareSerial mySerial(RX, TX);
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long distance;
int last_pt = 0;
bool bi = false;

// 温湿度字符串转换缓冲区
char buffer_tem[6];
char buffer_hum[6];

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  // mySerial.setTimeout(1000);

  // 右电机
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  // 左电机
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);

  // 蜂鸣器
  pinMode(BI_PIN,OUTPUT);

  // RGB
  // pinMode(2,OUTPUT);
  // pinMode(3,OUTPUT);
  // pinMode(4,OUTPUT);
  
  // 温湿度传感器
  dht.begin();
  // 显示模块
  lcd.begin();
  lcd.backlight();// 背光

  // 定时器（蜂鸣器）
  MsTimer2::set(2,onTimer);
  MsTimer2::start();
}

int head = 0;

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("hello world");
  // delay(200);
  // 电机1
  // digitalWrite(4)
  // for(int i=0;i<255;i++){
  //   analogWrite(5,i);	
  //   delay(2000/255);
  // }
  // for(int i=255;i>0;i--){
  //   analogWrite(5,i);
  //   delay(2000/255);
  // }
  // delay(200);

  //测试前进后退
  // forward(255);
  // delay(1000);
  // back(255);
  // delay(1000);

  // 温湿度模块测试
  // Wait a few seconds between measurements.
  // delay(2000);
  // 读取湿度
  float h = dht.readHumidity();
  // 读取温度
  float t = dht.readTemperature();
  // dtostrf(t,2,2,buffer_tem);
  // dtostrf(h,2,2,buffer_hum);
  // Serial.print(buffer_tem);
  // Serial.print(buffer_hum);
  // mySerial.write(buffer_tem);
  // mySerial.write(buffer_hum);
  // Serial.print(F("Humidity: "));
  // Serial.print(h);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(t);
  // Serial.print(F("°C "));
  // Serial.print("\n");

  // lcd模块测试
  lcd.setCursor(0,0);
  lcd.print("TEM:"); // You can make spaces using well... spaces
  lcd.print(t);
  lcd.setCursor(0,1);
  // lcd.setCursor(5, 1); // Or setting the cursor in the desired position.
  lcd.print("HUM:");
  lcd.print(h);
  // Serial.println(h);
  // Serial.println(t);
  // delay(500);
  // lcd.clear();
  // delay(500);

 // 测试与esp32串口传输
  // Serial.readBytes(buffer, 3);
  // angles.x = buffer[0];
  // angles.y = buffer[1];
  // angles.z = buffer[2];

  // Serial.print("\n");
  // Serial.println(angles.x);
  // Serial.println(angles.y);
  // Serial.println(angles.z);

  // 测试RGB
  // RGB(R);
  // RGB(R);
  // delay(1000);
  // RGB(B);
  // delay(1000);
  // RGB(G);
  // delay(1000);

  // 手势检测测试
  mySerial.readBytes(buffer, 3);
  angles.x = buffer[0];
  angles.y = buffer[1];
  angles.z = buffer[2];
  // Serial.print("\n");
  // Serial.println(angles.x);
  // Serial.println(angles.y);
  // Serial.println(angles.z);

  // 超声波
  distance = sr04.Distance();
  // Serial.print(distance);
  // Serial.println("cm");

  // 避障
  if(distance < 15){
    turn_right();
    bi = true;
  }
  // 手控
  else{
    bi = false;
    if(angles.x > 0){
      // 后退
      roll(LEFT,NEGATIVE,angles.x*2+50+angles.y);
      roll(RIGHT,NEGATIVE,angles.x*2+50-angles.y);
    }
    else{
      // 前进
      roll(LEFT,POSITIVE,-angles.x*2+50+angles.y);
      roll(RIGHT,POSITIVE,-angles.x*2+50-angles.y);
    }
  }


  

}

void turn_right(){
  roll(LEFT,POSITIVE,150);
  roll(RIGHT,NEGATIVE,150);
  delay(100);
}

void RGB(int mode){
  if(mode == R){
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,HIGH);
  }else if(mode == G){
    digitalWrite(2,HIGH);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
  }
  else if(mode == B){
    digitalWrite(2,LOW);
    digitalWrite(3,HIGH);
    digitalWrite(4,LOW);
  }
}

// 前进
// 参数：速度0-255
void forward(int speed){
  roll(LEFT,POSITIVE,speed);
  roll(RIGHT,POSITIVE,speed);
}

// 后退
// 参数：速度0-255
void back(int speed){
  roll(LEFT,NEGATIVE,speed);
  roll(RIGHT,NEGATIVE,speed);
}

// 参数：左右轮，正反转，速度0-255
void roll(bool side,bool mode,int speed){
  // 左电机
  if(side == LEFT){
    // 正转
    if(mode == POSITIVE){
      digitalWrite(LEFT_N,LOW);
      analogWrite(LEFT_P,speed);
    }
    // 反转
    else{
      digitalWrite(LEFT_P,LOW);
      analogWrite(LEFT_N,speed);
    }
  }
  // 右电机
  else{
    // 正转
    if(mode == POSITIVE){
      digitalWrite(RIGHT_N,LOW);
      analogWrite(RIGHT_P,speed);
    }
    // 反转
    else{
      digitalWrite(RIGHT_P,LOW);
      analogWrite(RIGHT_N,speed);
    }
  }
}

void onTimer(){
  if(bi){
    if(last_pt == 1){
      digitalWrite(BI_PIN,LOW);
      last_pt = 0;
    }
    else{
      digitalWrite(BI_PIN,HIGH);
      last_pt = 1;
    }
  }
}

