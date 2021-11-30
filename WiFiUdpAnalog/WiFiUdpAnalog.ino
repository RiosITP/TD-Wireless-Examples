/*
  WiFi UDP Send and Receive String

  This sketch waits for a UDP packet on localPort using the WiFi module.
  When a packet is received an Acknowledge packet is sent to the client on port remotePort

  created 30 December 2012
  by dlf (Metodo2 srl)

  ammended 30 November 2021
  by David Rios

*/

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>


int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key index number (needed only for WEP)

unsigned int localPort = 2390;      // local port to listen on
unsigned int sendPort = 7403;

WiFiUDP Udp;

char out[7];
char out2[7];

char outIP[] = "192.168.154.57";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial); //Sketch will stop running unless there is serial connection. Comment this out if you want to run automatically 

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}

void loop() {

  int sensor1 = analogRead(A0); // sensor 1 data
  int sensor2 = analogRead(A1); // sensor 2 data

  String out = String(sensor1); // convert to String
  String out2 = String(sensor2); // convert to String

  char outgoing[] = {0, 0, 0, 0}; // create new array
  char outgoing2[] = {0, 0, 0, 0}; // create new array

  // transfer string data to outgoing char arrays[]
  for (int i = 0; i < sizeof(out); i++) {
    outgoing[i] = out[i];
  }
  for (int i = 0; i < sizeof(out2); i++) {
    outgoing2[i] = out2[i];
  }

  // print serial for feedback
  Serial.print(sensor1);
  Serial.print("/");
  Serial.println(sensor2);

  //format and sent udp
  Udp.beginPacket(outIP, sendPort);
  Udp.write(outgoing);
  Udp.write("/");     // udp and osc messages are usually separated with "/" if sending multiple values
  Udp.write(outgoing2);
  Udp.endPacket();
  delay(200); // if you are not using call and response, do not send more information than you need
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
