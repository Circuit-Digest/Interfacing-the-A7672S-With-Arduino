#include <SoftwareSerial.h>

// Pin definitions for GSM module
const int GSM_TX_PIN = 3;  // GSM TX connected to Arduino pin 3
const int GSM_RX_PIN = 2;  // GSM RX connected to Arduino pin 2

// Baud rates
const long SERIAL_BAUD_RATE = 9600;
const long GSM_BAUD_RATE = 9600;

// Timing constants
const unsigned long INITIALIZATION_DELAY = 1000;  // 1 second delay for initialization
const unsigned long SERIAL_TIMEOUT = 2000;        // 2 second timeout for serial communication

// Create software serial object to communicate with GSM module
SoftwareSerial gsmModule(GSM_TX_PIN, GSM_RX_PIN);

void setup() {
  // Initialize serial communication with Arduino IDE Serial Monitor
  Serial.begin(SERIAL_BAUD_RATE);
  
  // Initialize serial communication with GSM GSM module
  gsmModule.begin(GSM_BAUD_RATE);
  
  Serial.println("Initializing GSM module...");
  delay(INITIALIZATION_DELAY);
  
  // Test basic AT command communication
  gsmModule.println("AT");        // Handshake test - should return OK
  handleSerialCommunication();
  
  // Check signal quality (0-31, where 31 is best signal strength)
  gsmModule.println("AT+CSQ");
  handleSerialCommunication();
  
  // Check network registration status
  gsmModule.println("AT+CREG?");
  handleSerialCommunication();

  // Uncomment below to send SMS
  // gsmModule.println("AT+CMGF=1"); // Configure TEXT mode
  // handleSerialCommunication();
  // gsmModule.println("AT+CMGS=\"+919944373236\""); // Change number as needed
  // handleSerialCommunication();
  // gsmModule.print("Hello from GSM module."); // SMS content
  // handleSerialCommunication();
  // gsmModule.write(26); // Send Ctrl+Z to end SMS
}

void loop() {
  // Continuously handle serial communication between module and monitor
  handleSerialCommunication();
}



/**
 * Handle bidirectional serial communication between Arduino Serial Monitor and GSM module
 * Forwards user input from Serial Monitor to GSM module
 * Displays GSM module responses in Serial Monitor
 * Includes timeout to prevent hanging
 */
void handleSerialCommunication() {
  unsigned long communicationStartTime = millis();
  
  // Continue communication for up to the specified timeout period
  while (millis() - communicationStartTime < SERIAL_TIMEOUT) {
    
    // Forward any input from Serial Monitor to GSM module
    if (Serial.available()) {
      char userInput = Serial.read();
      gsmModule.write(userInput);
    }
    
    // Display any response from GSM module in Serial Monitor
    while (gsmModule.available()) {
      char moduleResponse = gsmModule.read();
      Serial.write(moduleResponse);
      
      // Reset timeout when receiving data to allow for complete responses
      communicationStartTime = millis();
    }
  }
}