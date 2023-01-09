//HW; WEMOS
//Wemos + LCD ca. 130mA | with servo; 600mA (!)

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

/* config */
const char *ssid = "Geo2";

#define SERVO D3 // Servo @ D3 (GPIO0)
#define BUZZER D5 // Piezospeaker @ D5 (GPIO14) | D0 (GPIO16) and D4 (GPIO2) causes failure during boot (?)

/* init handlers */ // Don't use Delay() within event handler 
WiFiEventHandler stationConnectedHandler;
WiFiEventHandler stationDisconnectedHandler;

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

  // start serial session
  Serial.begin(115200);
  
  // start wifi AP with event handlers
  WiFi.softAP(ssid);
  stationConnectedHandler = WiFi.onSoftAPModeStationConnected(&onStationConnected);
  stationDisconnectedHandler = WiFi.onSoftAPModeStationDisconnected(&onStationDisconnected);

  // wait
  delay(2000);
  
  // lcd
  lcdSetText("Geocache actief", "(Jouw beurt!)");
  playSound(YOURTURN); 
}

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
    tone(BUZZER, 250, 80); //pin, freq, duration
    delayLoop (150);
    tone(BUZZER, 250, 80); 
    delayLoop (150);
    tone(BUZZER, 250, 80); 
    delayLoop (150);
    tone(BUZZER, 200, 500);
    delayLoop (1000);
    break;

   case YOURTURN:
    tone(BUZZER, 280, 80); //pin, freq, duration
//    delay (150); // Delay can't used in event handler (causes crashes)
    delayLoop(150);
    tone(BUZZER, 250, 500);
    delayLoop (1000);
    break; 

   case GOTIT:
    tone(BUZZER, 300, 80); //pin, freq, duration
    delayLoop(150);
    tone(BUZZER, 330, 500);
    delayLoop (1000);
    break; 
    
  case BYEBYE:
    tone(BUZZER, 200, 80); //pin, freq, duration
    delayLoop (300);
    tone(BUZZER, 190, 80); 
    delayLoop (300);
    tone(BUZZER, 180, 80); 
    delayLoop (300);
    tone(BUZZER, 170, 750);
    delayLoop (1000);
    break;
  }

}

void controlServo (uint8_t servoPosition)
{
switch (servoPosition) {  
    case OPEN:
     myservo.write(180);
      break;

    case CLOSE:
      myservo.write(0);
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


