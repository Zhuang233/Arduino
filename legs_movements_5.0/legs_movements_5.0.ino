/*
此程序可以用软串口控制幻尔的串口舵机
*/

#include <SoftwareSerial.h>
SoftwareSerial Bluetooth05(2,3); //ibus 和电机的软串口115200
//新建一个名字叫serial4的软串口，RX：2   TX：3

#define GET_LOW_BYTE(A) (uint8_t)((A))
//宏函数 获得A的低八位
#define GET_HIGH_BYTE(A) (uint8_t)((A) >> 8)
//宏函数 获得A的高八位
#define BYTE_TO_HW(A, B) ((((uint16_t)(A)) << 8) | (uint8_t)(B))
//宏函数 以A为高八位 B为低八位 合并为16位整形

#define LOBOT_SERVO_FRAME_HEADER         0x55
#define LOBOT_SERVO_MOVE_TIME_WRITE      1
#define LOBOT_SERVO_MOVE_TIME_READ       2
#define LOBOT_SERVO_MOVE_TIME_WAIT_WRITE 7
#define LOBOT_SERVO_MOVE_TIME_WAIT_READ  8
#define LOBOT_SERVO_MOVE_START           11
#define LOBOT_SERVO_MOVE_STOP            12
#define LOBOT_SERVO_ID_WRITE             13
#define LOBOT_SERVO_ID_READ              14
#define LOBOT_SERVO_ANGLE_OFFSET_ADJUST  17
#define LOBOT_SERVO_ANGLE_OFFSET_WRITE   18
#define LOBOT_SERVO_ANGLE_OFFSET_READ    19
#define LOBOT_SERVO_ANGLE_LIMIT_WRITE    20
#define LOBOT_SERVO_ANGLE_LIMIT_READ     21
#define LOBOT_SERVO_VIN_LIMIT_WRITE      22
#define LOBOT_SERVO_VIN_LIMIT_READ       23
#define LOBOT_SERVO_TEMP_MAX_LIMIT_WRITE 24
#define LOBOT_SERVO_TEMP_MAX_LIMIT_READ  25
#define LOBOT_SERVO_TEMP_READ            26
#define LOBOT_SERVO_VIN_READ             27
#define LOBOT_SERVO_POS_READ             28
#define LOBOT_SERVO_OR_MOTOR_MODE_WRITE  29
#define LOBOT_SERVO_OR_MOTOR_MODE_READ   30
#define LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE 31
#define LOBOT_SERVO_LOAD_OR_UNLOAD_READ  32
#define LOBOT_SERVO_LED_CTRL_WRITE       33
#define LOBOT_SERVO_LED_CTRL_READ        34
#define LOBOT_SERVO_LED_ERROR_WRITE      35
#define LOBOT_SERVO_LED_ERROR_READ       36

#define LOBOT_DEBUG 1  /*调试是用，打印调试数据*/

byte LobotCheckSum(byte buf[])
{
  byte i;
  uint16_t temp = 0;
  for (i = 2; i < buf[3] + 2; i++) {
    temp += buf[i];
  }
  temp = ~temp;
  i = (byte)temp;
  return i;
}

void LobotSerialServoMove(SoftwareSerial &SerialX, uint8_t id, int16_t position, uint16_t time)
{
  byte buf[10];
  if(position < 0)
    position = 0;
  if(position > 1000)
    position = 1000;
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 7;
  buf[4] = LOBOT_SERVO_MOVE_TIME_WRITE;
  buf[5] = GET_LOW_BYTE(position);
  buf[6] = GET_HIGH_BYTE(position);
  buf[7] = GET_LOW_BYTE(time);
  buf[8] = GET_HIGH_BYTE(time);
  buf[9] = LobotCheckSum(buf);
  SerialX.write(buf, 10);
}

void LobotSerialServoStopMove(SoftwareSerial &SerialX, uint8_t id)
{
  byte buf[6];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 3;
  buf[4] = LOBOT_SERVO_MOVE_STOP;
  buf[5] = LobotCheckSum(buf);
  SerialX.write(buf, 6);
}

