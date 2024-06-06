#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // Utilisation des broches 12, 11, 5, 4, 3 et 2 pour le LCD

const int sensorPin1 = A0;  // Broche analogique pour le premier capteur
const int sensorPin2 = A1;  // Broche analogique pour le deuxième capteur
const int sensorPin3 = A2;  // Broche analogique pour le troisième capteur

const int relayPin1 = 6;  // Broche de commande du relais 1
const int relayPin2 = 7;  // Broche de commande du relais 2
const int relayPin3 = 8;  // Broche de commande du relais 3

const int numSamples = 10;  // Nombre d'échantillons à prendre pour chaque capteur

void setup() {
  lcd.begin(16, 2);  // Initialisation de l'écran LCD
  Serial.begin(9600);  // Initialisation du moniteur série

  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);

  lcd.print("Mesure de tension");
  lcd.setCursor(0, 1);
  lcd.print("en cours...");
  delay(2000);
  lcd.clear();
}

void loop() {
  float total1 = 0.0;
  float total2 = 0.0;
  float total3 = 0.0;

  for (int i = 0; i < numSamples; i++) {
    total1 += analogRead(sensorPin1);
    total2 += analogRead(sensorPin2);
    total3 += analogRead(sensorPin3);
    delay(10);
  }

  float average1 = map(total1 / numSamples, 0, 1023, 0, 230);
  float average2 = map(total2 / numSamples, 0, 1023, 0, 230);
  float average3 = map(total3 / numSamples, 0, 1023, 0, 230);

  lcd.setCursor(0, 0);
  lcd.print("U1:");
  lcd.print(average1);
  lcd.print("V");

  lcd.setCursor(6, 0);
  lcd.print("U2: ");
  lcd.print(average2);
  lcd.print("V");

  lcd.setCursor(0, 1);
  lcd.print("U3: ");
  lcd.print(average3);
  lcd.print("V");

  Serial.print("U1: ");
  Serial.print(average1);
  Serial.print("V\t");

  Serial.print("U2: ");
  Serial.print(average2);
  Serial.print("V\t");

  Serial.print("U3: ");
  Serial.print(average3);
  Serial.println("V");

  delay(1500);
  lcd.clear();

  // Comparaison des tensions pour trouver la plus élevée dans la plage de 200V à 230V
  if (average1 >= 200 && average1 <= 230 && average1 >= average2 && average1 >= average3) {
    digitalWrite(relayPin1, HIGH);
    digitalWrite(relayPin2, LOW);
    digitalWrite(relayPin3, LOW);
      lcd.setCursor(9, 1);
  lcd.print("U1");
  Serial.println("U1");
  } else if (average2 >= 200 && average2 <= 230 && average2 >= average1 && average2 >= average3) {
    digitalWrite(relayPin1, LOW);
    digitalWrite(relayPin2, HIGH);
    digitalWrite(relayPin3, LOW);
      lcd.setCursor(9, 1);
  lcd.print("U2");
  Serial.println("U2");
  } else if (average3 >= 200 && average3 <= 230 && average3 >= average1 && average3 >= average2) {
    digitalWrite(relayPin1, LOW);
    digitalWrite(relayPin2, LOW);
    digitalWrite(relayPin3, HIGH);
      lcd.setCursor(9, 1);
  lcd.print("U3");
  Serial.println("U3");
  }
  else {
    digitalWrite(relayPin1, LOW);
    digitalWrite(relayPin2, LOW);
    digitalWrite(relayPin3, LOW);
  }
}
