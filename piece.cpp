#include <LedControl.h>
#define DIN 8
#define CS 7
#define CLK 6
#define Height 8
#define Width 8
#define button_r 10
#define button_l 9
#define button_fix 11
#define button_end_start 12
#define button_delay 20
#define piece_num 4
class Timer
{
  private:
    long Time;

  public:
    void TimerShutdown()
    {
      Time = -1;
    }

    void TimerSet()
    {
      Time = millis();
    }

    bool Timeout(long Millis)
    {
      long C_Time = millis();
      if ((C_Time - Time) < Millis || Time == -1)
      {
        return false;
      }
      else
      {
        return true;
      }
    }
};
byte board[8][8],bottom_layer[8],finnal[8][8];
int piece_color=1,state=0,start_x=0,start_y=7;
bool judge_start=true,end_start=true;
void Move(int direct);
void Set_start();
void Update();
void debug();
bool line();
bool column();
bool right_oblique();
bool left_oblique();
bool Judge();
void Ending();
void set_up();
LedControl LC=LedControl(DIN,CLK,CS,1);
Timer T;
void setup() {
  pinMode(DIN,OUTPUT);
  pinMode(CS,OUTPUT);
  pinMode(CLK,OUTPUT);
  pinMode(button_l,INPUT);
  pinMode(button_r,INPUT);
  pinMode(button_fix,INPUT);
  pinMode(button_end_start,INPUT);
  LC.shutdown(0,false);
  LC.setIntensity(0,15);
  Serial.begin(9600);
}

