/*
  BluetoothAT_test: tests the  Bluetooth Shield v2.2
  Settings: Baud=38400, data bits: 8, stop bits: 1, parity none, flow control none
  Connection BT-Arduino: Tx - Rx3, Rx - Tx3

*/

int wt=0; // wait time between commands

void printBlue(char *str){
  Serial.print(str);
  Serial3.print(str);
}

void getResponse(char *response){
  while(Serial3.available()) {
    int inByte = Serial3.read();
    Serial.write(inByte);
    *response++=inByte;
 }
 *response='\0';
}

char *sendCMD(char *cmd){
  static char response[128];
  printBlue(cmd);
  Serial.print("wait for response ... ");
  waitResponse();
  Serial.println("get response ... ");
  getResponse(response);
  return response;
}
void waitResponse(){
  while(!Serial3.available())
    delay(100);
}

void printState(){
  Serial.println("*******************");
  Serial.println("*** print state ***");
  Serial.println("*******************");
  sendCMD("AT+name?\r\n");
  delay(wt);
  sendCMD("AT+version?\r\n");
  delay(wt);  
  sendCMD("AT+ROLE?\r\n");
  delay(wt); 
  sendCMD("AT+UART?\r\n");
  delay(wt);
  sendCMD("AT+cmode?\r\n");
  delay(wt);
  sendCMD("AT+state?\r\n");
  delay(wt);
  sendCMD("AT+IAC?\r\n");
  delay(wt);
  sendCMD("AT+CLASS?\r\n");
  delay(wt);
  sendCMD("AT+INQM?\r\n");
  delay(wt);
  Serial.println("********** end print state **********");
}

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(38400);
  Serial3.begin(38400);
  Serial.println("\n\n\n");
  Serial.println("********** Start new session **********");
}

// the loop routine runs over and over again forever:
void loop() {
  char *response=NULL;
 
  sendCMD("AT\r\n");
  delay(wt);
  sendCMD("AT+ORGL\r\n");
  delay(wt); 
  sendCMD("AT+reset\r\n");
  delay(wt<1000?1000:wt); // after reset need to wait at least 1s 

  // print state after reset
  printState();

  sendCMD("AT+name=SMA-Solar1\r\n");
  delay(wt); 
  sendCMD("AT+pswd=0000\r\n");
  delay(wt);
  // master mode
  sendCMD("AT+ROLE=1\r\n");
  delay(wt);
  // forget existing pairing
  sendCMD("AT+rmaad\r\n");
  delay(wt);
  sendCMD("AT+UART=38400,0,0\r\n"); // 9600 is original and works also
  delay(wt);
  sendCMD("AT+cmode=1\r\n");
  delay(wt);
  sendCMD("AT+state?\r\n");
  delay(wt);
  sendCMD("AT+INIT\r\n");  
  delay(wt);
  sendCMD("AT+state?\r\n");
  delay(wt);
  sendCMD("AT+IAC=9e8b33\r\n");
  delay(wt);
  // find out class: Download Android "Bluetooth Scannµer"
  // scan for the Inverter
  // Device major class code: dez. 7936 = 0x1F00
  sendCMD("AT+CLASS=1F00\r\n");
  // sendCMD("AT+CLASS=0200\r\n");
  delay(wt);
  sendCMD("AT+INQM=1,1,20\r\n");
  delay(wt);

  sendCMD("AT+INQ\r\n");
  delay(wt);

  for (int i=0; i<1000; i++) {
    char response[128];
    Serial.println(i);
    // sendCMD("AT+STATE?\r\n");
    // delay(100);        // delay in between reads for stability
    waitResponse();
    getResponse(response);
    delay(wt);
  }
  Serial.print("end INQ");
  
  

  delay(200000);
}
