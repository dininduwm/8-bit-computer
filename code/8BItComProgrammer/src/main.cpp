#include <Arduino.h>
#define CLK 38
#define ProgEN 42
#define Reset 40
#define cReset 46
#define miReset 44

int pinArray[] = {22, 24, 26, 28, 30, 32, 34, 36};


int code[] = {
  50,
  62,
  0b00010000,
  0b00110001,
  0b11100000, 
  0b11110000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
};
/*
int code[] = {
  0b00000000,
  0b00100100,
  0b11100000,
  0b01100001,
  0b00000001, 
  0b10110000,
  0b10110000,
  0b10110000,
  0b10110000,
  0b10110000,
  0b10110000,
  0b10110000,
  0b10110000,
  0b10110000,
  0b10110000,
};
*/
/*
int code[] = {
  0b00001111,
  0b00100100,
  0b00100100,
  0b00100100,
  0b00100100, 
  0b00100100,
  0b00100100,
  0b10110000,
  0b10110000,
  0b10110000,
  0b10110000,
  0b10110000,
  0b10110000,
  0b10110000,
  0b10110000,
};
*/
String getValue(String data, char separator, int index)
{
    int maxIndex = data.length() - 1;
    int j = 0;
    String chunkVal = "";

    for (int i = 0; i <= maxIndex && j <= index; i++)
    {
        chunkVal.concat(data[i]);

        if (data[i] == separator)
        {
            j++;

            if (j > index)
            {
                chunkVal.trim();
                return chunkVal;
            }

            chunkVal = "";
        }
        else if ((i == maxIndex) && (j < index)) {
            chunkVal = "";
            return chunkVal;
        }
    }   
}

void reset(){
  pinMode(Reset, OUTPUT);
  pinMode(cReset, OUTPUT);
  pinMode(miReset, OUTPUT);
  digitalWrite(Reset, HIGH);
  digitalWrite(cReset, LOW);
  digitalWrite(miReset, LOW);
  delayMicroseconds(1);
  digitalWrite(Reset, LOW);
  digitalWrite(cReset, HIGH);
  digitalWrite(miReset, HIGH);
  pinMode(Reset, INPUT);
  pinMode(cReset, INPUT);
  pinMode(miReset, INPUT);
}

void pulsePin(){
  digitalWrite(CLK, HIGH);
  delay(1000);
  digitalWrite(CLK, LOW);
  delay(1000);
} 

void shift( int input ){
  // pin 2 to 9
  for(int i = 0 ; i <= 7 ; i ++ ){
    int a = input % 2 ;
    input = input / 2 ;
    digitalWrite( pinArray[i] , a );
    //Serial.print(a);

  }
  //Serial.println();

}

void setup() {
  Serial.begin(9600);

  for (int i = 0; i <= 7; i++){ //making the data bus as OUTPUT
    pinMode(pinArray[i], INPUT);
  }
  pinMode(Reset, INPUT);
  pinMode(cReset, INPUT);
  pinMode(miReset, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(CLK, INPUT);
  pinMode(ProgEN, OUTPUT);
  digitalWrite(ProgEN, LOW); //initialize the clk
/*
  for (int i = 0; i <= 15; i++){ //uploading the code
    Serial.print("ROM POSITION = ");
    Serial.println(i);

    Serial.print("RAM ADDR - ");
    shift(i); 
    pulsePin();
    delay(1000);

    Serial.print("RAM DATA - ");
    shift(code[1]);
    pulsePin();
    delay(1000);

    Serial.println();
  }
*/
}

String mode = "idle";
bool runMode = true;
bool highFlag = false;
bool lowFlag = true;
bool resetFlag = true;
bool runFalg = false;
bool resetMIFlag = true;
int tmpcount = 0;
int codeCount = 0; //save the count when intaking the programme
bool tmpFlag = true;

void loop() {
  if (mode == "prog"){
    digitalWrite(ProgEN, HIGH); //initialize the clk
    digitalWrite(LED_BUILTIN, LOW);

    bool clk = digitalRead(CLK);
    if (clk == HIGH && highFlag && runMode){
      //Serial.println("HIGH");
      highFlag = false;
      lowFlag = true;
    }
    if (clk == LOW && lowFlag && runMode){
      //Serial.println("LOW");
      lowFlag = false;
      highFlag = true;
      if (resetMIFlag){ //syncronize step
        pinMode(miReset, OUTPUT);
        digitalWrite(miReset, LOW);
        digitalWrite(miReset, HIGH);
        pinMode(miReset, INPUT);
        resetMIFlag = false;
      }
      if (tmpcount%2 == 0){
        //ADDR
        //Serial.print("RAM ADDR - ");
        shift(tmpcount/2);
      }
      else{
        //DATA
        //Serial.print("RAM DATA - ");
        shift(code[tmpcount/2]);
      }
      tmpcount++;
      //Serial.println(tmpcount);
      
    }
    
    if (tmpcount>=33 && resetFlag){
      runMode = false;
      for (int i = 0; i <= 7; i++){ //making the data bus as OUTPUT
        pinMode(pinArray[i], INPUT);
      }
      //Serial.println("Done Uploading");
      //Serial.println("Resetting the computer");
      digitalWrite(ProgEN, LOW); //disable the programming mode
      reset();
      resetFlag = false;
    }

    if ((!resetFlag) && (!runFalg)) {
      //waiting until the programme start
      //Serial.println("Waiting");
      if (clk == LOW){
        reset();
        runFalg = true;
        //Serial.println("Waiting Done");
        Serial.println("k");
        mode = "idle";
        for (int i = 0; i <= 7; i++){ //making the data bus as OUTPUT
          pinMode(pinArray[i], INPUT);
        }
      }
    }

    if (runFalg){    
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  else if (mode == "idle"){
    while(Serial.available()){
      String s = Serial.readString();
      //Serial.println(s);
      if (s == "prog") {
        //Serial.println("Prog Mode");
        runMode = true;
        highFlag = false;
        lowFlag = true;
        resetFlag = true;
        runFalg = false;
        resetMIFlag = true;
        tmpcount = 0;
        mode = "prog";        
        reset(); //reset the computer
        for (int i = 0; i <= 7; i++){ //making the data bus as OUTPUT
          pinMode(pinArray[i], OUTPUT);
        }
      }
      else if (s == "inp"){
        mode = "input";
        tmpFlag = true;
        //Serial.println("Input Mode");
        codeCount = 0;
      }
      else if (s == "res"){
        reset();
        //Serial.println("Reset");
        codeCount = 0;
      }
    }
  }
  else if (mode == "input"){
    while(tmpFlag){
      while(Serial.available()){      
        String s = Serial.readString();
        //Serial.println(s);
        for (int j = 0; j <= 15; j++){
          int tmpx = getValue(s, ',', j).toInt();
          //Serial.println(tmpx);
          code[j] = tmpx;
        }     
        tmpFlag = false;   
      }
    }
    mode = "idle";
    Serial.println("u");
  }
}