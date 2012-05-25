import processing.serial.*;

Serial myPort;

int MESSAGE_BYTES  = 4; // the total bytes in a message
int HEADER_FRAME = '#'; // 33
int HANDSHAKE_FRAME = '!'; // 35
int SERIAL_SPEED= 9600;


int messageCounter = MESSAGE_BYTES-1;
int[] serialBuffer = new int[MESSAGE_BYTES];
int bufferIndex = 0;
boolean readyTX = false;

void setup()
{
  size(100, 100);
  //    String portName = "master";
  println(Serial.list());
  String portName = Serial.list()[2];
  myPort = new Serial(this, portName, SERIAL_SPEED);
}

long previousMillis = 0;
long previousMillis2 = 0;   

void draw() {
  listenSerial();
  long currentMillis = millis();
  if (currentMillis - previousMillis > 5000) {
    previousMillis = currentMillis;   
    sendMessage('A', 100);
  }
  long currentMillis2 = millis();
  if (currentMillis2 - previousMillis2 > 3000) {
    previousMillis2 = currentMillis2;   
    sendMessage('B', 100);
  }
}

void runCommand(int command, int value) {
  switch (command) {
  case 'A':
    // function
    println("RUN function: A" + value);
    break;
  case 'B':
    // function 
    println("RUN function: B" + value);
    break;
  default: 
    // message with unknown command
    break;
  }
}


//void serialEvent(Serial myPort) {
//  listenSerial();
//}

void listenSerial() {
  try {
    if ( myPort.available() > 0) {
      int incommingByte = myPort.read(); 
      if (incommingByte == HANDSHAKE_FRAME && bufferIndex == 0) { 
        readyTX = true;
        myPort.clear();
        println("readyTX OK");
        //        delay(500);
        //        handShake();
      } 
      else {
        serialBuffer[bufferIndex] = incommingByte;
        bufferIndex++;
        println("bufferIndex: " + bufferIndex);
        if (bufferIndex > messageCounter) {
          int header = serialBuffer[0];
          if (header == HEADER_FRAME) {
            int command = serialBuffer[1];
            int value = serialBuffer[3] * 256 + serialBuffer[2];
            println("bufferRset: ");
            bufferIndex = 0; // Reset
            runCommand(command, value);
            myPort.clear();
            delay(200);
            handShake();
          }
        }
      }
    } 
    else {
      handShake();
    }
  }
  catch (Exception e) {
    println("serialEvent failed!");
  }
}
void sendMessage(char command, int value) {
  while (readyTX == false) {
    listenSerial();
  }
  myPort.write(HEADER_FRAME);
  myPort.write(command);
  myPort.write((char)(value / 256));
  myPort.write(value & 0xff);
  print("SEND function:");
  print(command);
  print(" ");
  println(value);
  readyTX = false;
  println("readyTX NO");
}


void handShake() {
  myPort.write(HANDSHAKE_FRAME);
}

