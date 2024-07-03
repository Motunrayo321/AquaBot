#define LIMIT_SWITCH_PIN 7
 
void setup() {
  Serial.begin(9600);
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
}
 
void loop() {
 
  Serial.println(digitalRead(LIMIT_SWITCH_PIN));
  delay(1000);
}