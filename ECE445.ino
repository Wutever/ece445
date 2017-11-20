#include <stdio.h>

#define EMPTY 0
#define CIRCLE 1
#define CROSS 2

int Max7219_pinCLK = 11;
int Max7219_pinCS = 9;
int Max7219_pinDIN = 12; 

/*Patterns Dictionary*/
unsigned char pattern[3][8] = {
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x41,0x22,0x14,0x08,0x14,0x22,0x41},
  {0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C}
};

/*Game Status*/
unsigned char LED_status[9] = {
   0,0,0,
   0,0,0,
   0,0,0
};

unsigned char disp1[38][8]={
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//0
{0x10,0x18,0x14,0x10,0x10,0x10,0x10,0x10},//1
{0x7E,0x2,0x2,0x7E,0x40,0x40,0x40,0x7E},//2
{0x3E,0x2,0x2,0x3E,0x2,0x2,0x3E,0x0},//3
{0x8,0x18,0x28,0x48,0xFE,0x8,0x8,0x8},//4
{0x3C,0x20,0x20,0x3C,0x4,0x4,0x3C,0x0},//5
{0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C,0x0},//6
{0x3E,0x22,0x4,0x8,0x8,0x8,0x8,0x8},//7
{0x0,0x3E,0x22,0x22,0x3E,0x22,0x22,0x3E},//8
{0x3E,0x22,0x22,0x3E,0x2,0x2,0x2,0x3E},//9
{0x8,0x14,0x22,0x3E,0x22,0x22,0x22,0x22},//A
{0x3C,0x22,0x22,0x3E,0x22,0x22,0x3C,0x0},//B
{0x3C,0x40,0x40,0x40,0x40,0x40,0x3C,0x0},//C
{0x7C,0x42,0x42,0x42,0x42,0x42,0x7C,0x0},//D
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C},//E
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x40},//F
{0x3C,0x40,0x40,0x40,0x40,0x44,0x44,0x3C},//G
{0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44},//H
{0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x7C},//I
{0x3C,0x8,0x8,0x8,0x8,0x8,0x48,0x30},//J
{0x0,0x24,0x28,0x30,0x20,0x30,0x28,0x24},//K
{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C},//L
{0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x81},//M
{0x0,0x42,0x62,0x52,0x4A,0x46,0x42,0x0},//N
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//O
{0x3C,0x22,0x22,0x22,0x3C,0x20,0x20,0x20},//P
{0x1C,0x22,0x22,0x22,0x22,0x26,0x22,0x1D},//Q
{0x3C,0x22,0x22,0x22,0x3C,0x24,0x22,0x21},//R
{0x0,0x1E,0x20,0x20,0x3E,0x2,0x2,0x3C},//S
{0x0,0x3E,0x8,0x8,0x8,0x8,0x8,0x8},//T
{0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1C},//U
{0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18},//V
{0x0,0x49,0x49,0x49,0x49,0x2A,0x1C,0x0},//W
{0x0,0x41,0x22,0x14,0x8,0x14,0x22,0x41},//X
{0x41,0x22,0x14,0x8,0x8,0x8,0x8,0x8},//Y
{0x0,0x7F,0x2,0x4,0x8,0x10,0x20,0x7F},//Z
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} // empty 
};

/* General byte wise write */
void Write_Max7219_byte(unsigned char DATA) 
{   
        unsigned char i;
        
        for(i=8;i>=1;i--)
        {     
             // Serial.println(digitalRead(5));

             //digitalWrite(Max7219_pinCLK,LOW);
             PORTB = PORTB&0xf7;
             //digitalWrite(Max7219_pinDIN,DATA&0x80);// Extracting a bit data ,looks a bit werid but works 
             PORTB = (PORTB&0xef)|((DATA&0x80)>>3);
             DATA = DATA<<1;
             PORTB = PORTB|0x08;
             //digitalWrite(Max7219_pinCLK,HIGH);
        }  
}

/* 3*3 LED write */
void Write_Max7219(unsigned char address,unsigned char dat,unsigned char LED_position)
{
       PORTD = (PORTD&0x0f)|(LED_position<<4);
       Write_Max7219_byte(address);           //address，code of LED
       Write_Max7219_byte(dat);               //data，figure on LED
       PORTD=(PORTD|0xf0);
}

void Write_Max7219_letter(unsigned char address,unsigned char dat,unsigned char letter_position)
{
       PORTB = (PORTB&0xfc);//|(letter_position&0x03);
       Write_Max7219_byte(address);           //address，code of LED
       Write_Max7219_byte(dat);               //data，figure on LED
       PORTB=(PORTB|0x03);
}