void LobotSerialServoSetID(SoftwareSerial &SerialX, uint8_t oldID, uint8_t newID)
{
  byte buf[7];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = oldID;
  buf[3] = 4;
  buf[4] = LOBOT_SERVO_ID_WRITE;
  buf[5] = newID;
  buf[6] = LobotCheckSum(buf);
  SerialX.write(buf, 7);
  
#ifdef LOBOT_DEBUG
  Serial.println("LOBOT SERVO ID WRITE");
  int debug_value_i = 0;
  for (debug_value_i = 0; debug_value_i < buf[3] + 3; debug_value_i++)
  {
    Serial.print(buf[debug_value_i], HEX);
    Serial.print(":");
  }
  Serial.println(" ");
#endif

}

void LobotSerialServoSetMode(SoftwareSerial &SerialX, uint8_t id, uint8_t Mode, int16_t Speed)
{
  byte buf[10];

  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 7;
  buf[4] = LOBOT_SERVO_OR_MOTOR_MODE_WRITE;
  buf[5] = Mode;
  buf[6] = 0;
  buf[7] = GET_LOW_BYTE((uint16_t)Speed);
  buf[8] = GET_HIGH_BYTE((uint16_t)Speed);
  buf[9] = LobotCheckSum(buf);

#ifdef LOBOT_DEBUG
  Serial.println("LOBOT SERVO Set Mode");
  int debug_value_i = 0;
  for (debug_value_i = 0; debug_value_i < buf[3] + 3; debug_value_i++)
  {
    Serial.print(buf[debug_value_i], HEX);
    Serial.print(":");
  }
  Serial.println(" ");
#endif

  SerialX.write(buf, 10);
}
void LobotSerialServoLoad(SoftwareSerial &SerialX, uint8_t id)
{
  byte buf[7];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 4;
  buf[4] = LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE;
  buf[5] = 1;
  buf[6] = LobotCheckSum(buf);
  
  SerialX.write(buf, 7);
  
#ifdef LOBOT_DEBUG
  Serial.println("LOBOT SERVO LOAD WRITE");
  int debug_value_i = 0;
  for (debug_value_i = 0; debug_value_i < buf[3] + 3; debug_value_i++)
  {
    Serial.print(buf[debug_value_i], HEX);
    Serial.print(":");
  }
  Serial.println(" ");
#endif

}

void LobotSerialServoUnload(SoftwareSerial &SerialX, uint8_t id)
{
  byte buf[7];
  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 4;
  buf[4] = LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE;
  buf[5] = 0;
  buf[6] = LobotCheckSum(buf);
  
  SerialX.write(buf, 7);
  
#ifdef LOBOT_DEBUG
  Serial.println("LOBOT SERVO LOAD WRITE");
  int debug_value_i = 0;
  for (debug_value_i = 0; debug_value_i < buf[3] + 3; debug_value_i++)
  {
    Serial.print(buf[debug_value_i], HEX);
    Serial.print(":");
  }
  Serial.println(" ");
#endif
}


int LobotSerialServoReceiveHandle(SoftwareSerial &SerialX, byte *ret)
{
  bool frameStarted = false;
  bool receiveFinished = false;
  byte frameCount = 0;
  byte dataCount = 0;
  byte dataLength = 2;
  byte rxBuf;
  byte recvBuf[32];
  byte i;

  while (SerialX.available()) {
    rxBuf = SerialX.read();
    delayMicroseconds(100);
    if (!frameStarted) {
      if (rxBuf == LOBOT_SERVO_FRAME_HEADER) {
        frameCount++;
        if (frameCount == 2) {
          frameCount = 0;
          frameStarted = true;
          dataCount = 1;
        }
      }
      else {
        frameStarted = false;
        dataCount = 0;
        frameCount = 0;
      }
    }
    if (frameStarted) {
      recvBuf[dataCount] = (uint8_t)rxBuf;
      if (dataCount == 3) {
        dataLength = recvBuf[dataCount];
        if (dataLength < 3 || dataCount > 7) {
          dataLength = 2;
          frameStarted = false;
        }
      }
      dataCount++;
      if (dataCount == dataLength + 3) {
        
#ifdef LOBOT_DEBUG
        Serial.print("RECEIVE DATA:");
        for (i = 0; i < dataCount; i++) {
          Serial.print(recvBuf[i], HEX);
          Serial.print(":");
        }
        Serial.println(" ");
#endif

        if (LobotCheckSum(recvBuf) == recvBuf[dataCount - 1]) {
          
#ifdef LOBOT_DEBUG
          Serial.println("Check SUM OK!!");
          Serial.println("");
#endif

          frameStarted = false;
          memcpy(ret, recvBuf + 4, dataLength);
          return 1;
        }
        return -1;
      }
    }
  }
}


