

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud
  while (!Serial);    // Wait for serial port to be ready 
}

bool connected = false;

void loop() {
  //Establish connection
  if(!connected){
    Serial.print("ping");
    if(Serial.available() > 0){
      String str = Serial.readString();
      connected = str == "pong";
    }    
  }

  delay(100);
}