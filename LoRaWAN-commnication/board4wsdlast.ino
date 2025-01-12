#include <SPI.h>
#include <LoRa.h>
#include <SD.h>

int debug = 0;
const int chipSelect = 4; // Change this to the appropriate pin for your setup
int counter = 0;

//int sf = 10;

void setup() {
Serial.begin(9600);
if(debug) {
while (!Serial);

Serial.println("LoRa Receiver3 and Sender4");
}

if (!LoRa.begin(868E6)) {
if(debug) {
Serial.println("Starting LoRa failed!");
}
while (1);
}

if (!SD.begin(chipSelect)) {
if(debug) {
Serial.println("Initialization failed. Check if the SD card is inserted properly.");
}
while (1);
}

// Register the receive callback
LoRa.onReceive(onReceive);

// Put the radio into receive mode
LoRa.receive();
//LoRa.setTxPower(14); // set output power to 17 dBm
}

void loop() {
// Do nothing in the loop, as the reception is handled by the callback
}

void onReceive(int packetSize) {
// Received a packet
if(debug) {
Serial.print("Received packet: ");
}
// Read packet
String rec_data;
for (int i = 0; i < packetSize; i++) {
rec_data = rec_data + ((char)LoRa.read());
}

if(debug) {
Serial.print(rec_data);
}

// Print RSSI of packet
if(debug) {
Serial.print(" with RSSI ");
}
int rssi = LoRa.packetRssi();
if(debug) {
Serial.println(rssi);
}

// Log received data and RSSI to SD card
logData(rec_data, rssi);

// Send acknowledgment packet (optional)
if (rec_data.equals("Fi_ACK ")) {
//logData(rec_data, rssi);
sendAck("Ale_ACK ");
} else if (rec_data.equals("START")) {
sendAck("START");
} else if (rec_data.equals("STOP")) {
sendAck("STOP");
} else {
LoRa.receive();
}

if(debug) {
  Serial.println(" ");
}
}

void sendAck(String print) {
delay(3000);
//LoRa.setSpreadingFactor(sf);
if(debug) {
Serial.print("Sending acknowledgment: " + print);
}
LoRa.beginPacket();
LoRa.print(print);
LoRa.endPacket();

//sf++;


// Put the radio back into receive mode
LoRa.receive();
}

void logData(String data, int rssi) {
// Open the file in write mode
File dataFile = SD.open("log.txt", FILE_WRITE);

// If the file is available, write to it
if (dataFile) {
dataFile.print(data);
dataFile.print(counter);
counter++;
dataFile.print(", RSSI: ");
dataFile.println(rssi);
dataFile.close();
if(debug) {
Serial.println("Data logged to SD card.");
} 
}
else {
if(debug) {
Serial.println("Error opening log file.");
}
}
}
