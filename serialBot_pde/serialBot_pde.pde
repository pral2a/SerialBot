import processing.serial.*;

Serial myPort;

public static final int MESSAGE_BYTES  = 4; // the total bytes in a message
public static final char HEADER_FRAME = 'H';
public static final char HANDSHAKE_FRAME = '&'; // placeholder
int serialBuffer[];
int indexBuffer = 0;
//int[] indexBuffer = new int[MESSAGE_BYTES]; 
boolean firstContact = false;


void setup()
{
  size(100, 100);
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  handShake();
}

void draw() {
}

void serialEvent(Serial myPort) {
  int incommingByte = myPort.read();
  if (firstContact == false) {
    if (incommingByte == HANDSHAKE_FRAME) { 
      myPort.clear();         
      firstContact = true; 
      handShake();
    }
  } 
  else {
    serialBuffer[indexBuffer] = incommingByte;
    indexBuffer++;
    if (indexBuffer > MESSAGE_BYTES ) {
      if (incommingByte == HEADER_FRAME) {
        int command = serialBuffer[0];
        int value = serialBuffer[2] * 256 + serialBuffer[1];
        handShake();
        indexBuffer = 0; // Rese
      } 
      else {
//        firstContact = false; tornar a esperar a rebre un handshake per seguir.
        handShake();
      }
    }
  }
}

void sendMessage(char command, int value) {
  myPort.write(HANDSHAKE_FRAME);
  myPort.write(command);
  myPort.write((char)(value / 256));
  myPort.write(value & 0xff);
  //  myPort.write((char)value & 0xff); // check...
}

void handShake() {
  myPort.clear(); // necessari?
  myPort.write(HANDSHAKE_FRAME);
}
