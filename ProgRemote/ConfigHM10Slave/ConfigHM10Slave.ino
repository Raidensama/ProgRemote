
#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(11, 10); // (RX, TX) (pin Rx BT, pin Tx BT)
const int key = 7;
void setup()
{
    pinMode(key, OUTPUT);
    digitalWrite(key, LOW);
    
    mySerial.begin(9600); // communication HM-10
    Serial.begin(9600); // moniteur série
    
    digitalWrite(key, HIGH);
    Serial.println("reçu:");
    
  
    mySerial.write("AT+ROLE0"); // 0=slave(default) and 1=master
    delay(200);

    mySerial.write("AT+BAUD4"); // 0=9600 (default) and 4=115200
    delay(200);
   
    
}
 
void loop() // run over and over
{
  if (mySerial.available())
   
    Serial.write(mySerial.read());
    
    if (Serial.available())

    mySerial.write(Serial.read());

     
}

