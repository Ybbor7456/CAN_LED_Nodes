#include <SPI.h>
#include "mcp_can.h"

constexpr byte CS_PIN   = 17;   // MCP2155 CS
constexpr byte INT_PIN  = 20;   // only necessary if the board lacks a pull-up
constexpr byte LED_PIN  = 8;
constexpr unsigned CAN_ID_MATCH = 0x101;

constexpr unsigned ID_NODE1 = 0x310;   // Pi #1 (not this board)
constexpr unsigned ID_NODE2 = 0x311;   // Pi #2 (this one)
constexpr unsigned ID_NODE3 = 0x312; 	// PI #3

constexpr byte LED_OFF = 0x00;
constexpr byte LED_ON  = 0x01;

MCP_CAN CAN(CS_PIN);


void setup() {
  Serial.begin(115200);
  uint32_t t0 = millis();
  while (!Serial && millis() - t0 < 3000) {}  

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);


  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
    Serial.println("CAN init FAIL");
    while (1) {}
  }

  CAN.init_Mask(0, 0, 0x7FF);           // match all 11 bits
  CAN.init_Filt(0, 0, ID_NODE2);
  CAN.init_Filt(1, 0, ID_NODE2);

  CAN.init_Mask(1, 0, 0x7FF);
  CAN.init_Filt(2, 0, ID_NODE2);
  CAN.init_Filt(3, 0, ID_NODE2);
  CAN.init_Filt(4, 0, ID_NODE2);
  CAN.init_Filt(5, 0, ID_NODE2);

  CAN.setMode(MCP_NORMAL);

  pinMode(INT_PIN, INPUT_PULLUP);
}

void loop() {
  if (CAN.checkReceive() == CAN_MSGAVAIL) {
    unsigned long id = 0;
    byte len = 0;
    byte buf[8] = {0};

    CAN.readMsgBuf(&id, &len, buf);

 
    if (id == ID_NODE2 && len >= 1) {
      if (buf[0] == LED_ON)  { digitalWrite(LED_PIN, HIGH); 
      Serial.println("LED ON"); }
      else { digitalWrite(LED_PIN, LOW);  
      Serial.println("LED OFF"); }
    }

  }
}
