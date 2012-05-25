#include <SoftwareSerial.h>

const int  MESSAGE_BYTES  = 4  ; // the total bytes in a message
const char HEADER_FRAME = '#'; // DEC 33
const char HANDSHAKE_FRAME = '!'; // DEC 35
const long SERIAL_SPEED = 9600;

SoftwareSerial mySerial(2, 3);

const int debugLed = 13;

int messageCounter = MESSAGE_BYTES-1;
int serialBuffer[MESSAGE_BYTES];
int bufferIndex = 0;
boolean readyTX = false;


void setup()
{
  pinMode(debugLed, OUTPUT);
  Serial.begin(SERIAL_SPEED);
  handShake(); 
  mySerial.begin(4800);
  mySerial.println("Hello"); 
}

long previousMillis = 0;
long previousMillis2 = 0;   

void loop(){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > 1000) {
      previousMillis = currentMillis;   
      sendMessage('A', 100);
    }
    unsigned long currentMillis2 = millis();
    if(currentMillis2 - previousMillis2 > 2000) {
      previousMillis2 = currentMillis2;   
      sendMessage('B', 100);
    }
  //  lifeGuard();


}

void runCommand(int command, int value) {
  switch (command) {
  case 'A':
    // function
    //    Serial.print('A');
    mySerial.print("RUN function: A");
    mySerial.println(value);
    digitalWrite(debugLed, HIGH);
    break;
  case 'B':
    // function 
    //    Serial.print('B');
    mySerial.print("RUN function: B");
    mySerial.println(value);
    digitalWrite(debugLed, LOW);
    break;
  default: 
    // messahe with unknown command
    break;
  }
}


//void listenSerial() {
//  if ( Serial.available() > 0){
//    char incommingByte = Serial.read();
//    if (incommingByte == HANDSHAKE_FRAME) { 
//      readyTX = true;
//      Serial.flush();
//       mySerial.println("handshake RECEIVED");
//    } 
//    else if(incommingByte == HEADER_FRAME)
//    {
//      mySerial.print(incommingByte);
//      char command = Serial.read();
//      int value = Serial.read() * 256; 
//      value = value + Serial.read();
//      runCommand(command, value);
//      mySerial.print(command);
//      mySerial.println(value);
//      Serial.flush();
//      delay(500);
//      handShake(); 
//    } 
//  } 
//}

void listenSerial() {
  if ( Serial.available() > 0) {
    int incommingByte = Serial.read(); 
    if (incommingByte == HANDSHAKE_FRAME && bufferIndex <= 0) { 
      mySerial.println('HANDSHAKE');
      readyTX = true;
      Serial.flush();
      bufferIndex = 0;
      //        println("readyTX OK");
    } 
    else {
      serialBuffer[bufferIndex] = incommingByte;
      bufferIndex++;
      mySerial.println(bufferIndex);
      if (bufferIndex > messageCounter) {
        int header = serialBuffer[0];
        if (header == HEADER_FRAME) {

          char command =  serialBuffer[1];
          int value = serialBuffer[2] * 256; 
          value = value + serialBuffer[3];
          bufferIndex = 0; // Reset
          runCommand(command, value);
          mySerial.print(command);
          mySerial.println(value);
          Serial.flush();
          delay(200);
          handShake();
        }
      }
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
  while(readyTX == false) {
    listenSerial();
  } 
  Serial.write(HEADER_FRAME);
  Serial.write(command);
  sendBinaryInteger(value);
  readyTX = false;
}

//boolean readyTXprev = false;
//long previousMillis = 0;  
//
//void lifeGuard(){
//  unsigned long currentMillis = millis();
//  if(currentMillis - previousMillis > 4000) {
//    readyTX = true;
//  }
//  if (readyTXprev != readyTX) {
//    previousMillis = currentMillis;   
//  }
//  readyTXprev = readyTX;
//}

void statusLed() {
  if (readyTX == true) {
    digitalWrite(debugLed, HIGH); 
  } 
  else {
    digitalWrite(debugLed, LOW); 
  }
}









































