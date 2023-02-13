/* This code is for Arduino #2.
//inputs:
1) Arduino #2 will read from this light sensor indicator and will trigger a songs that will be played by the Arduino #2 by writing to V2 using webhook.
2) accelerometer sensor - while "dancing".
 
outputs:
1) After the first trigger (from the light sensor of Arduino #1), we will get into "party Mode"
2) while dancing and celebrating life, if  accelerometer sensor will hit a threshold, we will write to V1 (Ardiono #1 is listening) and set (using a webhook) the alarm clock!

In our code, we are writing to the pin V1 on Blynk, the value that we provide this pin, is actually the pin number that we want to trigger.
*/

//Blynk cradentials

#define BLYNK_TEMPLATE_ID "<BLINK_CREDS_HERE>"
#define BLYNK_DEVICE_NAME "<BLINK_CREDS_HERE>"
#define BLYNK_AUTH_TOKEN "<BLINK_CREDS_HERE>"

//Defining some basic Colors for the lights
#define PURPLE 0xA040EA
#define FUXIA 0xEA40DF
#define PINK 0xEA408A
#define RED 0xEA4B40
#define ORANGE 0xEAA040
#define YELLOW 0xDFEA40

#define BLYNK_PRINT SerialUSB

// Notes for the songs and for the "party mode" functions

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

#include <Adafruit_CircuitPlayground.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

#define EspSerial Serial1
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

int melody[] = {

  // Take on me, by A-ha
  // Score available at https://musescore.com/user/27103612/scores/4834399
  // Arranged by Edward Truong

  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  
};

BlynkTimer timer;
bool isPartyMode = false;
int speaker = A0;
int partyModeTimer = -1;

/**
  this function will be called everytime we set the partymode button
Arduino #1 will write to V1 after "falling asleep" and covering the light sensor
*/
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

  if (pinValue == 0) {
    isPartyMode = false;
    Serial.println("Turning off partyMode");
    if (partyModeTimer > -1) {
      timer.deleteTimer(partyModeTimer);
    }
  } else {
    isPartyMode = true;
    Serial.println("Turning on partyMode");
  }
}

void setup() {
  Serial.begin(115200);
  SerialUSB.begin(115200);
  CircuitPlayground.begin();

  EspSerial.begin(ESP8266_BAUD);
  
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);

  CircuitPlayground.setBrightness(255);
  
}

void loop()
{
  Blynk.run();
  timer.run();
  
  if (isPartyMode) {
    playMusic();
  }

    
}


/**
  plays music with light show for the party mode
*/
void playMusic() {
  int tempo = 140;

  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;

  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;

  int colors[] = {PURPLE, FUXIA, PINK, RED, ORANGE, YELLOW};

  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    int randomLight = random(0, 9);
    int randomColor = random(0, 5);
    
    CircuitPlayground.setPixelColor(randomLight, colors[randomColor]);
    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(speaker, melody[thisNote], noteDuration * 0.9);
    

    // Wait for the specief duration before playing the next note.
    sendPartyModeData();
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(speaker);
    CircuitPlayground.clearPixels();
  }
}

/**
  this function send data about how hard you party, when Ardiono #1 hits the threshold,
   it will Write to V2 and trigger the alarm clock!
*/
void sendPartyModeData()
{
  float motion = CircuitPlayground.motionX();

  if (isnan(motion)) {
    return;
  }

  if (motion > 10 || motion < -10) {
    Serial.println("writing to v2");
    Blynk.virtualWrite(V1, 2);
  }
}
