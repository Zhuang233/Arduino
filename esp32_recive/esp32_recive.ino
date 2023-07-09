/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-many-to-one-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <esp_now.h>
#include <WiFi.h>
#include <HardwareSerial.h>

#define BLINKER_WIFI
#define LED_BUILTIN 22
#include <Blinker.h>


HardwareSerial SerialPort(2); // use UART2

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  // int id;
  // int x;
  // int y;
  float angleX;
  float angleY;
  float angleZ;
}struct_message;

// 帧头+角度数据
int head1 = 0x53;
int head2 = 0xfd;
int angleX_int = 0;
int angleY_int = 0;
int angleZ_int = 0;

// 局域网信息
char auth[] = "c2f6b82b6eb6";
char ssid[] = "abcd";
char pswd[] = "155155155";

// 串口缓冲区
char buffer[10];

char buf_tem[6]={0,0,0,0,0,0};
char buf_hum[6]={0,0,0,0,0,0};

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
// struct_message board1;
// struct_message board2;
// struct_message board3;

// Create an array with all the structures
// struct_message boardsStruct[3] = {board1, board2, board3};

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  // char macStr[18];
  // Serial.print("Packet received from: ");
  // snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
  //          mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  // Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  // boardsStruct[myData.id-1].x = myData.x;
  // boardsStruct[myData.id-1].y = myData.y;
  // Serial.printf("x value: %d \n", boardsStruct[myData.id-1].x);
  // Serial.printf("y value: %d \n", boardsStruct[myData.id-1].y);
  // Serial.println();

  // 测试传输
  // Serial.printf("X %3f\n",myData.angleX);
  // Serial.printf("Y %3f\n",myData.angleY);
  // Serial.printf("Z %3f\n",myData.angleZ);

  // Serial2.write("x%3fy%3z%3f",myData.angleX,myData.angleY,myData.angleZ);
  angleX_int = myData.angleX;
  angleY_int = myData.angleY;
  angleZ_int = myData.angleZ;
  // Serial.printf("X %3f\n",myData.angleX);
  // Serial.printf("X2 %d\n",angleX_int);


  // Serial2.write(head1);
  // Serial2.write(head2);
  Serial2.write(angleX_int);
  Serial2.write(angleY_int);
  Serial2.write(angleZ_int);
  // Serial2.write("zbw");

}
 
void setup() {
  //Initialize Serial Monitor
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);
}
 
void loop() {
  // Acess the variables for each board
  /*int board1X = boardsStruct[0].x;
  int board1Y = boardsStruct[0].y;
  int board2X = boardsStruct[1].x;
  int board2Y = boardsStruct[1].y;
  int board3X = boardsStruct[2].x;
  int board3Y = boardsStruct[2].y;*/
  Blinker.run();
  Serial2.readBytes(buffer, 10);
  for(int i=0;i<5;i++){
    buf_tem[i] = buffer[i];
    buf_hum[i] = buffer[i+5];
  }
  // // Serial.print(buffer);
  Blinker.print("temperature", buf_tem);
  Blinker.print("humidity", buf_hum);
  delay(100);  
}