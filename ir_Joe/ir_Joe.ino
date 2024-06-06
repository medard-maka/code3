// Inclure les bibliothèques nécessaires
#include <Servo.h>

// Définir les broches des capteurs et des actionneurs
const int capteurObstacle = 2;    // Exemple de broche pour le capteur d'obstacle
const int brossePin = 3;          // Exemple de broche pour le moteur de la brosse
const int servoPin = 4;           // Exemple de broche pour le servo-moteur
const int detecteurDechets = 5;   // Exemple de broche pour le capteur de déchets
const int brocheRelais = 6;       // Exemple de broche pour le relais de déchargement

// Déclarer les objets
Servo servo; // Créer un objet servo pour contrôler le servo-moteur

// Définir les constantes
const int angleMin = 0;   // Angle minimum pour le servo-moteur
const int angleMax = 180; // Angle maximum pour le servo-moteur
const int seuilDechets = 50; // Exemple de seuil pour déterminer si le sac est rempli

// Variable pour suivre l'état du sac
int capaciteSac = 0;

// Fonction pour initialiser les composants
void setup()
{
  // Initialiser les broches
  pinMode(capteurObstacle, INPUT);
  pinMode(brossePin, OUTPUT);
  pinMode(detecteurDechets, INPUT);
  pinMode(brocheRelais, OUTPUT);

  // Attacher le servo-moteur à la broche
  servo.attach(servoPin);
}

// Fonction pour balayer
void balayer()
{
  // Vérifier si un obstacle est détecté
  if (digitalRead(capteurObstacle) == HIGH)
  {
    // Arrêter la brosse
    digitalWrite(brossePin, LOW);

    // Tourner le servo-moteur à gauche
    servo.write(angleMin);
  }
  else
  {
    // Démarrer la brosse
    digitalWrite(brossePin, HIGH);

    // Tourner le servo-moteur à droite
    servo.write(angleMax);
  }
}

// Fonction pour collecter les déchets
void collecterDechets()
{
  // Vérifier si des déchets sont détectés par le capteur de proximité à effet Hall
  if (digitalRead(detecteurDechets) == HIGH)
  {
    capaciteSac++; // Incrémenter la capacité du sac

    // Vérifier si le sac est plein
    if (capaciteSac >= seuilDechets)
    {
      arreterBalayage(); // Arrêter le balayage une fois le sac plein
    }
  }
}

// Fonction pour décharger les déchets
void dechargerDechets()
{
  // Décharger les déchets dans un endroit approprié
  digitalWrite(brocheRelais, HIGH);
  delay(1000);
  digitalWrite(brocheRelais, LOW);

  capaciteSac = 0; // Réinitialiser la capacité du sac après le déchargement
}

// Fonction pour arrêter le balayage
void arreterBalayage()
{
  // Arrêter la brosse
  digitalWrite(brossePin, LOW);

  // Tourner le servo-moteur à la position centrale
  servo.write(angleMax / 2);

  // Surveiller en continu l'état des capteurs et des équipements
  while (digitalRead(detecteurDechets) == HIGH)
  {
    // Attendre que les déchets soient déchargés
    delay(1000);
  }

  dechargerDechets(); // Décharger les déchets une fois le sac vidé
}

// Fonction principale
void loop()
{
  // Appeler la fonction de balayage
  balayer();

  // Appeler la fonction pour collecter les déchets
  collecterDechets();
}
