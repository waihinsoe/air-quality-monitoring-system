#include <ArduinoJson.h>

/*************** firebase setup ****************/
#include <Firebase_ESP_Client.h>
// Include helper functions for Firebase token and RTDB management
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "AIzaSyDlyMzwQN36mqMs-16lY6sHggV1D3XBBiw"
#define USER_EMAIL "yuzinmontutgo@gmail.com"
#define USER_PASSWORD "yewaitun11621"
#define DATABASE_URL "https://air-quality-monitoring-s-abfc6-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Firebase objects for managing data and authentication
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String uid;

/************** wifi setup ****************/
#include <WiFi.h>
#define WIFI_SSID "6ecthesis"
#define WIFI_PASSWORD "6ecthesis"


/******************** mq135 sensor *******************/
#include <MQ135.h>
#define MQ135_SENSOR_PIN 5
MQ135 airSensor = MQ135(MQ135_SENSOR_PIN);

/********************* DHT22 *********************/
#include <DHT.h>
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

/********************* sps30 setup ****************/
#include <Wire.h>
#include <sps30.h>

#define SDA_PIN 3
#define SCL_PIN 2

//*********************************************************************************************************

//*************************** global variables *********************
float PM_1p0;
float PM_2p5;
float PM_4p0;
float PM_10p0;
String pmAirQuality;

// ************************ golbal function ************************** 
void initWiFi();
String assessAirQuality(float pm2_5);
float calculatePPM(float voltage);
String mq135AirQualityLevel(float ppm);
void setup_firebase();
void syncDataToFirebase(float humi, float temC, float ppm, const char* mq135AirQuality, float PM_1p0, float PM_2p5, float PM_4p0, float PM_10p0, const char* pmAirQuality);


void setup() {
  uint8_t auto_clean_days = 4;
  Serial.begin(115200); // Start the main serial at 115200 baud rate for debugging
  initWiFi();
  dht.begin();

  setup_firebase();
  //************** sps 30 *******************
  Wire.begin(SDA_PIN, SCL_PIN);
  while (sps30_probe() != 0) {
    Serial.println("SPS sensor probing failed");
    delay(500);
  }

  if (sps30_set_fan_auto_cleaning_interval_days(auto_clean_days)) {
    Serial.println("Error setting the auto-clean interval");
  }

  if (sps30_start_measurement() < 0) {
    Serial.println("Error starting measurement");
  }

  delay(500);

  //************** sps 30 *******************

}

void loop() {
  /************ sps 30 ********************************/
  struct sps30_measurement m;
  uint16_t data_ready;

  if (sps30_read_data_ready(&data_ready) < 0 || !data_ready) {
    // Serial.println("Data not ready, no new measurement available");
    delay(100);
    return;
  }

  if (sps30_read_measurement(&m) < 0) {
    Serial.println("Error reading measurement");
  } else {
    StaticJsonDocument<200> doc;

      PM_1p0 =  m.mc_1p0;
      PM_2p5 =  m.mc_2p5;
      PM_4p0 =  m.mc_4p0;
      PM_10p0 = m.mc_10p0;

      // Serial.println(PM_1p0);
      // Serial.println(PM_2p5);
      // Serial.println(PM_4p0);
      // Serial.println(PM_10p0);

      pmAirQuality = assessAirQuality(PM_2p5);
      // Serial.println(pmAirQuality);
  }


  /************** MQ135 SENSOR ********************/

  int sensorValue = analogRead(MQ135_SENSOR_PIN); // Read the analog value from sensor
  float sensorVoltage = sensorValue * (5.0 / 1023.0); // Convert the analog value to voltage
  float ppm = calculatePPM(sensorVoltage); // Convert voltage to ppm (you'll define this function)

  // Serial.print("Sensor Voltage: ");
  // Serial.print(sensorVoltage);
  // Serial.print("V, ");
  // Serial.print("Air Quality: ");
  // Serial.print(ppm);
  // Serial.println(" ppm");
  String mq135AirQuality = mq135AirQualityLevel(ppm); 

  /******************** DHT22 SENSOR ******************/
  // Reading humidity and temperature

  float humi = dht.readHumidity();
  float temC = dht.readTemperature();

  if(temC >= 35) {
    StaticJsonDocument<200> doc;
    doc["sensor"] = "dht22";
    serializeJson(doc, Serial); // Serialize JSON and send over serial
  }

  // Serial.print("Temperature: ");
  // Serial.print(temC);
  // Serial.print("°C  Humidity: ");
  // Serial.print(humi);
  // Serial.println("%");

  syncDataToFirebase( humi, temC, ppm, mq135AirQuality.c_str(), PM_1p0, PM_2p5, PM_4p0, PM_10p0, pmAirQuality.c_str());
  delay(500);
}


