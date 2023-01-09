//HW; ATTiny85
//ATTiny85 + LCD ca. 60mA

//#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <TimerFreeTone.h> //Doesn't use timers avoiding conflicts with other libraries - see https://bitbucket.org/teckel12/arduino-timer-free-tone/wiki/Home

/* config */
//const char *ssid = "Geo2";

//#define SERVO D3 // Servo @ D3 (GPIO0)
//#define BUZZER D5 // Piezospeaker @ D5 (GPIO14) | D0 (GPIO16) and D4 (GPIO2) causes failure during boot (?)
#define SERVO 1 // Servo @ PB1
#define BUZZER 3 // Piezospeaker @ PB3


/* init handlers */ // Don't use Delay() within event handler 
//WiFiEventHandler stationConnectedHandler;
//WiFiEventHandler stationDisconnectedHandler;

/* init led panel */
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Instantiate servo
Servo myservo; 

#define WELCOME 1 // welcome sound
#define YOURTURN 2  
#define GOTIT 3 
#define BYEBYE 4 

#define OPEN 1
#define CLOSE 2

void setup() {
  
  // lcd setup
  lcd.begin();
  lcdSetText("Geocache starten!", "Houd vast!");
  playSound(WELCOME); 
  
  myservo.attach(SERVO);  // attached servo  
  myservo.write(0);  // servo to closed position
  // wait
  delay(1000);
  controlServo (OPEN);
  // start serial session
//  Serial.begin(115200);
  
  // start wifi AP with event handlers
//  WiFi.softAP(ssid);
//  stationConnectedHandler = WiFi.onSoftAPModeStationConnected(&onStationConnected);
//  stationDisconnectedHandler = WiFi.onSoftAPModeStationDisconnected(&onStationDisconnected);

  // wait
  delay(2000);
  
  // lcd
  lcdSetText("Geocache actief", "(Jouw beurt!)");
  playSound(YOURTURN); 
  controlServo (CLOSE);
}

/*
void onStationConnected(const WiFiEventSoftAPModeStationConnected& evt) {

  // lcd
  lcdSetText("Verbonden", "      Mooi werk!");
  playSound(GOTIT); 
  controlServo (OPEN);
}

void onStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt) {
  
  // lcd
  lcdSetText("Doei!", "        Vaarwel!");
  playSound(BYEBYE);
  controlServo (CLOSE);
}
*/

void lcdSetText(char lineOne[], char lineTwo[]) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(lineOne);
  lcd.setCursor(0,1);
  lcd.print(lineTwo);
}

void loop() { }

void playSound (uint8_t sound){
  switch (sound) {
    case  WELCOME:
    //tatatadaa welcome
    TimerFreeTone(BUZZER, 250, 50); //pin, freq, duration
    delayLoop (150);
    TimerFreeTone(BUZZER, 250, 50); 
    delayLoop (150);
    TimerFreeTone(BUZZER, 250, 50); 
    delayLoop (150);
    TimerFreeTone(BUZZER, 200, 500);
    delayLoop (1000);
    break;

   case YOURTURN:
    TimerFreeTone(BUZZER, 280, 50); //pin, freq, duration
//    delay (150); // Delay can't used in event handler (causes crashes)
    delayLoop(150);
    TimerFreeTone(BUZZER, 250, 500);
    delayLoop (1000);
    break; 

   case GOTIT:
    TimerFreeTone(BUZZER, 300, 80); //pin, freq, duration
    delayLoop(150);
    TimerFreeTone(BUZZER, 330, 500);
    delayLoop (1000);
    break; 
    
  case BYEBYE:
    TimerFreeTone(BUZZER, 200, 80); //pin, freq, duration
    delayLoop (300);
    TimerFreeTone(BUZZER, 190, 80); 
    delayLoop (300);
    TimerFreeTone(BUZZER, 180, 80); 
    delayLoop (300);
    TimerFreeTone(BUZZER, 170, 750);
    delayLoop (1000);
    break;
  }

}

void controlServo (uint8_t servoPosition)
{
switch (servoPosition) {  
    case OPEN:
     myservo.write(175);
      break;

    case CLOSE:
      myservo.write(5);
      break;
  }
}

// The Delay function can't used in event handler (causes crashes) - while loop is ok
void delayLoop(unsigned long milliSeconds)
{
   const unsigned long start = millis();
   while (true)
   {
       unsigned long now = millis();
       if (now - start >= milliSeconds) return;
   }
}


