#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9             // RST


const byte numReaders = 4;            // Anzahl an RFID Lesern
const byte ssPins[] = {2, 3, 4, 5};   // Slave Pins am Arduino
byte readCard[4];                     // Speichert die Karten ID
MFRC522 mfrc522[numReaders];           // Array mit MFRC522 Instanzen

void setup() {
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
  for(uint8_t i=0; i<numReaders; i++) {
    mfrc522[i].PCD_Init();
    String readRFID = "";
    if(mfrc522[i].PICC_IsNewCardPresent() && mfrc522[i].PICC_ReadCardSerial()) {
      //readRFID = dump_byte_array(mfrc522[i].uid.uidByte, mfrc522[i].uid.size);
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
}
