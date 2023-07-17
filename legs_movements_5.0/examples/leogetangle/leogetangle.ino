
#include "leogetangle.h"

// leogetangle(x_tmp, x_tmp, &theta1, &theta2);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
    double theta1;
  double theta2;
  double x_tmp=0;
  double y_tmp=100;
  leogetangle(x_tmp, y_tmp, &theta1, &theta2);
  Serial.print("theta1= ");
  Serial.print(theta1);
  Serial.print("   theta2= ");
  Serial.println(theta2);
  // put your main code here, to run repeatedly:

}
