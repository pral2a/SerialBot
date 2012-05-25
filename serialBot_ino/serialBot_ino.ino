const int  MESSAGE_BYTES  = 4  ; // the total bytes in a message
const char HEADER_FRAME = '#'; // DEC 33
const char HANDSHAKE_FRAME = '!'; // DEC 35
const long SERIAL_SPEED = 9600;


const int debugLed = 13;

boolean readyTX = false;


void setup()
{
  pinMode(debugLed, OUTPUT);
  Serial.begin(SERIAL_SPEED);
  delay(500);
  handShake();  
}

void loop(){
  //  lifeGuard();
  sendMessage('B', 100);
  delay(1000);
  sendMessage('A', 100);
  delay(1000);
}

void runCommand(int command, int value) {
  switch (command) {
  case 'A':
    // function
    digitalWrite(debugLed, HIGH);
    break;
  case 'B':
    // function 
    digitalWrite(debugLed, LOW);
    break;
  default: 
    // messahe with unknown command
    break;
  }
}



void listenSerial() {
  if ( Serial.available() > 0){
    char incommingByte = Serial.read();
    if (incommingByte == HANDSHAKE_FRAME) { 
      readyTX = true;
      Serial.flush();
    } 
    else if(incommingByte == HEADER_FRAME)
    {
      char command = Serial.read();
      int value = Serial.read() * 256; 
      value = value + Serial.read();
      runCommand(command, value);
      Serial.flush();
      delay(500);
      handShake(); 
    } 
  } 
}

void serialEvent(){
  listenSerial();
}


void handShake(){
  Serial.write(HANDSHAKE_FRAME);
}

void sendBinaryInteger(int value){
  Serial.write(lowByte(value));
  Serial.write(highByte(value));
}

void sendMessage(char command, int value){
  if (readyTX == false){
    while(readyTX == false) {
      listenSerial();
    } 
  }
  else 
  {
    Serial.write(HEADER_FRAME);
    Serial.write(command);
    sendBinaryInteger(value);
    readyTX = false;
  } 
}

boolean readyTXprev = false;
long previousMillis = 0;  

void lifeGuard(){
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > 4000) {
    readyTX = true;
  }
  if (readyTXprev != readyTX) {
    previousMillis = currentMillis;   
  }
  readyTXprev = readyTX;
}

void statusLed() {
  if (readyTX == true) {
    digitalWrite(debugLed, HIGH); 
  } 
  else {
    digitalWrite(debugLed, LOW); 
  }
}





























