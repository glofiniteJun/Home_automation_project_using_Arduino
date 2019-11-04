/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include <SPI.h>
#include <ESP8266WiFi.h>

//미세먼지 센서에 필요한 데이터들
int measurePin = A6;
int ledPower = 2;
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;



byte ledPin = 2;
const char* ssid = "juntime2G"; // SSID of your home WiFi
const char* password = "M940111m"; // password of your home WiFi

unsigned long askTimer = 0;

IPAddress server(192,168,0,80);       // the fix IP address of the server
WiFiClient client;

void setup() {
  //미세먼지 센서를 위한 setup 이하 2줄
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  
  Serial.begin(115200);               // only for debug
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  pinMode(ledPin, OUTPUT);
}

void loop () {

  //미세먼지 센서에서 미세먼지 농도 계산
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
 
  voMeasured = analogRead(measurePin); // read the dust value
 
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
 
  // 0 - 3.3V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (3.3 / 1024);
 
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = 0.17 * calcVoltage - 0.1;//최종적인 미세먼지 값 받기

  if(dustDensity >=250){ //미세먼지 농도가 250이상이면 server 아두이노에 메세지 보내기
    client.connect(server, 80);   // Connection to the server
    digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
    Serial.println(".");
    client.println("3\r");  // sends the message to the server 3번 음악 파일 재생을 trigger
    String answer = client.readStringUntil('\r');   // receives the answer from the sever
    Serial.println("from server: " + answer);
    client.flush();
    digitalWrite(ledPin, HIGH);
    delay(2000);                  // client will trigger the communication after two seconds
  }
}
