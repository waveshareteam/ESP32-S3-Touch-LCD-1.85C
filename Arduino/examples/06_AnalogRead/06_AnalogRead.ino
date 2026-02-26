void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  //set the resolution to 12 bits (0-4095)
  analogReadResolution(12);
}

void loop() {
  // read the analog / millivolts value for pin 2:
  int analogValue = analogRead(8);
  int analogVolts = analogReadMilliVolts(8);

  // print out the values you read:
  Serial.printf("ADC analog value = %d\n", analogValue);
  Serial.printf("BAT  millivolts value = %d\n", analogVolts*3);

  delay(100);  // delay in between reads for clear read from serial
}
