/** Voice controlled house lights with Google Assitant
 *  Instructable contest
 *  Mridul Mahajan
 */
/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

 */
#define BLYNK_PRINT Serial
#define EspSerial Serial1

#include<Servo.h> //servo 모터를 위한 헤더
Servo servo_test; //servo 모터 초기화

int angle = 0; //servo를 돌리기 위한 angle 변수

#include <SoftwareSerial.h>
#include<SPI.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// You should get Auth Token in the Blynk App.
char auth[] = "125ser6bd2g39456t985g2s29374se3d"; //servo 모터를 돌리는 아두이노에게 부여된 token

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "juntime2G"; // SSID of your home WiFi
char pass[] = "M940111m"; // password of your home WiFi


SoftwareSerial EspSerial(2, 3); // RX, TX of ESP8266

String s;    //to store incoming text ingredient

BLYNK_WRITE(V0)     // it will run every time a string is sent by Blynk app Blynk어플을 통해 명령을 전달 받음
{
  s=param.asStr();
  Serial.print(s); //string sent by Blynk app will be printed on Serial Monitor
   if(s=="on")
  {
    //서보모터를 90도까지 돌리고 돌아오는 코드
    for(angle = 0; angle < 90; angle += 1)    // command to move from 0 degrees to 180 degrees 
    {                                  
      servo_test.write(angle);                 //command to rotate the servo to the specified angle
      delay(15);                       
    } 
    delay(1000);
    
    for(angle = 90; angle>=1; angle-=5)     // command to move from 180 degrees to 0 degrees 
    {                                
      servo_test.write(angle);              //command to rotate the servo to the specified angle
      delay(5);                       
    } 
    delay(1000);
  }
  else if(s=="off")
  {
    //서보모터를 90도까지 돌리고 돌아오는 코드
    for(angle = 0; angle < 90; angle += 1)    // command to move from 0 degrees to 180 degrees 
    {                                  
      servo_test.write(angle);                 //command to rotate the servo to the specified angle
      delay(15);                       
    } 
    delay(1000);
    
    for(angle = 90; angle>=1; angle-=5)     // command to move from 180 degrees to 0 degrees 
    {                                
      servo_test.write(angle);              //command to rotate the servo to the specified angle
      delay(5);                       
    } 
    delay(1000);    
  }
  else{
    Serial.print("Say on or off");
  }
}
// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

void setup()
{
  servo_test.attach(9); //서보 모터를 9번 디지털 핀에 적
  
  // Debug console
  Serial.begin(9600);
  pinMode(7,OUTPUT);      //Pin 7 is set to output
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
 
  Blynk.begin(auth, wifi, ssid, pass);
}

void loop()
{
  Blynk.run();
}
