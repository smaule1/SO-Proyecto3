#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int btnTask = 3;
int btnSuspend = 4;
int btnRestart = 5;
int btnShutdown = 6;

void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud
  while (!Serial); 
  lcd.init();
  lcd.backlight();
}

int timeout = 0;
int pressed = 0;

void loop() {
  lcd.setCursor(0, 1);
  if (timeout == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    if (pressed != 0) {
      lcd.print("Error");
      pressed = 0;
      timeout = 2000;
    } else {
      lcd.print("Press any button");
    }
  }

  if (pressed == 0) {
    handleButtonPressed();
  } else {
    if (Serial.available() > 0) {
      String str = Serial.readString();
      if (str == "r") {
        printText(pressed);
        pressed = 0;
      }
    }
  }

  lcd.display();
  if(timeout >= 0){
    timeout -= 100;
  }
  delay(100);
}

void handleButtonPressed() {

  if (digitalRead(btnTask) == HIGH) {
    Serial.print("1");
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Button 1 Pressed...");
    timeout = 2000;
    pressed = 1;
  } else if (digitalRead(btnSuspend) == HIGH) {
    Serial.print("2");
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Button 2 Pressed...");
    timeout = 2000;
    pressed = 2;    
  } else if (digitalRead(btnRestart) == HIGH) {
    Serial.print("3");
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Button 3 Pressed...");
    timeout = 2000;
    pressed = 3;
  } else if (digitalRead(btnShutdown) == HIGH) {
    Serial.print("4");
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Button 4 Pressed...");
    timeout = 2000;
    pressed = 4;
  }
}


void printText(int n) {
  if (n == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Open TaskManager");
    lcd.setCursor(0, 1);
    timeout = 2000;
  } else if (n == 2){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PC Suspended");
    lcd.setCursor(0, 1);
    timeout = 2000;
  } else if (n == 3){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Restarting PC");
    lcd.setCursor(0, 1);
    timeout = 2000;
  } else if (n == 4){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Shutting down PC");
    lcd.setCursor(0, 1);
    timeout = 2000;
  } 
}