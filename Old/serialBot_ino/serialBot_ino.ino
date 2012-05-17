const int  MESSAGE_BYTES  = 4  ; // the total bytes in a message
const char HEADER_FRAME = 'H';
const char HANDSHAKE_FRAME = '&';

void setup()
{
  Serial.begin(9600);
//  handShake();
}

void loop(){
  if ( Serial.available() >= MESSAGE_BYTES)
  {
    char incomingByte = Serial.read();
    if(incomingByte == HEADER_FRAME)
    {
      char command = Serial.read();
      int value = Serial.read() * 256; 
      value = value + Serial.read();
      switch (command) {
      case 'A':
        // function 
        break;
      case 'B':
        // function 
        break;
      default: 
        // messahe with unknown command
        break;
      }
//      handShake(); 
    } 
//    else if (incomingByte == HANDSHAKE_FRAME) {
//      sendMessage('A', 180);
//    }
  }
}



void handShake(){
  Serial.flush(); // necessari?
  Serial.write(HANDSHAKE_FRAME);
}

void sendBinaryInteger(int value){
  Serial.write(lowByte(value));
  Serial.write(highByte(value));
}

void sendMessage(char command, int value){
  Serial.write(HEADER_FRAME);
  Serial.write(command);
  sendBinaryInteger(value);
}





