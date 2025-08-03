  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>

  const char* ssid = "MyHotspot";
  const char* password = "12345678";

  const String serverUrl = "http://192.168.137.1/spacebot/update_pulse.php";

  // Pin setup
  const int pulsePin = A0;
  const int touchPin = D5;  // Touch sensor pin

  // Thresholds
  const int lowPulseThreshold = 500;  // Adjust based on your sensor

  void setup() {
    Serial.begin(115200);
    pinMode(touchPin, INPUT);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi connected");
  }

  void loop() {
    int touchState = digitalRead(touchPin); // 1 when touched

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      WiFiClient client;

      if (touchState == HIGH) {
        int pulseValue = analogRead(pulsePin);
        Serial.print("Pulse (Raw): ");
        Serial.println(pulseValue);

        String alert = "normal";
        if (pulseValue < lowPulseThreshold) {
          alert = "low";
        }

        String url = serverUrl + "?pulse=" + pulseValue + "&alert=" + alert + "&touch=yes";
        http.begin(client, url);
        http.GET();
        http.end();
         
      } else {
        // Send signal to show "Touch Inactive" to website
        Serial.println("No touch detected. Not reading pulse.");
        String url = serverUrl + "?pulse=0&alert=none&touch=no";
        http.begin(client, url);
        http.GET();
        http.end();
       
      }
      
    }

    delay(200);  // Delay to avoid spam and debounce
  }
