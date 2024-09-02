#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN         9             // RST


const byte numReaders = 4;            // Anzahl an RFID Lesern
const byte ssPins[] = {2, 3, 4, 5};   // Slave Pins am Arduino
const byte lockPin = A0;
byte readCard[4];                     // Speichert die Karten ID
boolean solved = false;
const String correctIDs[] = {"45CF3C", "4C115D", "4841AD", "461F8C"};

MFRC522 mfrc522[numReaders];           // Array mit MFRC522 Instanzen
Servo lock;

void setup() {
  lock.attach(lockPin);
  Serial.begin(9600);
  while(!Serial);
  SPI.begin();

  for(uint8_t i=0; i<numReaders; i++) {
    mfrc522[i].PCD_Init(ssPins[i], RST_PIN);
    Serial.print(F("Leser an Pin Nr: "));
    Serial.print(String(ssPins[i]));
    Serial.print(F(". Version: "));
    mfrc522[i].PCD_DumpVersionToSerial();     // Infos zum Leser
    delay(100);
  }
  delay(4);
  Serial.println(F("Starklar..."));
}

void loop() {
  solved = true;
  String readIDs[] = {"000000", "000000", "000000", "000000"};
  for(uint8_t i=0; i<numReaders; i++) {
    mfrc522[i].PCD_Init();
    String readRFID = "";
    if(mfrc522[i].PICC_IsNewCardPresent() && mfrc522[i].PICC_ReadCardSerial()) {
      readIDs[i] = dump_byte_array(mfrc522[i].uid.uidByte, 4);
      Serial.print(F("Leser #"));
      Serial.print(String(i+1));
      Serial.print(F(": "));
      for ( uint8_t j = 0; j < 4; j++) { 
        Serial.print(mfrc522[i].uid.uidByte[j], HEX);
      }
      Serial.println("");
      mfrc522[i].PICC_HaltA(); // Stop
    }
  }
  Serial.print(F("Gelesene IDs: "));
  for(uint8_t j = 0; j < 4; j++) {
    Serial.print(readIDs[j]);
    Serial.print(F("  "));
  }
  Serial.println();
  for(uint8_t j = 0; j < numReaders; j++) {
    if(!readIDs[j].equals(correctIDs[j]))
      solved = false;
  }
  /*if(readIDs[0].equals(correctIDs[0]))
    if(readIDs[1].equals(correctIDs[1]))
      if(readIDs[2].equals(correctIDs[2]))
        if(readIDs[3].equals(correctIDs[3]))
          solved = true;*/
  if(solved){
    Serial.println(F("Raetsel geloest!"));
    lock.write(180);
    delay(5000);
  }
  else {
    Serial.println(F("Schloss bleibt zu."));
    lock.write(0);
  }
  delay(200);
}

String dump_byte_array(byte *buffer, byte bufferSize) {
  String strID = "";
  for (byte i = 0; i < bufferSize; i++) {
    //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    //Serial.print(buffer[i], HEX);
    strID += String(buffer[i], HEX);
  }
  strID.toUpperCase();
  return strID;
}