#include <WiFi.h>
#include <HTTPClient.h>
#include<ArduinoJson.h>

#define RXD1 16  // Change to your specific RX pin
#define TXD1 17  // Change to your specific TX pin


// Replace these with your network credentials
const char* ssid = "Your WiFi SSID";         // Replace with your WiFi SSID
const char* password = "Your WiFi Password";  // Replace with your WiFi Password


// Google Apps Script Web App URL
const String scriptUrl = "https://script.google.com/macros/s/AKfycbyYoKnoQBZVDCnhrXNBZVcNlHnh1EYuWAPSmcGlzuzhwsFoAZkIDS_xLv3jB1Zl1XfM/exec";

// Array to hold the gas reading data (for example purposes)
char readArray[4];

void setup() {
  Serial.begin(38400);
  Serial1.begin(38400);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void send_read_live_data_simple() {
  char readByte1 = 0x00;
  char readByte2 = 0x00;
  char readByte3 = 0x7A;
  char readByte4 = 0xC3;

  readArray[0] = 0x00;
  readArray[1] = 0x00;
  readArray[2] = 0x7A;
  readArray[3] = 0xC3;
}

void receive_read_live_data_sample() {
  while (Serial1.available()) {
    float gasReading = *(float*)readArray; // Convert the 4-byte array to a float
    Serial.println(gasReading);           // Print the converted float value
    sendDataToGoogleSheet(gasReading);    // Send data to Google Sheet
  }
}

void sendDataToGoogleSheet(float gasReading) {
  HTTPClient http;
  
  http.begin(scriptUrl); // Your Apps Script URL
  
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  String httpRequestData = "gasReading=" + String(gasReading, 4); // Sends data with 4 decimal places
  
  int httpResponseCode = http.POST(httpRequestData);
  
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error in HTTP request: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
}

void loop() {
  // Sending the array to simulate sensor data
  send_read_live_data_simple();

  // Simulate receiving and processing the data
  receive_read_live_data_sample();

  delay(5000); // Add a delay to avoid flooding Serial
}
