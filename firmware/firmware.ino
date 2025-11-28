#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3f, 16, 2);

int btnTask = 3;
int btnSuspend = 4;
int btnRestart = 5;
int btnShutdown = 6;

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud
  while (!Serial);    // Wait for serial port to be ready 
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Feliz Jueves :)");
  lcd.setCursor(0,1);
  lcd.print("");
}

bool connected = false;

void loop() {  
  lcd.setCursor(0,1);
  if(digitalRead(btnTask) == HIGH){
    Serial.print("1");
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("Open TaskManager");
    lcd.setCursor(0,1);
    lcd.print("Button 1 Pressed");
  } else if (digitalRead(btnSuspend) == HIGH){
    lcd.clear();

    Serial.print("2");
    lcd.setCursor(0,0);
    lcd.print("PC suspended");
    lcd.setCursor(0,1);
    lcd.print("Button 2 Pressed");
  } else if (digitalRead(btnRestart) == HIGH){
    Serial.print("3");
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Restarting PC");
    lcd.setCursor(0,1);
    lcd.print("Button 3 Pressed");
  } else if (digitalRead(btnShutdown) == HIGH){
    Serial.print("4");
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Shutting down PC");
    lcd.setCursor(0,1);
    lcd.print("Button 4 Pressed");
  }
  lcd.display();
  delay(100);
}
