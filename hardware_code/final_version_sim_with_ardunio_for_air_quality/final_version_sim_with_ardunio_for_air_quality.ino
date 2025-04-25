#include <SoftwareSerial.h>
#include <ArduinoJson.h>

// Initialize the software serial interface for the SIM900A module          
SoftwareSerial mySerial(9, 10);

const char* phoneNumbers[] = {
  "+959672264334",
  "+959672739000",  
  "+95943109732",
  "+959451832352",
  "+959450239655",
  "+959799956509",
  "+959796621703",
  "+959969617206",
};

const int numPhones = 7;

// Flags to control message sending
bool mq135Sent = false;
bool pmSent = false;
bool dht22Sent = false;

// Timer variables for resetting flags
unsigned long previousMillis = 0;
const long interval = 180000; // 10 minutes in milliseconds

void SendMessage(const char* msg);

void setup() {
  mySerial.begin(9600);   // Setting the baud rate of GSM Module
  Serial.begin(115200);   // Setting the baud rate of Serial M onitor (Arduino)
}

void loop() {
  StaticJsonDocument<200> doc; // Allocate a small JSON document

  // Check if it is time to reset the flags
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    mq135Sent = false;
    pmSent = false;
    dht22Sent = false;
    Serial.println("All flags have been reset.");
  }

  if (Serial.available()) {
    DeserializationError error = deserializeJson(doc, Serial);
    if (!error) {
      String sensorName = doc["sensor"].as<String>();

      if (sensorName == "mq135" && !mq135Sent) {
        // SendMessage("CO2 level over 600ppm!");
        SendMessage("CO2 level over 400ppm!");
        mq135Sent = true;
      } else if (sensorName == "pm" && !pmSent) {
        // SendMessage("pm level over 40µg/m³!");
        SendMessage("Don't forget to wear a mask");
        pmSent = true;
      } else if (sensorName == "dht22" && !dht22Sent) {
        // SendMessage("temperature is over 35°C!");
        SendMessage("To carry an umbralla and water bottle");
        dht22Sent = true;
      }

      Serial.println(sensorName);
    } else {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
    }
  }
  if (mySerial.available() > 0) {
    Serial.write(mySerial.read());
  }
}

void SendMessage(const char* msg) {
  mySerial.println("AT+CMGF=1");    // Sets the GSM Module in Text Mode
  delay(1000);

  for (int i = 0; i < numPhones; i++) {
    mySerial.print("AT+CMGS=\"");
    mySerial.print(phoneNumbers[i]);
    mySerial.println("\"");
    delay(1000);
    mySerial.println(msg); // The SMS text you want to send
    delay(100);
    mySerial.write(26); // ASCII code of CTRL+Z to send the SMS
    delay(5000); // Give some time for the message to be sent
    Serial.print("SMS sent to ");
    Serial.println(phoneNumbers[i]);
  }
}
