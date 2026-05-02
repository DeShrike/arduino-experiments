#include <SPI.h>
#include <MFRC522.h>

// Homer : A3 CA 72 9A
// Marge : 1D 58 42 16
// Bart : 3D 76 7A 4A

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

const int ledCount = 5;
int ledPins[] = { 2, 3, 4, 5, 6 };
int ledState[] = { 0, 0, 0, 0, 0 };

void setup() {
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card

  for (int i = 0; i < ledCount; i++)
  {
    pinMode(ledPins[i], OUTPUT);
    Serial.println(ledPins[i]);
  }

  Serial.begin(9600);
  Serial.println("Starting");

  for (int i = 0; i < ledCount; i++)
  {
    digitalWrite(ledPins[i], HIGH);
    delay(200);
  }

  delay(500);

  for (int i = 0; i < ledCount; i++)
  {
    digitalWrite(ledPins[i], LOW);
    delay(200);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (mfrc522.PICC_IsNewCardPresent() == false) {
    return;
  }

  // Select one of the cards
  if (mfrc522.PICC_ReadCardSerial() == false) {
    return;
  }

  Serial.print(F("Card UID:"));    //Dump UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }







  delay(1000); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
