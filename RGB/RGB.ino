#define R_pin 9
#define G_pin 10
#define B_pin 11

#define RLIGHT digitalWrite(9,LOW)
#define GLIGHT digitalWrite(10,LOW)
#define BLIGHT digitalWrite(11,LOW)
#define NOLIGHT digitalWrite(9,HIGH); \
                digitalWrite(10,HIGH); \
                digitalWrite(11,HIGH)

void setup() {
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(11,OUTPUT);
    NOLIGHT;
}

void loop() {
  NOLIGHT;
  RLIGHT;
  delay(1000);
  NOLIGHT;
  GLIGHT;
  delay(1000);
  NOLIGHT;
  BLIGHT;
  delay(1000);
}
