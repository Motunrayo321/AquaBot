#define LIMIT_SWITCH_PIN 7
 
void setup() {
  Serial.begin(9600);
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
}
 
void loop() {
 
  if (digitalRead(LIMIT_SWITCH_PIN) == LOW)
  {
    Serial.println("Time to drain...");
  }
 
  else
  {
    // Keep Moving
  }
   
  delay(100);
}