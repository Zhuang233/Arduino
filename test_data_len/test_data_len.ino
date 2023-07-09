void setup() {
  Serial.begin(9600);

  // 以下为uno开发板(8位微控制器)测试结果
  Serial.println(sizeof(long));// 4
  Serial.println(sizeof(unsigned long));// 4
  Serial.println(sizeof(double));// 4
  Serial.println(sizeof(float));// 4
  Serial.println(sizeof(int));// 2
  Serial.println(sizeof(unsigned int));// 2
  Serial.println(sizeof(word));// 2
  Serial.println(sizeof(short));// 2
  Serial.println(sizeof(bool));// 1
  Serial.println(sizeof(char));// 1
  Serial.println(sizeof(unsigned char));// 1
  Serial.println(sizeof(byte));// 1
  Serial.println(sizeof(void));// 1
  Serial.println(sizeof(String));// 6

  Serial.println(sizeof(int *));// 2
  Serial.println(sizeof(double *));// 2
  Serial.println(sizeof(char *));// 2
  Serial.println(sizeof(void *));// 2
}

void loop() {
}
