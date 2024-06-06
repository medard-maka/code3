#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "McMeder";
const char* password = "mcmeder91";

int xPin = 34;
int yPin = 35;
int buttonPin = 7;


WebServer server(80);

void handleRoot() {
  int xVal = analogRead(xPin);
  int yVal = analogRead(yPin);
  int buttonVal = digitalRead(buttonPin);

  String html = "<html>";
  html += "<head>";
  html += "<style>";
  html += "body {font-family: Arial, Helvetica, sans-serif;}";
  html += ".container {display: flex; justify-content: center; align-items: center; height: 100vh; flex-direction: column;}";
  html += ".cursor {position: relative; width: 200px; height: 200px; background-color: #f1f1f1; border-radius: 50%;}";
  html += ".info {margin-top: 20px;}";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class=\"info\">";
  html += "<h1>Position du curseur</h1>";
  html += "</body>";
  html += "</html>";


  server.send(200, "text/html", html);
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au WiFi en cours...");
  }
  Serial.println("Connecté au WiFi");

  // Affiche l'adresse IP sur le moniteur série
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();
}

void loop() {
  server.handleClient();
}
