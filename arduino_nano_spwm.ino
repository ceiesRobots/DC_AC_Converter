/*
  The values:
  |-------------------|
  |freq = ture  = 60Hz|
  |freq = false = 50Hz|
  |volt = true  = 220v|
  |volt = false = 110v|
  |-------------------|
*/
#include <TimerOne.h>
#include <LiquidCrystal_I2C.h>

int pwm1 = 9;
int pwm2 = 10;
int freq60 = 36; // Control the Freq
int freq50 = 45; // Control the Freq
int pin2, pin3; // For the switches
volatile boolean freq = true;
volatile boolean volt = false;
int pwmSin220v[]={0,18,36,54,71,89,107,125,143,160,178,195,213,230,248,265,282,299,316,333,350,367,384,400,416,433,449,465,
481,496,512,527,543,558,573,587,602,616,630,644,658,672,685,698,711,724,737,749,761,773,784,796,807,818,
828,839,849,859,868,878,887,896,904,912,920,928,935,943,949,956,962,968,974,979,984,989,994,998,1002,1005,
1008,1011,1014,1016,1018,1020,1022,1023,1023,1024}; // 100% of the wave
int pwmSin110v[] = {0,9,18,27,36,45,54,62,71,80,89,98,106,115,124,133,141,150,158,167,175,183,192,200,208,216,224,232,240,248,
256,264,271,279,286,294,301,308,315,322,329,336,343,349,356,362,368,374,380,386,392,398,403,409,414,419,424,
429,434,439,443,448,452,456,460,464,468,471,475,478,481,484,487,490,492,495,497,499,501,503,504,506,507,508,
509,510,511,511,512,512,512}; // 50% of the wave

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD config

void setup(){
    // put your setup code here, to run once:
    Timer1.initialize(30);// 33.333kHz
    lcd.begin(); // Start the LCD
    pinMode(2,INPUT); //FreqPin
    pinMode(3,INPUT); //VoltPin
    pinMode(9,OUTPUT);  // SPWM1
    pinMode(10,OUTPUT); // SPWM2
    /*------------LCD---------------*/
    lcd.backlight();
    lcd.print("*  KAU SMART  *");
    lcd.setCursor(0,1);
    lcd.print("** CONVERTER **");
    digitalWrite(pwm1,LOW);
    digitalWrite(pwm2,LOW);
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Volt  ||  Freq ");
    lcd.setCursor(0,1);
    lcd.print("       ||       ");
    if (digitalRead(3) == 0) {
      volt = false;
      lcd.setCursor(1,1);
      lcd.print("110V");
    }
    else if (digitalRead(3) == 1) {
      volt = true;
      lcd.setCursor(1,1);
      lcd.print("220V");
    }
    if (digitalRead(2) == 0) {
      freq = false;
      lcd.setCursor(11,1);
      lcd.print("50Hz");
    }
    else if (digitalRead(2) == 1) {
      freq = true;
      lcd.setCursor(11,1);
      lcd.print("60Hz"); 
    }
    /*------------------------------*/
}

void loop(){
    //The pin state specified in digitalRead()stored in the value variable.
    //“0” is stored if it is 0V or “1” if it is 5V.
    /* To update the LCD */
    if(volt == true && digitalRead(3) == 0){
      lcd.setCursor(1,1);
      lcd.print("110V");
      }
    else if(volt == false && digitalRead(3) == 1){
      lcd.setCursor(1,1);
      lcd.print("220V");
      }
    else if(freq == true && digitalRead(2) == 0){
      lcd.setCursor(11,1);
      lcd.print("50Hz");
      }
    else if(freq == false && digitalRead(2) == 1){
      lcd.setCursor(11,1);
      lcd.print("60Hz");  
      }
    /*To change the frequancy & the voltage*/
    if (digitalRead(3) == 0) {
      volt = false;
    }
    else if (digitalRead(3) == 1) {
      volt = true;
    }
    if (digitalRead(2) == 0) {
      freq = false;
    }
    else if (digitalRead(2) == 1) {
      freq = true;
    }
  /*Final Execution*/
  if (freq == true && volt == true) { //                                  60Hz-220v
        loop_HF(freq60,pwmSin220v);
  } else if (freq == true && volt == false) { //                          60Hz-110v
        loop_HF(freq60,pwmSin110v);
  } else if (freq == false && volt == true) { //                          50Hz-220v
        loop_HF(freq50,pwmSin220v);
  } else { //                                                             50Hz-110v
        loop_HF(freq50,pwmSin110v);
  }
}

/*SPMW Generator Function*/
void loop_HF(int freq, int pwmSin[]){
    digitalWrite(pwm2,LOW);
    for (int i=0;i<=89;i++) {
      Timer1.pwm(pwm1 , pwmSin[i]);
      delayMicroseconds(freq); // 50Hz=45, 60Hz=36
    }
    for (int i=89;i>=0;i--) {
      Timer1.pwm(pwm1 , pwmSin[i]);
      delayMicroseconds(freq); // 50Hz=45, 60Hz=36
    }

    digitalWrite(pwm1,LOW);
    for (int i=0;i<=89;i++) {
      Timer1.pwm(pwm2 , pwmSin[i]);
      delayMicroseconds(freq);//  50Hz=45, 60Hz=36
    }
    for (int i=89;i>=0;i--) {
      Timer1.pwm(pwm2 , pwmSin[i]);
      delayMicroseconds(freq);//  50Hz=45, 60Hz=36
    }
}
