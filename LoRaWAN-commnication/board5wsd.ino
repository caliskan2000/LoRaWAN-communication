#include <SPI.h>
#include <LoRa.h>
#include <SD.h>

const int chipSelect = 4; // Change this to the appropriate pin for your setup

void setup() {
  Serial.begin(9600);
  
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  if (!SD.begin(chipSelect)) {
    Serial.println("Initialization failed. Check if the SD card is inserted properly.");
    while (1);
  }

  // Set LoRa receiver mode
  LoRa.receive();
}

void loop() {
  int packetSize = LoRa.parsePacket(); // Check if there's a packet available

  if (packetSize) { // If packet is available
    String receivedData = "";

    // Read packet data
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    int rssi = LoRa.packetRssi();

    // Print received data
    Serial.print("Received packet: ");
    Serial.println(receivedData);
    Serial.print("RSSI: ");
    Serial.println(rssi);

    //logData(receivedData);

    // Check if received data matches "Ale_ACK"
    if (receivedData.equals("Ale_ACK ")) {
      Serial.println("Received Ale_ACK from board4");
      // Handle the acknowledgment
      // (You can add your desired functionality here)

      logData(receivedData, rssi);
    }
  }
}

void logData(String data, int rssi) {
  // Open the file in write mode
  File dataFile = SD.open("log.txt", FILE_WRITE);

  // If the file is available, write to it
  if (dataFile) {
    dataFile.print("Data: ");
    dataFile.print(data);
    dataFile.print("Ale");
    dataFile.print("RSSI: ");
    dataFile.println(rssi);
    dataFile.close();
    Serial.println("Data logged to SD card.");
  }
  else {
    Serial.println("Error opening log file.");
  }
}
