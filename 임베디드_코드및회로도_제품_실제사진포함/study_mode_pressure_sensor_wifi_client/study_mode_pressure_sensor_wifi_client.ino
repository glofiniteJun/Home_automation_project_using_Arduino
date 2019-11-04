/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include <SPI.h>
#include <ESP8266WiFi.h>

const int FSR_PIN1 = A0; //압력 센서1와 연결된 
const int FSR_PIN2 = A1; //압력 센서2와 연결된 

byte ledPin = 2;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "juntime2G"; // SSID of your home WiFi
char pass[] = "M940111m"; // password of your home WiFi// Your WiFi credentials.

unsigned long askTimer = 0;

IPAddress server(192,168,0,80);       // the fix IP address of the server
WiFiClient client;

void setup() {
  //압력 센서를 동작시키는 작업에 대한 설정 이하 3줄
  pinMode(FSR_PIN1, INPUT);
  pinMode(FSR_PIN2, INPUT);
  Serial.begin(9600); // Starts the serial communication
  
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
  int fsrADC1 = analogRead(FSR_PIN1); //압력 센서1에서 값을 읽어옴//압력 센서에서 값을 읽어옴
  int fsrADC2 = analogRead(FSR_PIN2); //압력 센서2에서 값을 읽어옴//압력 센서에서 값을 읽어옴
  {
    // Use ADC reading to calculate voltage:
    float fsrV1 = fsrADC1 * VCC / 1023.0;
    float fsrV2 = fsrADC2 * VCC / 1023.0;
    // calculate FSR resistance 압력센서1:
    float fsrR1 = R_DIV * (VCC / fsrV1 - 1.0);
    float force1;
    float fsrG1 = 1.0 / fsrR1; // Calculate conductance
    // calculate FSR resistance 압력센서2:
    float fsrR2 = R_DIV * (VCC / fsrV2 - 1.0);
    float force2;
    float fsrG2 = 1.0 / fsrR2; // Calculate conductance
    if (fsrR1 <= 600) //압력센서1 최종적인 force값 계산
      force1 = (fsrG1 - 0.00075) / 0.00000032639;
    else
      force1 =  fsrG1 / 0.000000642857;
    if (fsrR2 <= 600) //압력센서2 최종적인 force값 계산
      force2 = (fsrG2 - 0.00075) / 0.00000032639;
    else
      force2 =  fsrG2 / 0.000000642857;
    if((force1 + force2) > 100){ //만약이 사람이 가하는 압력이 총 100이 넘으면 앉았다고 판단하여 server에게 메세지를 보내는 if문
      client.connect(server, 80);   // Connection to the server
      digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
      Serial.println(".");
      client.println("5\r");  // sends the message to the server 5번 음악 파일 재생을 trigger//
      String answer = client.readStringUntil('\r');   // receives the answer from the sever
      Serial.println("from server: " + answer);
      client.flush();
      digitalWrite(ledPin, HIGH);
      delay(2000);                  // client will trigger the communication after two seconds
    }
  }
}
