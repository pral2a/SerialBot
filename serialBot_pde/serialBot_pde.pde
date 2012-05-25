import processing.serial.*;

Serial myPort;

int MESSAGE_BYTES  = 4; // the total bytes in a message
int HEADER_FRAME = '#'; // 33
int HANDSHAKE_FRAME = '!'; // 35
int SERIAL_SPEED = 9600;


int messageCounter = MESSAGE_BYTES-1;
int[] serialBuffer = new int[MESSAGE_BYTES];
int bufferIndex = 0;
boolean readyTX = false;

void setup()
{
  size(100, 100);
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, SERIAL_SPEED);
}

void draw() {
  sendMessage('A', 100);
  delay(2000);
  sendMessage('B', 100);
  delay(2000);
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





void serialEvent(Serial myPort) {
  listenSerial();
}

void listenSerial() {
  try {
    int incommingByte = myPort.read(); 
    if (incommingByte == HANDSHAKE_FRAME) { 
      readyTX = true;
      myPort.clear();
      println("readyTX OK");
      delay(500);
      handShake();
    } 
    else {
      serialBuffer[bufferIndex] = incommingByte;
      bufferIndex++;
      if (bufferIndex > messageCounter) {
        int header = serialBuffer[0];
        if (header == HEADER_FRAME) {
          int command = serialBuffer[1];
          int value = serialBuffer[3] * 256 + serialBuffer[2];
          bufferIndex = 0; // Reset
          runCommand(command, value);
          myPort.clear();
          handShake();
        }
      }
    }
  } 
  catch (Exception e) {
    println("serialEvent failed!");
  }
}
void sendMessage(char command, int value) {
  if (readyTX == false) {
    while (readyTX == false) {
      listenSerial();
    }
  } 
  else {
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
}


void handShake() {
  myPort.write(HANDSHAKE_FRAME);
}

