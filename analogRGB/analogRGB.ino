#define R_pin 9
#define G_pin 10
#define B_pin 11

#define NOLIGHT digitalWrite(R_pin,HIGH); \
                digitalWrite(G_pin,HIGH); \
                digitalWrite(B_pin,HIGH)

void setup() {
    pinMode(R_pin,OUTPUT);
    pinMode(G_pin,OUTPUT);
    pinMode(B_pin,OUTPUT);
    NOLIGHT;
}

void loop() {
  for(int val = 0; val < 255; val++){
    analogWrite(R_pin,val);
    delay(2);
  }
  for(int val = 255; val > 0; val--){
    analogWrite(R_pin,val);
    delay(2);
  }
}