void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}


void setup_firebase() {
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;

  // Initialize Firebase with the configured settings
  Firebase.begin(&config, &auth);

  // Wait for user UID as it's necessary for database path
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }

  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);
  Serial.println("OK ..");

}

void syncDataToFirebase(float humi, float temC, float ppm, const char* mq135AirQuality, float PM_1p0, float PM_2p5, float PM_4p0, float PM_10p0, const char* pmAirQuality) {
  FirebaseJson json;

  json.set("/DHT22/temC", temC);
  json.set("/DHT22/humi", humi);

  FirebaseJson mq135Json;
  mq135Json.set("/value", ppm);
  mq135Json.set("/quality", mq135AirQuality);
  json.set("/mq135", mq135Json);

  FirebaseJson pmJson;
  pmJson.set("/PM_1p0", PM_1p0);
  pmJson.set("/PM_2p5", PM_2p5);
  pmJson.set("/PM_4p0", PM_4p0);
  pmJson.set("/PM_10p0", PM_10p0);
  pmJson.set("/quality", pmAirQuality);
  json.set("/pmValue", pmJson);

  String path = "/SensorData/" + String(uid);

  if (!Firebase.RTDB.updateNode(&fbdo, path, &json)) {
    // Serial.println("Failed to upload data: " + String(fbdo.errorReason()));
  } else {
    // Serial.println("Uploaded successfully!");
  }
}

float calculatePPM(float voltage) {
  // Example conversion formula - this needs calibration
  float rs_air_ratio = (5.0 - voltage) / voltage; // Assuming RL = 1KOhm
  float ppm = 116.6020682 * pow(rs_air_ratio, -2.769034857);
  return ppm;
}

String assessAirQuality(float pm2_5) {
    if(pm2_5 >= 40) {
        StaticJsonDocument<200> doc;
        doc["sensor"] = "pm";
        serializeJson(doc, Serial); // Serialize JSON and send over serial
    }

    if (pm2_5 <= 12.0) {
        return "Good";
    } else if (pm2_5 <= 40.4) {
        return "Moderate";
    } else if (pm2_5 <= 55.4) {
        return "Unhealthy for Sensitive Groups";
    } else if (pm2_5 <= 150.4) {
        return "Unhealthy";
    } else if (pm2_5 <= 250.4) {
        return "Very Unhealthy";
    } else {
        return "Hazardous";
    }


}

String mq135AirQualityLevel(float ppm) {

  if(ppm >= 400) {
    StaticJsonDocument<200> doc;
    doc["sensor"] = "mq135";
    serializeJson(doc, Serial); // Serialize JSON and send over serial
  }

  if (ppm < 400) {
    return "Good";
  } else if (ppm >= 600 && ppm < 1000) {
    return "Moderate";
  } else if (ppm >= 1000 && ppm < 1500) {
    return "Poor";
  } else if (ppm >= 1500 && ppm < 2000) {
    return "Unhealthy";
  } else if (ppm >= 2000 && ppm < 5000) {
    return "Very Unhealthy";
  } else {
    return "Hazardous";
  }


}





