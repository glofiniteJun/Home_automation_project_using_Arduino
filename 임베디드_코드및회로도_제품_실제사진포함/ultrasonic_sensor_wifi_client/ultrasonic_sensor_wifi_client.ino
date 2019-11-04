/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 */ 
 
#include <SPI.h>
#include <ESP8266WiFi.h>

// defines ultrasonic1 pins numbers
const int trigPin1 = 9;
const int echoPin1 = 10;

// defines ultrasonic2 pins numbers
const int trigPin2 = 9;
const int echoPin2 = 10;

// defines ultrasonic1 variables
long duration1;
int distance1;
// defines ultrasonic2 variables
long duration2;
int distance2;

int check1 = 0; //첫번째 센서가 반응할때
int check2 = 0; //두번째 센서가 반응할때
int enter_sig; //들어가는 시그널
int exit_sig; //나가는 시그널

byte ledPin = 2;
const char* ssid = "juntime2G"; // SSID of your home WiFi
const char* password = "M940111m"; // password of your home WiFi

unsigned long askTimer = 0;

IPAddress server(192,168,0,80);       // the fix IP address of the server
WiFiClient client;

void setup() {
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output for ultrasonic1
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input for ultrasonic1
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output for ultrasonic2
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input for ultrasonic2
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
  // Clears the trigPin
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin1, HIGH);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration1 = pulseIn(echoPin1, HIGH);
  duration2 = pulseIn(echoPin2, HIGH);
  // Calculating the distance
  distance1= duration1*0.034/2;
  distance2= duration2*0.034/2;

  if(distance1 < 40 && check1 == 1 && check2 == 0){ //1번째 센서가 먼저 반응하면 exit
      exit_sig = 1;
  }
  if(distance2 < 40 check1 == 0 && check2 == 1){ //2번째 센서가 먼저 반응하면 enter
      enter_sig = 1;
  }
  
  if(exit_sig){ //exit 시에 server에게 시그널 보내기
    client.connect(server, 80);   // Connection to the server
    digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
    Serial.println(".");
    client.println("1\r");  // sends the message to the server when enter room
    String answer = client.readStringUntil('\r');   // receives the answer from the sever
    Serial.println("from server: " + answer);
    client.flush();
    digitalWrite(ledPin, HIGH);
    delay(2000);                  // client will trigger the communication after two seconds
    break;
    }
  if(exit_sig){ //enter 시에 server에게 시그널 보내기
    client.connect(server, 80);   // Connection to the server
    digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
    Serial.println(".");
    client.println("2\r");  // sends the message to the server when exit room
    String answer = client.readStringUntil('\r');   // receives the answer from the sever
    Serial.println("from server: " + answer);
    client.flush();
    digitalWrite(ledPin, HIGH);
    delay(2000);                  // client will trigger the communication after two seconds
    break;
    }
  } 
}
