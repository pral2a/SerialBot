import processing.serial.*;

Serial myPort;

public static final int MESSAGE_BYTES  = 4  ; // the total bytes in a message
public static final char HEADER_FRAME = 'H';
public static final char HANDSHAKE_FRAME = '&';


void setup()
{
  size(100, 100);
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
  handShake();
}

void draw() {
  if (myPort.availabe() >= MESSAGE_BYTES) {
    char incomingByte = myPort.read()
      if (incomingByte == HEADER_FRAME) {
        char command = myPort.read();
        int value = myPort.read() * 256 + value;
      } 
      else if (incomingByte == HANDSHAKE_FRAME) {
        sendMessage('A', 180);
      }
  }
}


void sendMessage(char command, int value) {
  myPort.write(HANDSHAKE_FRAME);
  myPort.write(command);
  myPort.write((char)(value / 256));
  myPort.write(value & 0xff);
}

void handShake() {
  myPort.flush(); // necessari?
  myPort.write(HAND_SHAKE);
}

/*

Serial with buffer

byte bufferArray[MESSAGE_BYTES]; // Oju overflow?
char index;

void loop() {

  if (Serial.available()) { 
    bufferArray[index] = Serial.read(); 
    index++;
  } 
  else {  
    index = 0;
  } 
  if ((index > MESSAGE_BYTES)) {
    index = 0;
  } 
  if (bufferArray[0]== HEADER_FRAME)
  {
    // go
  }
}
*/

