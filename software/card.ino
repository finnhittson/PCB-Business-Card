#include <Wire.h>

#define NFC_ADDR 0x02
#define BLK_ADDR_0 0x00
#define BLK_ADDR_1 0x01

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Wire.begin();
  delay(100);

  scanI2CAddr();

  if (true) {
    uint8_t block0[16];
    block0[12] = 0xE1;
    block0[13] = 0x10;
    block0[14] = 0x6D;
    block0[15] = 0x00;
    writePage(BLK_ADDR_0, block0);
    delay(100);

    uint8_t block1[16];
    block1[0] = 0x03;
    block1[1] = 0x00;
    block1[2] = 0xFE;
    writePage(BLK_ADDR_1, block1);
    delay(100);
  }

  // here just for debugging
  if (false) {
    uint8_t block0[16];
    readPage(BLK_ADDR_0, block0);
    delay(100);

    Serial.println("Block 0 before modification");
    printArray(block0, 16);

    block0[12] = 0xE1;
    block0[13] = 0x10;
    block0[14] = 0x6D;
    block0[15] = 0x00;
    writePage(BLK_ADDR_0, block0);
    delay(100);

    readPage(BLK_ADDR_0, block0);
    delay(100);

    Serial.println("Block 0 after modification");
    printArray(block0, 16);

    uint8_t block1[16];
    readPage(BLK_ADDR_1, block1);
    delay(100);

    Serial.println("Block 1 before modification");
    printArray(block1, 16);

    for (uint8_t i = 0; i < 16; i++) {
      block1[i] = 0x00;
    }
    block1[0] = 0x03;
    block1[2] = 0xFE;
    writePage(BLK_ADDR_1, block1);
    delay(100);

    readPage(BLK_ADDR_1, block1);
    delay(100);

    Serial.println("Block 1 after modification");
    printArray(block1, 16);
  }
}

void loop() {
}

void readPage(uint8_t page, uint8_t *result) {
  Wire.beginTransmission(NFC_ADDR);
  Wire.write(page);
  Wire.endTransmission(false);
  uint8_t length = Wire.requestFrom(NFC_ADDR, (uint8_t)16);
  for (uint8_t idx = 0; idx < length && Wire.available(); idx++) {
    result[idx] = Wire.read();
  }
}

void writePage(uint8_t page, uint8_t *data) {
  Wire.beginTransmission(NFC_ADDR);
  Wire.write(page);
  for (uint8_t i = 0; i < 16; i++) {
    Wire.write(data[i]);
  }
  Wire.endTransmission();
}

void printArray(uint8_t *data, uint8_t n) {
  for (uint8_t i = 0; i < n; i++) {
    Serial.println(data[i]);
  }
  Serial.println();
}

void scanI2CAddr() {
  byte error, address;
  int devices = 0;

  // Serial.println("Scanning...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    delay(10);

    if (error == 0) {
      Serial.print("I2C device found at 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println();
      devices++;
    }
    else if (error == 4) {
      Serial.print("Unknown error at 0x");
      if (address < 16) Serial.print("0");
        Serial.println(address, HEX);
    }
  }

  if (devices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("Scan complete\n");
}