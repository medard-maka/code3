#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>

// Paramètres du réseau WiFi
const char* ssid = "McMeder";
const char* password = "mcmeder91";

// Paramètres du serveur web
AsyncWebServer server(80);

// Broches des capteurs et de la LED
const int ldrPin = 34;
const int dhtPin = 32;
const int soilPin = 35;
const int ledPin = 2;

// Instance du capteur DHT11
DHT dht(dhtPin, DHT11);

// Variables pour stocker les valeurs des capteurs
float temperature = 0.0;
float humidity = 0.0;
int ldrValue = 0;
int soilValue = 0;

void setup() {
  // Initialisation du port série
  Serial.begin(115200);

  // Connexion au réseau WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au réseau WiFi...");
  }
  Serial.println("Connecté au réseau WiFi");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());

  // Configuration du capteur DHT11
  dht.begin();

  // Configuration de la broche de la LED
  pinMode(ledPin, OUTPUT);

  // Configuration du serveur web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta http-equiv='refresh' content='5' charset='UTF-8' style='text-decoration: none'>";
    html += "<title>Sensor Data</title>";
    html += "<style>";
    html += "body {";
    html += "  background-color: #f2f2f2;";
    html += "  margin: 0;";
    html += "  padding: 0;";
    html += "  display: flex;";
    html += "  justify-content: center;";
    html += "  align-items: center;";
    html += "  height: 100vh;";
    html += "  font-family: Arial, sans-serif;";
    html += "}";
    html += ".container {";
    html += "  background-color: #ffffff;";
    html += "  padding: 40px;";
    html += "  border-radius: 10px;";
    html += "  box-shadow: 0 2px 5px rgba(0, 0, 0, 0.2);";
    html += "  text-align: center;";
    html += "  width: 80%;";
    html += "}";
    html += "h1 {";
    html += "  font-size: 66px;";
    html += "  margin-bottom: 30px;";
    html += "  color: #111010;";
    html += "}";
    html += "p {";
    html += "  font-size: 40px;";
    html += "  margin-bottom: 20px;";
    html += "  color: #060606;";
    html += "}";
    html += ".button {";
    html += "  display: inline-block;";
    html += "  background-color: #4CAF50;";
    html += "  color: #ffffff;";
    html += "  border: none;";
    html += "  width: 80%;";
    html += "  padding: 10px 20px;";
    html += "  border-radius: 5px;";
    html += "  font-size: 30px;";
    html += "  cursor: pointer;";
    html += "  transition: background-color 0.3s ease;";
    html += "}";
    html += ".button:hover {";
    html += "  background-color: #a09745;";
    html += "}";
    html += "</style>";
    html += "</head>";
    html += "<body>";
    html += "<div class=\"container\">";
    html += "<h1>Josline Plantation</h1>";
    html += "<p>Intensité lumineuse: <span id=\"ldrValue\">" + String(ldrValue) + "</span></p>";
    html += "<p>Température: <span id=\"temperature\">" + String(temperature) + "</span> °C</p>";
    html += "<p>Humidité: <span id=\"humidity\">" + String(humidity) + "</span> %</p>";
    html += "<p>Humidité du Sol: <span id=\"soilValue\">" + String(soilValue) + "</span></p>";
    html += "<button class=\"button\" onclick=\"toggleLED()\">POMPE</button>";
    html += "</div>";
html += "<script>";
html += "function updateSensorValues() {";
html += "  fetch(\"/sensor-values\")";
html += "    .then(response => response.json())";
html += "    .then(sensorValues => {";
html += "      document.getElementById(\"ldrValue\").innerHTML = sensorValues.ldrValue;";
html += "      document.getElementById(\"temperature\").innerHTML = sensorValues.temperature;";
html += "      document.getElementById(\"humidity\").innerHTML = sensorValues.humidity;";
html += "      document.getElementById(\"soilValue\").innerHTML = sensorValues.soilValue;";
html += "    });";
html += "}";
html += "setInterval(updateSensorValues, 5000);"; // Mettre à jour toutes les 5 secondes
html += "function toggleLED() {";
html += "  fetch(\"/toggle-led\")";
html += "    .then(response => response.json())";
html += "    .then(data => {";
html += "      if (data.ledState) {";
html += "        document.querySelector(\".button\").style.backgroundColor = \"#4CAF50\";";
html += "        document.querySelector(\".button\").innerHTML = \"POMPE\";";
html += "      } else {";
html += "        document.querySelector(\".button\").style.backgroundColor = \"#FF0000\";";
html += "        document.querySelector(\".button\").innerHTML = \"POMPE\";";
html += "      }";
html += "    });";
html += "}";
html += "</script>";
html += "</body>";
html += "</html>";

request->send(200, "text/html", html);
});

// Gestion de la requête pour obtenir les valeurs des capteurs
server.on("/sensor-values", HTTP_GET, [](AsyncWebServerRequest *request){
  String json = "{";
  json += "\"ldrValue\":" + String(ldrValue) + ",";
  json += "\"temperature\":" + String(temperature) + ",";
  json += "\"humidity\":" + String(humidity) + ",";
  json += "\"soilValue\":" + String(soilValue);
  json += "}";
  request->send(200, "application/json", json);
});

// Gestion de la requête pour basculer l'état de la LED
server.on("/toggle-led", HTTP_GET, [](AsyncWebServerRequest *request){
  digitalWrite(ledPin, !digitalRead(ledPin));
  String json = "{\"ledState\":" + String(digitalRead(ledPin)) + "}";
  request->send(200, "application/json", json);
});

// Démarrage du serveur web
server.begin();
}

void loop() {
  // Mesure de l'intensité lumineuse
  ldrValue = analogRead(ldrPin);

  // Mesure de la température et de l'humidité
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Mesure de l'humidité du sol
  soilValue = analogRead(soilPin);

  // Délai
  delay(1000);
}
