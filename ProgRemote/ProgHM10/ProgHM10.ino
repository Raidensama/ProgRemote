
#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(11, 10); // (RX, TX) (pin Rx BT, pin Tx BT)
const int key = 7;
void setup()
{
    pinMode(key, OUTPUT);
    digitalWrite(key, LOW);
    
    mySerial.begin(9600); //Baud rate à synchroniser avec le HM10
    Serial.begin(9600); //Baud rate à synchroniser avec le HM10
    
    digitalWrite(key, HIGH);
    Serial.println("reçu:");
    
   mySerial.write("AT+ADDR?"); // module adress
   delay(200);
  
    mySerial.write("AT+ROLE?"); // 0=slave and 1=master
    delay(200);

    mySerial.write("AT+BAUD?"); // 0=9600 and 4=115200
    delay(200);
    
}
 
void loop() // run over and over
{
  if (mySerial.available())
   
    Serial.write(mySerial.read());
    
    if (Serial.available())

    mySerial.write(Serial.read());

     
}

