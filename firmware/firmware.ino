int btnTask = 3;
int btnSuspend = 4;
int btnRestart = 5;
int btnShutdown = 6;

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud
  while (!Serial);    // Wait for serial port to be ready 
}

bool connected = false;

void loop() {
  if(digitalRead(btnTask) == HIGH){
    Serial.print("1");
  } else if (digitalRead(btnSuspend) == HIGH){
    Serial.print("2");
  } else if (digitalRead(btnRestart) == HIGH){
    Serial.print("3");
  } else if (digitalRead(btnShutdown) == HIGH){
    Serial.print("4");
  }
  delay(100);
}
