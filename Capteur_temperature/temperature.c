const int tempPin = A0;
float temperature;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(tempPin);
  temperature = (sensorValue * 5.0 * 100.0) / 1024;

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  delay(1000);
}
