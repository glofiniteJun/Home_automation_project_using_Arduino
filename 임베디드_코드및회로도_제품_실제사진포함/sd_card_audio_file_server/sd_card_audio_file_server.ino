#include "SD.h" //Lib to read SD card
#include "TMRpcm.h" //Lib to play auido
#include "SPI.h" //SPI lib for SD card

#define SD_ChipSelectPin 4 //Chip select is pin number 4
TMRpcm music; //Lib object is named "music"

int song_number=0;
boolean play_pause;int 



#include <ESP8266WiFi.h>
const char* ssid = "juntime2G"; // SSID of your home WiFi
const char* password = "M940111m"; // password of your home WiFi
#define MAX_CLIENTS 10
#define MAX_LINE_LENGTH 50
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
WiFiClient *clients[MAX_CLIENTS] = { NULL };

IPAddress ip(192, 168, 0, 80);            // IP address of the server
IPAddress gateway(192,168,0,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

char inputs[MAX_CLIENTS][MAX_LINE_LENGTH] = { 0 };

void setup() {
  music.speakerPin = 9; //Auido out on pin 9
  Serial.begin(9600); //Serial Com for debugging 
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    return;
  }
  music.setVolume(5);    //   0 to 7. Set volume level
  music.quality(1);        //  Set 1 for 2x oversampling Set 0 for normal
  //music.volume(0);        //   1(up) or 0(down) to control volume
  //music.play("filename",30); plays a file starting at 30 seconds into the track   



  Serial.begin(115200); // only for debug
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, password);             // connects to the WiFi router

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  
  // Check if a new client has connected
  WiFiClient newClient = server.available();
  if (client) {
    Serial.println("new client");
    // Find the first unused space
    for (int i=0 ; i<MAX_CLIENTS ; ++i) {
        if (NULL == clients[i]) {
            clients[i] = new WiFiClient(newClient);
            break;
        }
     }
  }

  // Check whether each client has some data
  for (int i=0 ; i<MAX_CLIENTS ; ++i) {
    // If the client is in use, and has some data...
    if (NULL != clients[i] && clients[i]->available() ) {
      // Read the data 
      char newChar = clients[i]->read();
      // If we have the end of a string
      // (Using the test your code uses)
      if ('\r' == newChar) { //when client's data is end of a strinng
        // Blah blah, do whatever you want with inputs[i]
        //여기다 이제 input에 따른 동작 작성
        if(inputs[i] == "1")
        {music.play("enter_room.wav");} //Play song 1 : enter_아두이노
        if(inputs[i] == "2")
        {music.play("exit_room.wav");} //Play song 2 : exit_아두이노
        if(inputs[i] == "3")
        {music.play("dust.wav");} //Play song 3 : 미세먼지_아두이노
        if(inputs[i] == "4")
        {music.play("bed_time.wav");} //Play song 4 : 침대_압력센서_아두이노
        if(inputs[i] == "5")
        {music.play("studying_time.wav");} //Play song 5 : 공부_압력센서_아두이노
        // Empty the string for next time
        inputs[i][0] = NULL;

        // The flush that you had in your code - I'm not sure
        // why you want this, but here it is
        clients[i]->flush();

        // If you want to disconnect the client here, then do this:
        clients[i]->stop();
        delete clients[i];
        clients[i] = NULL;

      } else {
        // Add it to the string
        strcat(inputs[i], newChar);
        // IMPORTANT: Nothing stops this from overrunning the string and
        //            trashing your memory. You SHOULD guard against this.
        //            But I'm not going to do all your work for you :-)
      }
    }
  }

}