/* Init LED chip */
void Init_MAX7219(void)
{
       int i ;
       
       for (i=0;i<9;i++){
          Write_Max7219(0x09, 0x00,i);       //decoding ：BCD
          Write_Max7219(0x0a, 0x03,i);       //brightness 
          Write_Max7219(0x0b, 0x07,i);       //scanlimit；8 LEDs
          Write_Max7219(0x0c, 0x01,i);       //power-down mode：0，normal mode：1
          Write_Max7219(0x0f, 0x00,i);       //test display：1；EOT，display：0
       }
       for (i=0;i<4;i++){
          Write_Max7219_letter(0x09, 0x00,i);       //decoding ：BCD
          Write_Max7219_letter(0x0a, 0x03,i);       //brightness 
          Write_Max7219_letter(0x0b, 0x07,i);       //scanlimit；8 LEDs
          Write_Max7219_letter(0x0c, 0x01,i);       //power-down mode：0，normal mode：1
          Write_Max7219_letter(0x0f, 0x00,i);       //test display：1；EOT，display：0
       }
 
}

/* Empty LED */
void LED_empty(){
       int S_time=0;
       int j;
       int i;
       
       for(j=0;j<19;j++)
       {
         for(i=1;i<9;i++){
            Write_Max7219(i,pattern[EMPTY][i-1],S_time);
         }
         S_time++;     
       } 
}

/* General Initialization */
void setup()
{
      //Initial Serial      
      //declear CS DIN
      //pinMode(Max7219_pinCLK,OUTPUT);
      //pinMode(Max7219_pinDIN,OUTPUT);

      //declear PIN 0 to 7
      DDRD = B11110000;

      //declear PIN 8 to 13
      DDRB = B11111011;
      //init 3*3 LED module
      Init_MAX7219();

      //empty 3*3 LED module
      LED_empty();
}


/* Set single LED */
void LED_set(int LED_position, int LED_pattern){
     int i;
     
     for(i=1; i<9; i++){
        Write_Max7219(i,disp1[LED_pattern][i-1],LED_position);
     }
     

}
void LED_set_9_letter(char* array1){
   int i = 0 ;
   int letter; 
   for(i=0;i<9;i++){
    if(array1[i]>=97){
      letter = array1[i]-97 + 10;
    }
    else if (array1[i]>=48){
      letter = array1[i] - 48 ;
    }
    //error reading
    else if(array1[i]==32)
    letter =  36;
    else 
    letter = 14;
    LED_set(i,letter);
   }
}

void Letter_set(int letter_position,int Letter_pattern){
     int i;
     for(i=1; i<9; i++){
        Write_Max7219_letter(i,disp1[Letter_pattern][i-1],letter_position);
     }
}

/* Refresh LED */
void LED_refresh(){
     int i;
  
     for(i = 0; i < 9; i++){
        if(CROSS == LED_status[i]){
          continue;
        }
        LED_set(i, LED_status[i]);
     }
}

/* Set Status Game */
void LED_status_set(int LED_position, int LED_pattern) {
     LED_status[LED_position] = LED_pattern;
}
int get_press_led (){
  Letter_set(0,5);
  
  unsigned char mask = 0x0f; 
  int input;
  input = (PIND&0x0f)^mask;
  while (input==0){
    input = (PIND&0x0f)^mask;
  }
  return input-1;
}
void pressure_led_set(int LED_pattern){


  unsigned char input ;
    input= get_press_led ();
    while (LED_status[input]!=EMPTY){
          input= get_press_led ();
    }
    int i ;
   // LED_set(input,LED_pattern);
     for(i=1; i<9; i++){
        Write_Max7219(i,pattern[LED_pattern+1][i-1],input);
     }
    LED_status_set(input, LED_pattern+1);
}
//we can change this to wait for anything being pressed
void wait_start_button(){
  int a  =  analogRead(0);
  while(a>=900){
     a  =  analogRead(0);
  }
}
int choose_mode(){
  int pressed_button=get_press_led ();
  if(pressed_button==1)
  return 0;
  else if (pressed_button==7)
  return 1;
  else return 2;
}
void single_player(){
  LED_set_9_letter("singleply");
  delay(3000);
   LED_set_9_letter("start1111");
   delay(3000);
   LED_empty();
   
  return;  
}
void double_player(){
  LED_set_9_letter("doubleply");
  delay(3000);
   LED_set_9_letter("start1111");
   delay(3000);
   LED_empty();
   int a = 0;
    while (a<9){
  pressure_led_set(a%2);
  delay(1000);
  a++;
    }
  return;
}
void not_correct_mode(){
  LED_set_9_letter("chooseagn");
  delay(3000);
}
void game_start(){
  LED_set_9_letter("gamestart");
  delay(3000);
  choosemod:
  LED_set_9_letter("choosemod");
  delay (3000);
  LED_set_9_letter("1plo r2pl");
  delay (3000);
  LED_set_9_letter(" 1     2 ");
  int mode = choose_mode();
  if(mode==0){
  single_player();  
  }
  else if (mode==1){
  double_player();  
  }
  else{
  not_correct_mode();
  goto choosemod ;
  }
}
/*
 * Game Main Body
 */
void loop()
{ 
  wait_start_button();
  Letter_set(0,3);
  game_start();



//    LED_refresh();
//    LED_Game_Print();
//    LED_set(3,0);
//    LED_refresh();
//    LED_Game_Print();
    while(1){};
}
