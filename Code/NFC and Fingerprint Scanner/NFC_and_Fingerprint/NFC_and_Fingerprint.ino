#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"
#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <Keyboard.h>

#define IRQ 6
#define RESET 8

Adafruit_NFCShield_I2C nfc(IRQ, RESET);

FPS_GT511C3 fps(10, 11); // change to whichever tx/rx ports your fingerprint scanner is using

void setup() {
  Serial.begin(9600);
  delay(5000);
  fps.Open(); // Turn on fingerprint scanner
  fps.SetLED(true); // Turn on the scanner LED
 
  nfc.begin(); // find Adafruit RFID/NFC shield

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
 
  // configure board to read RFID tags
  nfc.SAMConfig();
Keyboard.begin(); //initiate the Keyboard
}

/////////////////////////////////// LOOP

unsigned digit = 0;

void loop() {
  // Used for debugging
  if (fps.IsPressFinger()) // Checks if finger has been placed
  {
    fps.CaptureFinger(false);
    int id = fps.Verify1_1(0); // Verifies finger #
    if (id < 1) // Increase number to however many fingerprints are enrolled
    {
      Serial.print("Verified ID:"); // If finger scanned and success
      Serial.println(id);
    }
    else
    {
      Serial.println("Finger not found"); // If finger scanned and not found
    }
  }
  else
  {
    Serial.println("Please press finger"); // Asks to fix finger
  }

  
  delay(100);
  
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
  uint8_t uidLength; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // wait for RFID card to show up!
  Serial.println("Waiting for an ISO14443A Card ...");

   
  // Wait for an ISO14443A type cards (Mifare, etc.). When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  uint32_t cardidentifier = 0;
 
  if (success) {
    // Found a card!

    Serial.print("Card detected #");
    // turn the four byte UID of a mifare classic into a single variable #
    cardidentifier = uid[3];
    cardidentifier <<= 8; cardidentifier |= uid[2];
    cardidentifier <<= 8; cardidentifier |= uid[1];
    cardidentifier <<= 8; cardidentifier |= uid[0];
    Serial.println(cardidentifier);
    
    fps.CaptureFinger(false);
    int id = fps.Verify1_1(0);
    if (cardidentifier == 2428156789 && id == 0) { //change card # to whatever the uid is and change the id to which fingerprint id(s) you want
      Keyboard.write('p');
      Keyboard.write('a');
       Keyboard.write('s');
        Keyboard.write('s');
         Keyboard.write('w');
          Keyboard.write('o');
           Keyboard.write('r');
            Keyboard.write('d');
             Keyboard.write('0');
              Keyboard.write('1');
               Keyboard.write(KEY_RETURN);
              delay(5000); //makes sure the password isn't repeated
}
  }
    }
