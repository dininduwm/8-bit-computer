void setup() {
  for (int i = 2; i <= 9; i++){
    pinMode(i, INPUT);
  }
  Serial.begin(9600);

}

void loop() {
  for (int i = 2; i <= 9; i++){
    Serial.print(digitalRead(i));
  }
  Serial.println();
  delay(500);

}
