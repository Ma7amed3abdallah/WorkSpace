
// LCD module connections
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;
// End LCD module connections


//variables
int second;
int minute;
int hour;
int hr;
int day;
int dday;
int month;
int year;
int ap;
unsigned short set_count = 0;
short set;


char time[] = "00:00:00 PM";
char date[] = "00-00-00";

unsigned char Enter_ID[10]="", txt[17]="Correct Password";
char pass_word[5]="1111";
char i=0;
unsigned char keypadPort at PORTD;
unsigned char kp;
short cnt=0, oldstate = 0;
char e;
 //function prototypes
void Display_Time();
int BCD2Binary(int a);
int Binary2BCD(int a);
unsigned char BCD2LowerCh(unsigned char bcd);
unsigned char BCD2UpperCh(unsigned char bcd);
void write_ds1307(unsigned short address,unsigned short w_data);
unsigned short read_ds1307(unsigned short address);
void change_state(char k);
unsigned char Mask(char kp);
void Initialization();
char checkPassword();
void correctPassword();
void wrongPassword();
char readKeypad();
//*******************************


