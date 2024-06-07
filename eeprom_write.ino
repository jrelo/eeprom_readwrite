#include <Wire.h>

#define EEPROM_I2C_ADDRESS 0x50

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  while (!Serial) {
    delay(10);
  }
  
  // Initialize the I2C bus
  Wire.begin();
  
  // Get EEPROM information
  int eepromSize = getEEPROMSize();
  printEEPROMInfo(eepromSize);
  
  // Write values to EEPROM
  writeValuesToEEPROM(eepromSize);
}

void loop() {
}

// print EEPROM info
void printEEPROMInfo(int eepromSize) {
  Serial.println("EEPROM Information:");
  Serial.print("Size: ");
  Serial.print(eepromSize);
  Serial.println(" KB");
  Serial.println();
}

// write values to the EEPROM
void writeValuesToEEPROM(int eepromSize) {
  Serial.println("Writing values to EEPROM...");
  for (int i = 0; i < eepromSize * 1024; i++) {
    byte value = i % 256; // Example value
    writeEEPROM(i, value);
    Serial.print("Address: ");
    Serial.print(i);
    Serial.print("\tValue written: ");
    Serial.println(value);
    delay(10);
  Serial.println("Done writing.");
}

// write byte to specified address in EEPROM
void writeEEPROM(int address, byte value) {
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB
  Wire.write(value);
  Wire.endTransmission();
  delay(5);
}

// get size of the EEPROM in KB
int getEEPROMSize() {
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write(0x00); // Send address 0x00 (device ID)
  Wire.endTransmission();

  Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);
  if (Wire.available()) {
    byte deviceID = Wire.read();
    return 1 << (deviceID & 0b111); // Calculate size from device ID
  }

  return 32; // Default to 32KB if detection fails
}