void loop() {
  while(end_start){
    Update();
    debug();
    Serial.println(state);
    switch(state){
      case 0:
        if(judge_start){
          Set_start();
          judge_start=false;
        }
        if(digitalRead(button_l)==HIGH){
           T.TimerSet();
           state=1; 
        }
        else if(digitalRead(button_r)==HIGH){
           T.TimerSet();
           state=4;
        }
        else if(digitalRead(button_fix)==HIGH){
           T.TimerSet();
           state=7;
        }
        else if(digitalRead(button_end_start)==HIGH){
           T.TimerSet();
           state=11;
        }
        break;
      case 1:
           if(T.Timeout(button_delay)){
             if(digitalRead(button_l)==HIGH){
                state=2; 
             }
             else{
              state=0;
             }
             T.TimerShutdown();
            }
           break;
      case 2:
          Move(1);
          state=3;
          break;
      case 3:
          if(digitalRead(button_l)==LOW){
              state=0; 
           }
           break;
       case 4:
          if(T.Timeout(button_delay)){
             if(digitalRead(button_r)==HIGH){
                state=5; 
             }
             else{
              state=0;
             }
             T.TimerShutdown();
            }
           break;
       case 5:
           Move(0);
           state=6;
           break;
       case 6:
           if(digitalRead(button_l)==LOW){
              state=0; 
           }
           break;
       case 7:
           if(T.Timeout(button_delay)){
             if(digitalRead(button_fix)==HIGH){
                state=8; 
             }
             else{
              state=0;
             }
             T.TimerShutdown();
            }
           break;
       case 8:
           Down();
           judge_start=true;
           state=9;
           break;
       case 9:
           if(Judge()){
             LC.clearDisplay(0);
             Ending();
             set_up();
             state=13;
           }
           else{
             state=10;
           }
           break;
       case 10:
           if(digitalRead(button_l)==LOW){
              state=0; 
           }
           if(piece_color==1){
             piece_color=2;
           }
           else{
             piece_color=1;
           }
           start_x=0;
           start_y=7;
           break;
       case 11:
          if(T.Timeout(button_delay)){
             if(digitalRead(button_end_start)==HIGH){
                state=12; 
             }
             else{
              state=0;
             }
             T.TimerShutdown();
            }
           break;
       case 12:
          if(digitalRead(button_end_start)==LOW){
              state=13; 
          }
          break;
       case 13:
          end_start=false;
          LC.clearDisplay(0);
          break;
     }
   }
   if(digitalRead(button_end_start)==HIGH){
      T.TimerSet();
      bool t=true;
      while(t){
        while(T.Timeout(button_delay)){
          if(digitalRead(button_end_start)==HIGH){
             set_up();
             end_start=true;
          }
          t=false;
          break;
        }
      }
   }
}
void debug(){
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
       Serial.print(board[i][j]);
    }
    Serial.println();
  }
}
void Set_start(){
   if(board[start_y][start_x]){
      if(start_x<=6){
        start_x+=1;
      }
   }
   board[start_y][start_x]=piece_color;
}
void Down(){
  int d=bottom_layer[start_x];
  board[start_y][start_x]=0;
  if(d<=6){
    board[d][start_x]=piece_color;
    start_y=d;
    bottom_layer[start_x]+=1;
  }
 
}
void Move(int direct){
   if(direct==0&&start_x+1<Width){
      if(board[start_y][start_x+1]==0){
      board[start_y][start_x]=0;
      board[start_y][start_x+1]=piece_color;
      start_x+=1;
      }
      
   }
   else if(direct==1&&start_x-1>=0){
      if(board[start_y][start_x-1]==0){
      board[start_y][start_x]=0;
      board[start_y][start_x-1]=piece_color;
      start_x-=1;
      }
   }
}
void Update(){
  T.TimerSet();
  for(int i=0;i<Height;i++){
    for(int j=0;j<Width;j++){
      int light=0;
      if(board[i][j]!=0){
        light=1;
      }
      LC.setLed(0,j,i,light);
    }
  }
  while(1){
    while(T.Timeout(1000)){
      for(int i=0;i<Height;i++){
         for(int j=0;j<Width;j++){
           if(board[i][j]==2){
             LC.setLed(0,j,i,0);
           }
         }
       }
       goto flag;
    }
  }
  flag:
     return;
}
bool line(){
  int count=1;
  int tmp=start_x;
  while(1){
     if(start_x+1<Width&&board[start_y][start_x+1]==piece_color){
        count+=1;
        start_x+=1;
       
     }
     else{
        break;
     }
  }
  Serial.println("test ok");
  start_x=tmp;
  while(1){
     if(start_x-1>=0&&board[start_y][start_x-1]==piece_color){
           count+=1;
           start_x-=1;
        
     }
     else{
        break;
     }
  }
  start_x=tmp;
  if(count>=piece_num){
    return true;
  }
  else{
    return false;
  }
}
bool column(){
  int count=1;
  int tmp=start_y;
  while(1){
     if(start_y+1<Height&&board[start_y+1][start_x]==piece_color){
        count+=1;
        start_y+=1;
     }
     else{
        break;
     }
  }
  start_y=tmp;
  Serial.println("test ok");
  while(1){
     if(start_y-1>=0&&board[start_y-1][start_x]==piece_color){
        count+=1;
        start_y-=1;
     }
     else{
        break;
     }
  }
  start_y=tmp;
  if(count>=piece_num){
    return true;
  }
  else{
    return false;
  }
}
bool right_oblique(){
  int count=1;
  int tmp_x=start_x;
  int tmp_y=start_y;
  while(1){
    if(start_x+1<Width&&start_y+1<Height){
      if(board[start_y+1][start_x+1]==piece_color){
        count+=1;
        start_x+=1;
        start_y+=1;
      }
      else{
        break;
      }
    }
    else{
      break;
    }
  }
  start_x=tmp_x;
  start_y=tmp_y;
  Serial.println("test ok");
  while(1){
    if(start_x-1>=0&&start_y-1>=0){
      if(board[start_y-1][start_x-1]==piece_color){
        count+=1;
        start_x-=1;
        start_y-=1;
      }
      else{
        break;
      }
    }
    else{
      break;
    }
  }
  start_x=tmp_x;
  start_y=tmp_y;
  if(count>=piece_num){
    return true;
  }
  else{
    return false;
  }
}
bool left_oblique(){
  int count=1;
  int tmp_x=start_x;
  int tmp_y=start_y;
  while(1){
    if(start_x-1>=0&&start_y+1<Height){
      if(board[start_y+1][start_x-1]==piece_color){
        count+=1;
        start_x-=1;
        start_y+=1;
      }
      else{
        break;
      }
    }
    else{
      break;
    }
  }
  start_x=tmp_x;
  start_y=tmp_y;
  Serial.println("test ok");
  while(1){
    if(start_x+1<Width&&start_y-1>=0){
      if(board[start_y-1][start_x+1]==piece_color){
        count+=1;
        start_x+=1;
        start_y-=1;
      }
      else{
        break;
      }
    }
    else{
      break;
    }
  }
  start_x=tmp_x;
  start_y=tmp_y;
  if(count>=piece_num){
    return true;
  }
  else{
    return false;
  }
}
bool Judge(){
  return line()||column()||right_oblique()||left_oblique();
}
void Ending(){
  int index_y=7;
  while(index_y>=0){
    for(int j=0;j<Width;j++){
       for(int i=Height-1;i>=0;i--){
          LC.setLed(0,j,i,!finnal[i][j]);
          if(i==index_y){
            finnal[i][j]=1;
          }
       }
    }
    delay(1000);
    LC.clearDisplay(0);
    index_y-=1;
  }
}
void set_up(){
  memset(board,0,64*sizeof(byte));
  memset(bottom_layer,0,8*sizeof(byte));
  memset(finnal,0,64*sizeof(byte));
  piece_color=1;
  state=0;
  start_x=0;
  start_y=7;
  judge_start=true;
}
