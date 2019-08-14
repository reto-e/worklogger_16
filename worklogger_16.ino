#include <SPI.h>
#include<WiFiNINA.h>

// ----------------------------------------------------
//      variables
// ----------------------------------------------------

int current_position = -1; 
int new_position;               // position read from poti
int stable_count = 0;           // counts how many times current and new position are the same;
char ssid[] = "AndroidAP4750";    // your network SSID (name)
char pass[] = "aaf06d16f711";  // your network password (use for WPA, or use as key for WEP)
char server[] = "docs.google.com"; // google forms server
int status = WL_IDLE_STATUS;    // 
WiFiSSLClient client;           // client object which does the calls

String jobs[11] = { 
  "Con Admin", 
  "Con Testing", 
  "Con Analysis", 
  "Con Coord",
  "Z Admin",
  "Z Coord",
  "Z Analysis",
  "Z Testing",
  "I10000",
  "I15000",
  "Pause"
};

// ----------------------------------------------------
//      functions
// ----------------------------------------------------

// define position from turn knob
int define_position() {

  int pos = current_position; 
  
  int bit_8 = digitalRead(4);
  int bit_4 = digitalRead(5);
  int bit_2 = digitalRead(6);
  int bit_1 = digitalRead(7);
  
  // debug stuff
  Serial.print(bit_1);
  Serial.print(" ");
  Serial.print(bit_2); 
  Serial.print(" ");
  Serial.print(bit_4); 
  Serial.print(" ");
  Serial.println(bit_8); 
  
  if      (bit_1 == 0 && bit_2 == 0 && bit_4 == 0 & bit_8 == 0) pos = 0;
  else if (bit_1 == 1 && bit_2 == 0 && bit_4 == 0 & bit_8 == 0) pos = 1;
  else if (bit_1 == 0 && bit_2 == 1 && bit_4 == 0 & bit_8 == 0) pos = 2;
  else if (bit_1 == 1 && bit_2 == 1 && bit_4 == 0 & bit_8 == 0) pos = 3;
  else if (bit_1 == 0 && bit_2 == 0 && bit_4 == 1 & bit_8 == 0) pos = 4;
  else if (bit_1 == 1 && bit_2 == 0 && bit_4 == 1 & bit_8 == 0) pos = 5;
  else if (bit_1 == 0 && bit_2 == 1 && bit_4 == 1 & bit_8 == 0) pos = 6;
  else if (bit_1 == 1 && bit_2 == 1 && bit_4 == 1 & bit_8 == 0) pos = 7;
  else if (bit_1 == 0 && bit_2 == 0 && bit_4 == 0 & bit_8 == 1) pos = 8;
  else if (bit_1 == 1 && bit_2 == 0 && bit_4 == 0 & bit_8 == 1) pos = 9;
  else if (bit_1 == 0 && bit_2 == 1 && bit_4 == 0 & bit_8 == 1) pos = 10;
  else if (bit_1 == 1 && bit_2 == 1 && bit_4 == 0 & bit_8 == 1) pos = 11;
  else if (bit_1 == 0 && bit_2 == 0 && bit_4 == 1 & bit_8 == 1) pos = 12;
  else if (bit_1 == 1 && bit_2 == 0 && bit_4 == 1 & bit_8 == 1) pos = 13;
  else if (bit_1 == 0 && bit_2 == 1 && bit_4 == 1 & bit_8 == 1) pos = 14;
  else if (bit_1 == 1 && bit_2 == 1 && bit_4 == 1 & bit_8 == 1) pos = 15;

  return pos;
}

// store job to google forms
String storeWork(int pos) {
  Serial.print("storing job ");
  Serial.println(jobs[pos]);
  String jobdata = jobs[pos];
  jobdata.replace(" ", "%20");
  if (client.connectSSL(server, 443)) {
    client.print("GET http://docs.google.com/forms/d/e/1FAIpQLSdo4mZ-P4LfauRNjesSDceLhdNDUcVl5QPq9StZG6vIGjwQng/formResponse?entry.1030855005=");
    client.print(jobdata);
    client.println(" HTTP/1.1");
    client.println("Host: docs.google.com");
    client.println("Cache-Control: no-cache");
    client.println("Connection: close");
    client.println();
    }
    
    return "ok";
}


// ----------------------------------------------------
//      setup
// ----------------------------------------------------
void setup() {
  Serial.begin(9600);
  pinMode(4, INPUT);  // bit_8
  pinMode(5, INPUT);  // bit_4
  pinMode(6, INPUT);  // bit_2
  pinMode(7, INPUT);  // bit_1

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
     
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(7000);
  }
  Serial.println("Connected to wifi");

}


// ----------------------------------------------------
//      loop
// ----------------------------------------------------
void loop() {
  // read position from turn knob
  new_position = define_position();

  if (new_position != current_position) {
    stable_count = 0;
    
  }
  if (stable_count == 1) {
    storeWork(new_position);
  }
  current_position = new_position; 
  stable_count++;

  // Debug block
//  Serial.print(new_position);
//  Serial.print("\t");
//  Serial.print(current_position);
//  Serial.print("\t");
//  Serial.println(stable_count);

  delay(1000);
}
