/*
  7segment.ino
  Created by Your Name, Date.
  This code controls a 7-segment display using servos and an RTC module.
*/
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <RTClib.h>



#define SERVOMIN  100 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  480 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

#define INPUT_SIZE 30

int ir_pin = 2;
int motion_detected = 0;
DateTime motion_time = DateTime(2026,01,2,11,25,00);

bool motion_active = false;
bool move_now = false;



Adafruit_PWMServoDriver Minutes = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver Hours = Adafruit_PWMServoDriver(0x41);
RTC_DS1307 rtc;

byte digits[] = {119,36,107,109,60,93,95,100,127,125,0};

                        //0,  1,  2,  3,  4,  5,  6
byte minuteOnesOn[] =  { 75, 60, 40, 58, 90, 80, 90};
byte minuteOnesOff[] = {147,130,105,120,170,150,163};

                       // 8,  9, 10, 11, 12, 13, 14
byte minuteTensOn[] =  { 85, 95, 65, 80, 75, 80, 75};
byte minuteTensOff[] = {155,165,125,150,160,170,150};

                   //16,17,18,19,20,21, 22
byte hourOnesOn[] =  { 90, 70, 90, 40, 80, 100, 105};
byte hourOnesOff[] = {167,150,160,110,160,175,175};

                     //24, 25, 26, 27, 28, 29, 30
byte hourTensOn[] =  { 95, 85,105, 75, 70, 85, 70};
byte hourTensOff[] = {172,170,175,150,150,160,172};


int currentMinutes = 0;
int currentHours = 0;

void setup() {
  // put your setup code here, to run once:

  rtc.begin();
  motion_time = rtc.now();

  //rtc.adjust(DateTime(2026,01,2,11,25,00));

  Minutes.begin();
  Minutes.setOscillatorFrequency(27000000);
  Minutes.setPWMFreq(SERVO_FREQ); 

  delay(100);

  Hours.begin();
  Hours.setOscillatorFrequency(27000000);
  Hours.setPWMFreq(SERVO_FREQ); 

  delay(100);

  Serial.begin(9600);
  while (! Serial); // Wait untilSerial is ready - Leonardos
  Serial.println("Serial Ready");


  updateServoBank(Minutes,0,127,minuteOnesOn,minuteOnesOff);

  delay(100);

  updateServoBank(Minutes,1,127,minuteTensOn,minuteTensOff);

  delay(100);

  updateServoBank(Hours,0,127,hourOnesOn,hourOnesOff);

  delay(100);

  updateServoBank(Hours,1,127,hourTensOn,hourOnesOff);

}

void loop() {
  // put your main code here, to run repeatedly:

   DateTime now = rtc.now();
   motion_detected = digitalRead(ir_pin);

   if (motion_detected == HIGH && !motion_active) {
      motion_time = now;
      move_now = true;
   }

    // Check if motion within last 5 minutes
    if ((now.unixtime() - motion_time.unixtime()) < 60*5) {
      motion_active = true;
    } else {
      motion_active = false;
    }
  

   int m = now.minute();
   int h = now.twelveHour();


   if(h>12){
    h -= 12;
   }
   
  if((currentMinutes != m && motion_active) || move_now){
     char buf1[] = "hh:mm";
    Serial.println(now.toString(buf1));
    
    //Serial.print(" Minutes Ones: ");
    //Serial.println(m%10);
    //Minutes ones
    updateServoBank(Minutes,0,m%10,minuteOnesOn,minuteOnesOff);

    //Minutes tens
    updateServoBank(Minutes,1,m/10,minuteTensOn,minuteTensOff);

    currentMinutes = m;
  }

  if((currentHours != h && motion_active) || move_now){
    //Hours ones
    updateServoBank(Hours,0,h%10,hourOnesOn,hourOnesOff);


    if(h/10 == 0){
      updateServoBank(Hours,1,10,hourTensOn,hourTensOff);
    }else{
    //Hours tens
    updateServoBank(Hours,1,h/10,hourTensOn,hourTensOff);
    }
    currentHours = h;
  }
  
  move_now = false;

  delay(1000);


  
  
  //arduinoTest();


}

void updateServoBank(Adafruit_PWMServoDriver servoDriver, int tens, int numToDisplay, byte onAngle[],byte offAngle[]){
  /*
   * tens is 0,1 depending on digit to update. 0 is minutes and 1 is tens
   */


  //Serial.print("Number: ");
  //Serial.print(numToDisplay);
  //Serial.print(" Binary Expansion: ");
  //for(int i = 0, mask = 1; i < 7; i++, mask = mask << 1){
  for(int i = 0;i<7;i++){
    //if (digits[numToDisplay] & mask){
    if (bitRead(digits[numToDisplay],i) == 1){
        // bit "i" is on
        setAngle(servoDriver,i+8*tens,onAngle[i]);
        Serial.print("1");
    }
    else{
        // bit "i" is off
        setAngle(servoDriver,i+8*tens,offAngle[i]);
        Serial.print("0");
    }
    delay(100); // Delay so all servos don't move at once
  }

  delay(100);
  Serial.println();
   
}






void setAngle(Adafruit_PWMServoDriver pwm, int servoNumber, int angle){

  int constrainedAngle = constrain(angle,0,180);

  pwm.setPWM(servoNumber, 0, map(constrainedAngle,0,180,SERVOMIN,SERVOMAX));
}




void arduinoTest(){
 /*
  * Of the form servoNum1:angle1&servoNum2:angle2&...
  */
   
  char input[INPUT_SIZE + 1];
  byte size = Serial.readBytes(input, INPUT_SIZE);
  // Add the final 0 to end the C string
  input[size] = 0;
  
  // Read each command pair 
  char* command = strtok(input, "&");
  while (command != 0)
  {
      // Split the command in two values
      char* separator = strchr(command, ':');
      if (separator != 0)
      {
          // Actually split the string in 2: replace ':' with 0
          *separator = 0;
          int servoId = atoi(command);
          ++separator;
          int position = atoi(separator);
  
          if(servoId<16){
            
            setAngle(Minutes,servoId,position);
          }else{
            setAngle(Hours,servoId-16,position);
          }

          Serial.print(servoId);
          Serial.print(":");
          Serial.print(position);
          Serial.println();
      }
      // Find the next command in input string
      command = strtok(0, "&");
  }
}