int LobotSerialServoReadPosition(SoftwareSerial &SerialX, uint8_t id)
{
  int count = 10000;
  int ret;
  byte buf[6];

  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 3;
  buf[4] = LOBOT_SERVO_POS_READ;
  buf[5] = LobotCheckSum(buf);

#ifdef LOBOT_DEBUG
  Serial.println("LOBOT SERVO Pos READ");
  int debug_value_i = 0;
  for (debug_value_i = 0; debug_value_i < buf[3] + 3; debug_value_i++)
  {
    Serial.print(buf[debug_value_i], HEX);
    Serial.print(":");
  }
  Serial.println(" ");
#endif

  while (SerialX.available())
    SerialX.read();

  SerialX.write(buf, 6);

  while (!SerialX.available()) {
    count -= 1;
    if (count < 0)
      return -2048;
  }

  if (LobotSerialServoReceiveHandle(SerialX, buf) > 0)
    ret = (int16_t)BYTE_TO_HW(buf[2], buf[1]);
  else
    ret = -2048;

#ifdef LOBOT_DEBUG
  Serial.println(ret);
#endif
  return ret;
}
int LobotSerialServoReadVin(SoftwareSerial &SerialX, uint8_t id)
{
  int count = 10000;
  int ret;
  byte buf[6];

  buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
  buf[2] = id;
  buf[3] = 3;
  buf[4] = LOBOT_SERVO_VIN_READ;
  buf[5] = LobotCheckSum(buf);

#ifdef LOBOT_DEBUG
  Serial.println("LOBOT SERVO VIN READ");
  int debug_value_i = 0;
  for (debug_value_i = 0; debug_value_i < buf[3] + 3; debug_value_i++)
  {
    Serial.print(buf[debug_value_i], HEX);
    Serial.print(":");
  }
  Serial.println(" ");
#endif

  while (SerialX.available())
    SerialX.read();

  SerialX.write(buf, 6);

  while (!SerialX.available()) {
    count -= 1;
    if (count < 0)
      return -2048;
  }

  if (LobotSerialServoReceiveHandle(SerialX, buf) > 0)
    ret = (int16_t)BYTE_TO_HW(buf[2], buf[1]);
  else
    ret = -2049;

#ifdef LOBOT_DEBUG
  Serial.println(ret);
#endif
  return ret;
}






//________________________________________华丽的分割线____________________________________________


#include "leogetangle.h" // 自己写的逆解算



#include <SoftwareSerial.h> //前面也有一个
#include <IBusBM.h>
IBusBM ibus;

#define ID1   1
#define ID2   2
#define ID3   3
#define ID4   4
#define ID5   5
#define ID6   6
#define ID7   7
#define ID8   8
#define ID9   9
#define ID10   10
#define ID11   11
#define ID12   12


uint8_t buf[4] ;


union Data{
  uint8_t bytes[4];
  float f;
};

Data recieve_data ;


//ibus initialize
// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
  uint16_t ch = ibus.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Read the channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}



int lengths[] = {110, 100};      //{大臂长度，小臂长度}

int y_offset = 0 ; //测试地盘高度
int x_offset = 0 ;  //测试地盘高度 


void move_servo(int servo_1, int servo_2, int x, int y, int Time){ //servo_1 is the showder servo ID; servo_2 is the elbow servo ID
  //自己写的逆解算
  //解算程序xy的datatype是double，这里面放的是int. !
  double shouderAngle ;
  double elbowAngle ;
  leogetangle(x + x_offset,y + y_offset,&shouderAngle,&elbowAngle) ; //改高度

  //
  LobotSerialServoMove(Bluetooth05, servo_1, map(shouderAngle,60,300,0,1000), Time);
  LobotSerialServoMove(Bluetooth05, servo_2, map(elbowAngle,60,300,0,1000), Time);
}






