/* This code is for Arduino #3.
//inputs:
1) Arduino #3 will read from this temp sensor indicator and will trigger a a function that turn off the TV the Arduino #3 by writing to V3 using webhook.
2) sound sensor - while "Shouting" on the sleeping friend.
 
outputs:
1) After the first trigger (from the temp sensor of Arduino #1), we will send an IR signal that shut down the TV
2) the sound sensor will hit the threshold and will triger an IR signal that will turn on the AC and drop a glass of water on the sleeping friends head!
In our code, we are writing to the pin V1 on Blynk, the value that we provide this pin, is actually the pin number that we want to trigger.
*/

//IR protocols Of the TV and AC using NECx protocol
//* I used the Infrared_Read.ino Example sketch for IRLib2 and Circuit Playground Express in order to "read" the specific IR signals and decode them.
// later on i have used Infrared_Send.ino Example sketch for IRLib2 and Circuit Playground Express in order to use them
#define MY_PROTOCOL NECX
#define MY_BITS 32
#define MY_AC_POWER 0xE0E040BF
#define MY_TV_POWER 0xFF480D



// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "<BLINK_CREDS_HERE>"
#define BLYNK_DEVICE_NAME "<BLINK_CREDS_HERE>"
#define BLYNK_AUTH_TOKEN "<BLINK_CREDS_HERE>"

// Comment this out to disable prints and save space
#define BLYNK_PRINT SerialUSB

#include <Adafruit_CircuitPlayground.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);
BlynkTimer timer;
bool isColdMode = false;
bool Tv_turned_off = false;
bool AC_turned_off = false;
int speaker = A0;
int TurnOffTheTvMode = -1;
int TurnOnTheACMode = -1;
int soundTimer = -1;

void setup()
{
  CircuitPlayground.begin();
  Serial.begin(115200);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);
  CircuitPlayground.setBrightness(255);
    //Setting a timer that samples the sound sensor

  soundTimer = timer.setInterval(5000L, sendSoundData);
}

void loop()
{
Blynk.run();
timer.run();
}
/**
  this function will be called when Arduino #1 temp sensor will hit a certain thershold
*/

BLYNK_WRITE(V3)
{
  Serial.println("someone wrote to V3");
  int pinValue = param.asInt(); // assigning incoming value from pin V2 to a variable
  if (pinValue == 0) {
    isColdMode = false;
    Serial.println("Turning off isColdMode");
  } else {
    isColdMode = true;
    Serial.println("Turning on isColdMode and Turning off the TV");
    // Sending an IR signal that Turn Off this specific TV (Samsung TV)
    CircuitPlayground.irSend.send(MY_PROTOCOL,MY_TV_POWER,MY_BITS);
    Tv_turned_off = true;
  }
}

void sendSoundData(){
  int soundPressureLevel = CircuitPlayground.mic.soundPressureLevel(50);
  Serial.println("Sound metric is - ");
  Serial.println(soundPressureLevel);
  //here we check the sound threshold only after the TV is off. we are using a flag in order to maintain the order.
  if (soundPressureLevel > 75 && Tv_turned_off) {
    delay(50);
    Serial.println("Tv_turned_off ");
    delay(50);
  // Sending an IR signal that Turn on the AC
    CircuitPlayground.irSend.send(MY_PROTOCOL,MY_AC_POWER,MY_BITS);
    AC_turned_off = true;
    timer.deleteTimer(soundTimer);
  }
}