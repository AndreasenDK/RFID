#include <MFRC522.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Stepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x3f, 20, 4);

//RFID PIN - placering
#define SS_PIN 53
#define RST_PIN 5

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

int j = 0;

String RFIDKey1 = "93:81:A2:08";
String RFIDKey2 = "73:DE:F2:96";
String RFIDKey3 = "B3:A8:BD:0A";


Stepper myStepper(200, 6, 7, 8, 9);


void setup() {

  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  myStepper.setSpeed(150);









}

void loop() {

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);


  // Check kort type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    return;
  }

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }
  strID.toUpperCase();

  Serial.print("Scan dit studiekort : ");
  Serial.println(strID);

  //check RFID keys
  if (strID.indexOf(RFIDKey1) >= 0 or strID.indexOf(RFIDKey2) >= 0) {

    if (j == 2) {
      j = 0;
    }
    if (j == 0) {
      {
      digitalWrite(6, HIGH);
      myStepper.step(400);
      digitalWrite(7, LOW);

    }
  }

  if (j == 1) {
    {
  
    digitalWrite(7, HIGH);
    myStepper.step(-2000);
    digitalWrite(6, LOW);
  }
  }
  j++;
  }
  else
  {
    digitalWrite(7, HIGH);
    myStepper.step(2000);
    digitalWrite(6, LOW);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