//double_leg
void Double_leg_control() {
  //点位宏定义 ; 这个点位可能需要做实验
  #define Y1 175  //170 如果在压低地盘
  #define Y2 180
  #define X1 -10
  #define X2 0
  #define X3 10


  int Time = 100 ; //unit sm 
  int original_stand[2] = {50,180} ; // x,y pos for every leg 
  int ready_stand[4][2] = {{X1,Y2}, {X3,Y2}, {X3,Y2}, {X1,Y2}} ; //pos for four legs counter clockwise (1,4,7,10)脚落地 后脚镜像反转

 

  //步态坐标点位            {{X1,Y2}, {X3,Y2}, {X3,Y2}, {X1,Y2}}
  int pos[4][4][2]     = {{{X2,Y2}, {X2,Y1}, {X2,Y2}, {X2,Y1}}, 
                          {{X3,Y2}, {X1,Y1}, {X1,Y2}, {X3,Y1}}, 
                          //{{X3,Y2}, {X1,Y2}, {X1,Y2}, {X3,Y2}},
                          {{X2,Y1}, {X2,Y2}, {X2,Y1}, {X2,Y2}},
                          {{X1,Y1}, {X3,Y2}, {X3,Y1}, {X1,Y2}},
                          //{{X1,Y2}, {X3,Y2}, {X3,Y2}, {X1,Y2}}
                          //已经从一个方向的梯形变成另一个方向的梯

  } ;
  /*先让他站起来
  move_servo(ID2,ID3,ready_stand[0][0],ready_stand[0][1],Time) ;
  move_servo(ID5,ID6,ready_stand[1][0],ready_stand[1][1],Time) ;
  move_servo(ID8,ID9,ready_stand[2][0],ready_stand[2][1],Time) ;
  move_servo(ID11,ID12,ready_stand[3][0],ready_stand[3][1],Time) ;
  
delay(500) ;
*/
//test pos 数组

int side_adjust = 0 ; //侧面偏置

for (int j = 0; j < 10; j++) {
  for (int i = 0; i < 4; i++){
    move_servo(ID2,ID3,pos[i][0][0],pos[i][0][1] + side_adjust,Time) ; //侧面偏置
    move_servo(ID5,ID6,pos[i][1][0],pos[i][1][1],Time) ;
    move_servo(ID8,ID9,pos[i][2][0],pos[i][2][1],Time) ;
    move_servo(ID11,ID12,pos[i][3][0],pos[i][3][1],Time) ; 
    delay(Time) ;
  }
 
}


  /*循环行走
  while(readChannel(1,-100,100,0) >= 30){ //如果信号还在，就循环执行前进步伐

  }
*/    
    //delay(500) ;

}




//double_leg_slope 向上爬坡
void Double_leg_slope_control() {
  //点位宏定义 ; 这个点位可能需要做实验
  #define Y1 165  //170 如果在压低地盘
  #define Y2 180
  #define X1 -10
  #define X2 0
  #define X3 10

  int slope_adjust = -30 ;
  int Time = 100 ; //unit sm 
  int original_stand[2] = {50,180} ; // x,y pos for every leg 
  int ready_stand[4][2] = {{X1,Y2}, {X3,Y2}, {X3,Y2}, {X1,Y2}} ; //pos for four legs counter clockwise (1,4,7,10)脚落地 后脚镜像反转

 

  //步态坐标点位            {{X1,Y2}, {X3,Y2}, {X3,Y2}, {X1,Y2}}
  int pos[4][4][2]     = {{{X2,Y2}, {X2,Y1}, {X2,Y2}, {X2,Y1}}, 
                          {{X3,Y2}, {X1,Y1}, {X1,Y2}, {X3,Y1}}, 
                          //{{X3,Y2}, {X1,Y2}, {X1,Y2}, {X3,Y2}},
                          {{X2,Y1}, {X2,Y2}, {X2,Y1}, {X2,Y2}},
                          {{X1,Y1}, {X3,Y2}, {X3,Y1}, {X1,Y2}},
                          //{{X1,Y2}, {X3,Y2}, {X3,Y2}, {X1,Y2}}
                          //已经从一个方向的梯形变成另一个方向的梯

  } ;
  /*先让他站起来
  move_servo(ID2,ID3,ready_stand[0][0],ready_stand[0][1],Time) ;
  move_servo(ID5,ID6,ready_stand[1][0],ready_stand[1][1],Time) ;
  move_servo(ID8,ID9,ready_stand[2][0],ready_stand[2][1],Time) ;
  move_servo(ID11,ID12,ready_stand[3][0],ready_stand[3][1],Time) ;
  
delay(500) ;
*/
//test pos 数组


for (int j = 0; j < 10; j++) {
  for (int i = 0; i < 4; i++){
    move_servo(ID2,ID3,pos[i][0][0],pos[i][0][1] + slope_adjust,Time) ; //加了偏置
    move_servo(ID5,ID6,pos[i][1][0],pos[i][1][1] + slope_adjust,Time) ; //加了偏置
    move_servo(ID8,ID9,pos[i][2][0],pos[i][2][1],Time) ;
    move_servo(ID11,ID12,pos[i][3][0],pos[i][3][1],Time) ;
    delay(Time) ;
  }
 
}


  /*循环行走
  while(readChannel(1,-100,100,0) >= 30){ //如果信号还在，就循环执行前进步伐

  }
*/    
    //delay(500) ;

}


