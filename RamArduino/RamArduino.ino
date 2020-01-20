bool memory[16][8] =
{ {1, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(i + 2, OUTPUT);
  }

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(0, INPUT);
  pinMode(1, INPUT);

  for (int i = 0; i < 4; i++) {
    pinMode(i + 10, INPUT);
  }

}

void loop() {
  int x = getAddress();
  int A7State = analogRead(A7) > 100 ? 1 : 0;

  for (int i = 0; i < 8; i++){
    digitalWrite(2+i, memory[x][7-i]);
  }

  if (A7State == 0){
    memory[x][0] = digitalRead(A0);
    memory[x][1] = digitalRead(A1);
    memory[x][2] = digitalRead(A2);
    memory[x][3] = digitalRead(A3);
    memory[x][4] = digitalRead(A4);
    memory[x][5] = digitalRead(A5);
    memory[x][6] = digitalRead(0);
    memory[x][7] = digitalRead(1);
  }

}

int getAddress() {
  int Addr1 = digitalRead(10);
  int Addr2 = digitalRead(11);
  int Addr3 = digitalRead(12);
  int Addr4 = digitalRead(13);

  return (Addr1 + (Addr2 * 2) + (Addr3 * 4) + (Addr4 * 8));
}
