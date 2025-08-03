#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// ---------------- WiFi Credentials ----------------
const char* ssid = "MyHotspot";
const char* password = "12345678";

// ---------------- Server URLs ----------------
const String serverName = "http://192.168.137.1/spacebot/update.php";
const String commandUrl = "http://192.168.137.1/spacebot/commands.json";

// ---------------- State Variables ----------------
bool capsule2_motor = false;

// ---------------- Pin Definitions ----------------
#define DHTPIN     D2
#define DHTTYPE    DHT11
#define IN1        D5
#define IN2        D6
#define IR_SENSOR  D7

DHT dht(DHTPIN, DHTTYPE);

// ---------------- Setup ----------------
void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IR_SENSOR, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected");

  stopMotors();
}

// ---------------- Loop ----------------
void loop() {
  fetchCommands();  // 🔁 Get latest control status from website

  // Read obstacle status
  bool obstacleDetected = digitalRead(IR_SENSOR) == LOW;  // HIGH = Obstacle

  Serial.print("🧱 Obstacle: ");
  Serial.println(obstacleDetected ? "Yes" : "No");

  // Motor control based on command
  if (capsule2_motor) {
    if (obstacleDetected) {
      Serial.println("🟥 Obstacle detected! Reversing...");
      moveBackward();
      delay(800);
      stopMotors();
    } else {
      Serial.println("🟢 Path clear. Moving forward...");
      moveForward();
    }
  } else {
    stopMotors();
    Serial.println("🛑 Capsule2 motor disabled by command");
  }

  // Read temperature and humidity
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("⚠️ DHT read failed, retrying...");
    delay(1000);
    temp = dht.readTemperature();
    hum = dht.readHumidity();
  }

  if (!isnan(temp) && !isnan(hum)) {
    Serial.print("🌡️ temp: ");
    Serial.print(temp);
    Serial.print(" °C | 💧 hum: ");
    Serial.print(hum);
    Serial.println(" %");

    // Send to server
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      WiFiClient client;
      String url = serverName + "?temp=" + String(temp) + "&hum=" + String(hum);

      http.begin(client, url);
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.print("✅ Data sent. Response code: ");
        Serial.println(httpResponseCode);
      } else {
        Serial.print("❌ HTTP error: ");
        Serial.println(httpResponseCode);
      } 

      http.end();
    }
  } else {
    Serial.println("❌ DHT still failed after retry. Skipping update.");
  }

  delay(3000); // Wait before next cycle
}


// ---------------- Fetch Command Function ----------------
void fetchCommands() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    http.begin(client, commandUrl);
    int code = http.GET();

    if (code == 200) {
      String payload = http.getString();
      DynamicJsonDocument doc(256);
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        capsule2_motor = doc["capsule2_motor"];
        Serial.print("🔁 capsule2_motor = ");
        Serial.println(capsule2_motor ? "true" : "false");
      } else {
        Serial.println("❌ JSON parsing error");
      }
    } else {
      Serial.print("❌ Command fetch failed: ");
      Serial.println(code);
    }

    http.end();
  }
}

// ---------------- Motor Functions ----------------
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