void Single_leg_control(){
  //宏定义点
  #define Y1 170
  #define Y2 180
  #define X1 -10
  #define X2 0
  #define X3 10
  #define X4 20

  //initialise 14710 servos' position
  LobotSerialServoMove(Bluetooth05, ID1, 312, 500);
  LobotSerialServoMove(Bluetooth05, ID7, 688, 500) ;

  LobotSerialServoMove(Bluetooth05, ID4, 312, 500) ;
  LobotSerialServoMove(Bluetooth05, ID10, 688, 500) ;
  //


  int Time = 80 ;
  int original_stand[4][2] = {{X2,Y2}, {X4,Y2}, {X4,Y2}, {X2,Y2}} ;// initialise position 


  int pos [8][4][2] = {
                        {{X2,Y2}, {X2,Y1}, {X4,Y2}, {X2,Y2}},
                        {{X3,Y2}, {X1,Y2}, {X3,Y2}, {X1,Y2}}, 
                        {{X3,Y2}, {X1,Y2}, {X3,Y2}, {X3,Y1}},
                        {{X4,Y2}, {X2,Y2}, {X2,Y2}, {X4,Y2}},
                        {{X2,Y1}, {X2,Y2}, {X2,Y2}, {X4,Y2}},
                        {{X1,Y2}, {X3,Y2}, {X1,Y2}, {X3,Y2}},
                        {{X1,Y2}, {X3,Y2}, {X3,Y1}, {X3,Y2}},
                        {{X2,Y2}, {X4,Y2}, {X4,Y2}, {X2,Y2}}
  } ;
  //move servos according to the positions above
  while (true){
    for (int i = 0 ; i < 8; i++){
      move_servo(ID2,ID3,pos[i][0][0],pos[i][0][1],Time) ;
      move_servo(ID5,ID6,pos[i][1][0],pos[i][1][1],Time) ;
      move_servo(ID8,ID9,pos[i][2][0],pos[i][2][1],Time) ;
      move_servo(ID11,ID12,pos[i][3][0],pos[i][3][1],Time) ;
      delay(Time) ;
    }
  }

}



void setup(){
  // put your setup code here, to run once:
  Serial.begin(9600); //open mv
  Bluetooth05.begin(115200);
  ibus.begin(Serial) ;
  pinMode(13,OUTPUT) ;
   //initialise position
  LobotSerialServoMove(Bluetooth05, ID1, 312, 500);
  LobotSerialServoMove(Bluetooth05, ID7, 688, 500) ;

  LobotSerialServoMove(Bluetooth05, ID4, 312, 500) ;
  LobotSerialServoMove(Bluetooth05, ID10, 688, 500) ;
  delay(500) ;

}




void loop(){
  
  /* 地盘压低测试
  y_offset = -30 ;
  x_offset = 0 ;
  Double_leg_control() ;
  y_offset = 0 ;
  Double_leg_control() ;
  
  y_offset = -60 ;
  x_offset = -100 ;
  Double_leg_control() ; 
  */

  //Single_leg_control() ;
  //Double_leg_control() ;
  Double_leg_slope_control() ;
  if(Bluetooth05.available()){

    Bluetooth05.readBytes(buf,4);
    // 填充
    for(int i = 0;i < 4; i++){
      recieve_data.bytes[i] = buf[i];
      Serial.println(buf[i],HEX) ;
    }
    Serial.println(recieve_data.f);
    //for (int j = 0; j < 4; j++) Serial.println(int(recieve_data.bytes[j])) ;
    Serial.println("______________________") ;
  }




}













