#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <LiquidCrystal_I2C.h>

/* config */
const char *ssid = "Geo2";

/* init handlers */
WiFiEventHandler stationConnectedHandler;
WiFiEventHandler stationDisconnectedHandler;

/* init led panel */
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  // lcd setup
  lcd.begin();
  lcdSetText("Geocache starten!", "Houd vast!");

  // setup servo to pin d3
  myservo.attach(3);
  
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

}

void onStationConnected(const WiFiEventSoftAPModeStationConnected& evt) {

  // lcd
  lcdSetText("Verbonden", "      Mooi werk!");

}

void onStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt) {
  
  // lcd
  lcdSetText("Doei!", "        Vaarwel!");

}

void lcdSetText(char lineOne[], char lineTwo[]) {

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(lineOne);
  lcd.setCursor(0,1);
  lcd.print(lineTwo);

}

void loop() { }
