#include <esp_now.h>
#include <WiFi.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

// mpu6050 引脚定义
#define MPU6050_SDA 12
#define MPU6050_SCL 14

MPU6050 mpu6050(Wire);
long timer = 0;
// 发送频率 Hz
#define FRQ 10

// 接收方mac地址
uint8_t broadcastAddress[] = {0xD4, 0xD4, 0xDA, 0xCF, 0xAF, 0x30};

// 帧结构体
// 需与接收方的匹配
typedef struct struct_message {
    // int id; // must be unique for each sender board
    // int x;
    // int y;
    float angleX;
    float angleY;
    float angleZ;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create peer interface
esp_now_peer_info_t peerInfo;

// 帧发送回调函数
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.printf("X %3f\n",myData.angleX);
  Serial.printf("Y %3f\n",myData.angleY);
  Serial.printf("Z %3f\n",myData.angleZ);
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // 设置串口波特率
  Serial.begin(9600);

  // 设置为wifi站点
  WiFi.mode(WIFI_STA);

  // 初始化 esp-now
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // 设置引脚
  Wire.setPins(MPU6050_SDA,MPU6050_SCL);
  // 引脚定义初始化
  Wire.begin();
  // mpu6050初始化
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  // 注册帧发送回调函数（帧发送之后）
  esp_now_register_send_cb(OnDataSent);
  
  // 配对
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // 更新mpu6050数据
  mpu6050.update();
  
  if(millis() - timer > 1000/FRQ){
    // 发送的数据
    myData.angleX = mpu6050.getAngleX();
    myData.angleY = mpu6050.getAngleY();
    myData.angleZ = mpu6050.getAngleZ();
    // myData.id = 1;
    // myData.x = random(0,50);
    // myData.y = random(0,50);

    // 发送数据
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    // 调试用
    // if (result == ESP_OK) {
    //   Serial.println("Sent with success");
    // }
    // else {
    //   Serial.println("Error sending the data");
    // }

    timer = millis();
  }
}