/*
  WiFi Web Server

 A simple web server that shows the value of the analog input pins.
 using a WiFi shield.
 */
#include <ArduinoJson.h>
#include <SPI.h>
#include <WiFi101.h>

//#define __debug__

char ssid[] = "MartinRouterKing"; // your network SSID (name)
char pass[] = "kikihalkikiha";    // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  
#ifdef __debug__
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}


void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          // Allocate a temporary JsonDocument
          // Use arduinojson.org/v6/assistant to compute the capacity.
          StaticJsonDocument<500> doc;
        
          // Create the "analog" array
          JsonArray analogValues = doc.createNestedArray("analog");
          int pin = 0;
          // Read the analog input
          int value = analogRead(pin);
      
          // Add the value at the end of the array
          analogValues.add((value * (3.3 / 1023)));
        
          Serial.print(F("Sending: "));
          serializeJson(doc, Serial);
          Serial.println();
        
          // Write response headers
          client.println(F("HTTP/1.0 200 OK"));
          client.println(F("Content-Type: application/json"));
          client.print(F("Content-Length: "));
          client.println(measureJson(doc));
          client.println(F("Connection: close"));
          client.println();
          // Write JSON document
          serializeJson(doc, client);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
