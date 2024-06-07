#include <Wire.h>

#define EEPROM_I2C_ADDRESS 0x50

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  while (!Serial) {
    delay(10);
  }
  
  // Initialize I2C bus
  Wire.begin();
  
  // get EEPROM info
  int eepromSize = getEEPROMSize();
  Serial.print("EEPROM size detected: ");
  Serial.print(eepromSize);
  Serial.println(" KB");
  
  // Read values from the EEPROM
  readValuesFromEEPROM(eepromSize);
}

void loop() {
}

// read values from EEPROM
void readValuesFromEEPROM(int eepromSize) {
  Serial.println("Reading from EEPROM...");
  for (int i = 0; i < eepromSize * 1024; i++) {
    byte value = readEEPROM(i);
    Serial.print("Address: ");
    Serial.print(i);
    Serial.print("\tValue: ");
    Serial.println(value, DEC);
    delay(100);
  }
  Serial.println("Read complete.");
}

// read specified address
byte readEEPROM(int address) {
  byte value = 0xFF;

  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);
  if (Wire.available()) {
    value = Wire.read();
  }

  return value;
}

// get the size of EEPROM in KB
int getEEPROMSize() {
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write(0x00); // Send address 0x00 (device ID)
  Wire.endTransmission();

  Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);
  if (Wire.available()) {
    byte deviceID = Wire.read();
    int size = 1 << (deviceID & 0b111); // Calculate size from device ID
    Serial.print("EEPROM device ID: ");
    Serial.println(deviceID, BIN);
    return size;
  }

  return 32; // Default to 32KB if detection fails
}
