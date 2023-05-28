#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//OLED Display Object
Adafruit_SSD1306 display(128, 64, &Wire, -1);

//Put your WiFi Credentials here
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

//URL Endpoint for the API
String URL = "https://v2.jokeapi.dev/joke/Programming,Spooky?type=twopart";

void setup() {
  Serial.begin(115200);

  // Setup The OLED Display and initialize: address => 0x3C or 0x3D
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  // wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    //Set HTTP Request Final URL
    http.begin(URL);

    // start connection and send HTTP Request
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {

      //Read Data as a JSON string
      String JSON_Data = http.getString();
      Serial.println(JSON_Data);

      //Retrieve the joke from the JSON format
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, JSON_Data);
      JsonObject obj = doc.as<JsonObject>();

      const char* setup = obj["setup"].as<const char*>();
      const char* delivery = obj["delivery"].as<const char*>();
      
      //Display the Two parts of the joke
      display.clearDisplay();

      display.setTextSize(1);               // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);  // Draw white text
      display.setCursor(0, 0);              // Start at top-left corner

      display.print("Setup: ");
      display.println(F(setup));
      display.print("Delivery: ");
      display.println(F(delivery));
      display.display();

    } else {
      Serial.println("Error!");
      delay(2000);
    }
    http.end();
  }
  
  delay(60000);
}
