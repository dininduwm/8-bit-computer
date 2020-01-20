#define CLK A0
#define LATCH A1
#define DATA A2
#define OUT 9

int c1, c2, c3; //clocl inputs
int in1, in2, in3, in4;

int flag = 0; //flag to stop the constant shifting

void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(DATA, OUTPUT);

  for (int i = 2; i <= 8; i++){
    pinMode(i, INPUT);
  }

  pinMode(OUT, OUTPUT);
  digitalWrite(OUT, HIGH);
}

void loop() {
  c1 = digitalRead(2); //reading the clock input
  c2 = digitalRead(3);
  c3 = digitalRead(4);

  in1 = digitalRead(5); // reading the value from the instruction register
  in2 = digitalRead(6);
  in3 = digitalRead(7);
  in4 = digitalRead(8);

  //digitalWrite(13, c3);

  //fech cycle
  if (c3 == 0 & c2 == 0 & c1 == 0 & flag == 0){
    writeData(0xFF, 0x77);
    flag = 1;
  }
  else if (c3 == 0 & c2 == 0 & c1 == 1 & flag == 1){
    writeData(0xF7, 0xDB);
    flag = 2;
  }
  else if (c3 == 0 & c2 == 1 & c1 == 0 & flag == 2){
    writeData(0xFF, 0xFF);
    flag = 3;
  }
  else if(flag == 3){
    flag = 0;
  }
}

void writeData(byte data1, byte data2){
  digitalWrite(LATCH, LOW);
  digitalWrite(OUT, HIGH); //disable output
  // shift the bits out:
  shiftOut(DATA, CLK, MSBFIRST, data1);
  shiftOut(DATA, CLK, MSBFIRST, data2);       
  digitalWrite(LATCH, HIGH);
  digitalWrite(OUT, LOW); //enable output
}
