#include <SPI.h>
#include <LoRa.h>

int debug = 0;
int counter = 0;
const int packetsToSend = 10; // Number of packets to send
bool sen_data = true;    // State variable to control sending

int sf = 7;

void setup() {
Serial.begin(9600);
if(debug) {
while (!Serial);

Serial.println("LoRa Sender");
}

if (!LoRa.begin(868E6)) {
if(debug) {
Serial.println("Starting LoRa failed!");
}
while (1);
}
LoRa.setTxPower(14);
}

void loop() {
  
if (sen_data) {
LoRa.setSpreadingFactor(sf);
for (int i = 0; i < packetsToSend; i++) {
if(debug) {
Serial.print("Sending packet: Ciao_ACK ");
Serial.println(counter);
}
counter++;

      // Send packet
LoRa.beginPacket();
LoRa.print("Ciao_ACK ");
LoRa.endPacket();

delay(10000);
}
    

sf++;

sen_data = false;

} 

else {
    // Wait for 5 seconds
    //delay(5000);
for (int i = 0; i < packetsToSend; i++) {
delay(10000);
}
    // Resume sending packets
sen_data = true;
}
}

