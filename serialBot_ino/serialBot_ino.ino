const int  MESSAGE_BYTES  = 4  ; // the total bytes in a message
const char HEADER_FRAME = '#'; // DEC 33
const char HANDSHAKE_FRAME = '!'; // DEC 35
const long SERIAL_SPEED = 57600;


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
    } 
    else if(incommingByte == HEADER_FRAME)
    {
      char command = Serial.read();
      int value = Serial.read() * 256; 
      value = value + Serial.read();
      runCommand(command, value);
      handShake(); 
    } 
    Serial.flush();
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
  while (readyTX == true) {
    Serial.write(HEADER_FRAME);
    Serial.write(command);
    sendBinaryInteger(value);
    readyTX = false;
  }
  listenSerial();
}

//void sendMessage(char sendCommand, int sendValue){
//  if (readyTX == true) {
//    Serial.write(HEADER_FRAME);
//    Serial.write(sendCommand);
//    sendBinaryInteger(sendValue);
//    readyTX = false;
//  } 
//  else {
//    listenSerial();
//    delay(100);
//    sendMessage(sendCommand, sendValue);
//  }
//}


void statusLed() {
  if (readyTX == true) {
    digitalWrite(debugLed, HIGH); 
  } 
  else {
    digitalWrite(debugLed, LOW); 
  }
}





















