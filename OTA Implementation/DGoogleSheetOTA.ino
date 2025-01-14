#define MDASH_APP_NAME "MinimalApp"
#include <mDash.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define RXD1 16  // Change to your specific RX pin
#define TXD1 17  // Change to your specific TX pin

#define WIFI_NETWORK "RUT_B4Toffice"
#define WIFI_PASSWORD "Peipi6xahco0eepu"
#define DEVICE_PASSWORD "Mf2ucqEyzkRxhe4IYxK12w"

const String scriptUrl = "https://script.google.com/macros/s/AKfycbyYoKnoQBZVDCnhrXNBZVcNlHnh1EYuWAPSmcGlzuzhwsFoAZkIDS_xLv3jB1Zl1XfM/exec";

// Array to hold the gas reading data (for example purposes)
char readArray[4];

void setup() {
  Serial.begin(38400);
  Serial1.begin(38400);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected, IP address:");
  Serial.println(WiFi.localIP());
  mDashBegin(DEVICE_PASSWORD);
}

void send_read_live_data_simple() 
{
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
    float gasReading = *(float*)readArray;  // Convert the 4-byte array to a float
    Serial.println(gasReading);             // Print the converted float value
    sendDataToGoogleSheet(gasReading);      // Send data to Google Sheet
  }
}

void sendDataToGoogleSheet(float gasReading) {
  HTTPClient http;

  http.begin(scriptUrl);  // Your Apps Script URL

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String httpRequestData = "gasReading=" + String(gasReading, 4);  // Sends data with 4 decimal places

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

  delay(5000);  // Add a delay to avoid 
}
