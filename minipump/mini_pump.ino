int pump_pin = 3;
int sensor_pin = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pump_pin, OUTPUT);
  pinMode(sensor_pin, INPUT);
}

void loop() {
  // checks the water level
  if (analogRead(sensor_pin) >= 400){
    digitalWrite(pump_pin, HIGH);
    delay(500);
  }
  else{
    digitalWrite(pump_pin, LOW);
    delay(500);
  }
  Serial.println(analogRead(sensor_pin));
}
