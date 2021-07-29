#include<Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#define Red 6
#define Green 5
#define Blue 3
#define servo_pin 9
#define Max_D 100
#define trig_pin 2
#define echo_pin 4
#define tone_pin 8
#define RST_PIN  7  
#define NSS_PIN  10
#define Do 262
#define Re 294
#define Mi 330
#define Fa 369
#define Sol 415
#define La  466
#define Si  523
MFRC522 mfrc522 (NSS_PIN,RST_PIN);
Servo S;
void setup() {
  // put your setup code here, to run once:
  pinMode(Red,OUTPUT);
  pinMode(Blue,OUTPUT);
  pinMode(Green,OUTPUT);
  pinMode(tone_pin,OUTPUT);
  pinMode(trig_pin,OUTPUT);
  pinMode(echo_pin,INPUT);
  Serial.begin(9600); // 通訊 baud 值
  SPI.begin();
  S.attach(servo_pin);
  S.write(0);
  mfrc522.PCD_Init(); // 初始化 MFRC522 讀卡機模組
  Serial.print(F("Reader ")); // 印出文字
  Serial.print(F(": ")); // 印出文字
  mfrc522.PCD_DumpVersionToSerial(); // 顯示讀卡設備的版本 
}
int get_distance(){
  digitalWrite(trig_pin,LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin,LOW);
  long duration_=pulseIn(echo_pin,HIGH);
  int distance=duration_*0.34/2;
  return distance/10;
}
byte in[4]={99,170,136,21};
int little_bee[4][16]={
{Sol,Mi,Mi,0,Fa,Re,Re,0,Do,Re,Mi,Fa,Sol,Sol,Sol,0},
{Sol,Mi,Mi,0,Fa,Re,Re,0,Do,Mi,Sol,Sol,Mi,0,0},
{Re,Re,Re,Re,Re,Mi,Fa,0,Mi,Mi,Mi,Mi,Mi,Fa,Sol,0},
{Sol,Mi,Mi,0,Fa,Re,Re,0,Do,Mi,Sol,Sol,Do,0,0}};
void play_song(){
  for(int i=0;i<4;i++){
    for(int j=0;j<16;j++){
      if(little_bee[i][j]){
        tone(tone_pin,little_bee[i][j]);
      }
      delay(500);
      if(j!=15&&little_bee[i][j+1]){
        noTone(tone_pin);
      }
    }
    noTone(tone_pin);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  int cm=get_distance();
  Serial.println(cm);
  bool openflag=false;
  if(cm<10){
    analogWrite(Red,255);
    analogWrite(Green,0);
    analogWrite(Blue,0);
    Serial.println("Too dangerous");
  }
  else{
    analogWrite(Red,0);
    analogWrite(Green,255);
    analogWrite(Blue,0);
    openflag=true;
  }
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){ 
    byte *id = mfrc522.uid.uidByte;
    byte idSize = mfrc522.uid.size;
    for (byte i=0; i<4; i++){
      if (id[i]!=in[i]){ 
        openflag=false;
        Serial.println("Access Denied!");
        S.write(0);
        break;
       }
      }
    if(openflag){
      Serial.println("Access Granted!");
      S.write(45);
      play_song();
      S.write(0);
    }
  }

}
