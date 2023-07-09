
#define MAX_LENTH 4

union U1
{
    char s[MAX_LENTH];
    double d;
};
 
union U2
{
    char s[MAX_LENTH];
    double d;
};

U1 u1;
U2 u2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  u1.d = 1.234;
  u2.d = 5.678;

  // 查看u1的各个字节
  for(int i = 0;i<MAX_LENTH;i++){
    // Serial.print(u1.s[i]);
  }
  // 查看u1的各个字节
  for(int i = 0;i<MAX_LENTH;i++){
    // Serial.print(u2.s[i]);
  }

  // 逐字节赋值
  for(int i = 0;i<MAX_LENTH;i++){
    u1.s[i] = u2.s[i];
  }

  // 直接打印double类型
  // Serial.print(u1.d);

}

void loop() {
  // put your main code here, to run repeatedly:

}
