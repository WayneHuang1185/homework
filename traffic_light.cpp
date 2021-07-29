#include <LedControl.h>
#include <LiquidCrystal_I2C.h>
#define DIN 8
#define CS 7
#define CLK 6
#define Red 9
#define Green 5
#define Blue 3
#define Width 8
#define Height 8
#define totle_time 30.0
LiquidCrystal_I2C lcd(0x27,16,2);
LedControl LC=LedControl(DIN,CLK,CS,1);
byte data_first[8][8]={{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,0},{0,0,0,0,1,1,0,0},{0,0,0,1,1,1,1,0},{0,0,0,1,1,0,0,0},{0,0,1,0,1,1,0,0},{0,1,0,0,0,1,0,0},{0,1,1,0,0,1,1,0}};
byte data_second[8][8]={{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,0},{0,0,0,0,1,1,0,0},{0,0,1,1,1,1,1,0},{0,1,0,1,1,0,1,0},{0,0,1,0,1,1,0,0},{0,1,0,0,0,1,1,0},{0,1,0,0,0,0,1,0}};
byte data_third[8][8]={{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,0},{0,0,0,0,1,1,0,0},{0,0,1,1,1,1,1,0},{0,1,0,0,1,1,0,1},{0,0,0,1,1,0,0,0},{0,1,1,0,0,1,1,0},{0,1,0,0,0,0,0,0}};
byte data_four[8][8]={{0,0,0,0,0,0,1,1},{0,0,0,0,0,0,1,1},{0,0,0,1,1,1,1,0},{0,0,1,0,0,1,1,0},{0,0,0,0,1,1,0,1},{0,0,0,1,0,1,0,0},{0,1,1,0,0,0,1,1},{0,1,0,0,0,0,0,0}};
byte data_five[8][8]={{0,0,0,0,0,0,1,1},{0,0,0,0,0,0,1,1},{0,0,0,0,0,1,1,0},{0,0,0,1,1,1,1,0},{0,0,1,0,1,1,0,1},{0,1,0,1,1,1,0,0},{0,1,1,0,0,1,0,0},{0,1,0,0,0,1,1,1}};
byte data_six[8][8]={{0,0,0,0,0,0,1,1},{0,0,0,0,0,0,1,1},{0,0,0,0,0,1,1,0},{0,0,0,1,1,1,1,0},{0,0,1,0,1,1,0,1},{0,1,0,0,1,1,0,0},{0,0,1,1,0,0,1,0},{0,0,1,0,0,0,1,1}};
byte data_seven[8][8]={{0,0,0,0,0,0,1,1},{0,0,0,0,0,0,1,1},{0,0,0,0,0,1,1,0},{0,0,0,1,1,1,1,0},{0,0,1,0,1,1,1,0},{0,0,0,0,1,1,0,0},{0,0,0,1,1,0,1,0},{0,0,0,1,0,1,0,0}};
byte data_eight[8][8]={{0,0,0,0,0,1,1,0},{0,0,0,0,0,1,1,0},{0,0,0,0,1,1,0,0},{0,0,0,1,1,1,0,0},{0,0,0,1,1,0,0,0},{0,0,1,0,1,0,0,0},{0,0,1,1,1,0,0,0},{0,0,1,0,0,0,0,0}};
byte data_finnal[8][8]={{0,0,0,1,1,0,0,0},{0,0,0,1,1,0,0,0},{0,0,1,1,1,1,0,0},{0,1,0,1,1,0,1,0},{0,1,0,1,1,0,1,0},{0,0,0,1,1,0,0,0},{0,0,1,0,1,1,0,0},{0,1,0,1,1,0,1,0}};
void change_color(char c);
void Close();
void Update(byte array_[Height][Width],double &test){
  if(test>0){
    int time_;
    if(test>10){
      time_=1000;
    }
    else{
      time_=200;
    }
    if(test>2){
      change_color('G');
    }
    else{
      change_color('Y');
    }
    for(int i=0;i<Height;i++){
      for(int j=0;j<Width;j++){
        LC.setLed(0,j,i,array_[i][j]);
      }
    }
    delay(time_);
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print((int)test);
    test-=(double)time_/1000;
  }
  return;
}
void Close(){
  for(int i=0;i<Height;i++){
    LC.setRow(0,i,0);
  }
}
void change_color(char c){
   Serial.println(c);
   if(c=='R'){
     analogWrite(Red,255);
     analogWrite(Green,0);
     analogWrite(Blue,0);
   }
   else if(c=='G'){
     analogWrite(Red,0);
     analogWrite(Green,255);
     analogWrite(Blue,0);
   }
   else if(c=='Y'){
     analogWrite(Red,255);
     analogWrite(Green,255);
     analogWrite(Blue,0);
   }
}
void set_back(){
   analogWrite(Red,0);
   analogWrite(Green,0);
   analogWrite(Blue,0);
}
void all(double T){
  while(T>0){
   Update(data_first,T);
   Close();
   Update(data_second,T);
   Close();
   Update(data_third,T);
   Close();
   Update(data_four,T);
   Close();
   Update(data_five,T);
   Close();
   Update(data_six,T);
   Close();
   Update(data_seven,T);
   Close();
   Update(data_eight,T);
   Close();
  }
  for(int i=0;i<Height;i++){
      for(int j=0;j<Width;j++){
        LC.setLed(0,j,i,data_finnal[i][j]);
      }
    }
  change_color('R');
  
}
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  pinMode(DIN,OUTPUT);
  pinMode(CS,OUTPUT);
  pinMode(CLK,OUTPUT);
  LC.shutdown(0,false);
  LC.setIntensity(0,15);
  all(totle_time);
  lcd.clear();
}

void loop() {
   
}
