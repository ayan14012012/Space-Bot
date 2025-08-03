#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// WiFi credentials
const char* ssid = "MyHotspot";
const char* password = "12345678";

// Server URLs
const String dataURL = "http://192.168.137.1/spacebot/update.php";
const String commandURL = "http://192.168.137.1/spacebot/commands.json";

// Pin Definitions
#define MQ8_PIN     D1
#define MQ135_PIN   D2
#define IN1         D5
#define IN2         D6
#define TRIG_PIN    D7
#define ECHO_PIN    D8

// Control flags (set from server)
bool capsule1_motor = false;
bool capsule1_ultrasonic = false;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected");

  pinMode(MQ8_PIN, INPUT);
  pinMode(MQ135_PIN, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  stopMotors();
}

void loop() {
  fetchCommands();  // Check if motors/ultrasonic should run

  // Always read gas sensors
  int mq8 = digitalRead(MQ8_PIN);
  int mq135 = digitalRead(MQ135_PIN);

  // Handle ultrasonic and motors only if enabled
  if (capsule1_ultrasonic && capsule1_motor) {
    int distance = getDistance();

    Serial.print("📏 Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance > 15) {
      moveForward();
      Serial.println("🟢 Moving forward");
    } else {
      Serial.println("🟥 Obstacle detected! Reversing...");
      moveBackward();
      delay(800);
      stopMotors();
    }
  } else {
    stopMotors();  // If not enabled, stop motors
  }

  // Send gas sensor data to server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    String url = dataURL + "?mq8=" + mq8 + "&mq135=" + mq135;
    http.begin(client, url);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("✅ Data sent. Response: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("❌ Error sending data: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }

  delay(3000);
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

// ---------------- Distance Function ----------------
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 25000);
  int distance = duration * 0.034 / 2;
  return distance;
}

// ---------------- Fetch Command from Server ----------------
void fetchCommands() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    http.begin(client, commandURL);
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      String payload = http.getString();

      // Simple parsing without ArduinoJson
capsule1_motor = payload.indexOf("\"capsule1_motor\": true") != -1;
capsule1_ultrasonic = payload.indexOf("\"capsule1_ultrasonic\": true") != -1;


      Serial.print("🔁 Command Fetched → Motor: ");
      Serial.print(capsule1_motor);
      Serial.print(" | Ultrasonic: ");
      Serial.println(capsule1_ultrasonic);
    } else {
      Serial.print("❌ Command fetch failed: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
}
