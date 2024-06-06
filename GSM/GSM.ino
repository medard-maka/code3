#include <SoftwareSerial.h>

SoftwareSerial gsm(7, 8);  // Utilisez les broches appropriées pour la communication avec le module GSM

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);

  delay(2000);  // Attendez que le module GSM soit prêt

  // Configurez le module GSM pour se connecter au réseau
  gsm.println("AT");  // Vérifiez la communication avec le module
  delay(1000);
  gsm.println("AT+CFUN=1");  // Activez le module GSM
  delay(3000);
  gsm.println("AT+COPS=1,2,\"Airtel Congo\"");  // Sélectionnez manuellement l'opérateur réseau (Airtel Congo en RDC)
  delay(5000);
  gsm.println("AT+CGATT=1");  // Attachez-vous au réseau GPRS
  delay(5000);
  gsm.println("AT+CGDCONT=1,\"IP\",\"internet\"");  // Configurez l'APN (internet pour Airtel Congo en RDC)
  delay(1000);
}

void loop() {
  // Envoyez un message SMS
  gsm.println("AT+CMGF=1");  // Configurez le mode SMS
  delay(1000);
  gsm.println("AT+CMGS=\"+243975556493\"");  // Spécifiez le numéro de téléphone du destinataire (remplacez "+243975556493" par votre numéro de téléphone)
  delay(1000);
  gsm.print("Bonjour, ceci est un test");  // Contenu du message
  delay(100);
  gsm.write(0x1A);  // Envoyer le caractère de fin de message
  delay(5000);

  // Vérifiez si le message a été envoyé avec succès
  bool messageSent = false;
  while (gsm.available()) {
    String response = gsm.readString();
    if (response.indexOf("OK") != -1) {
      messageSent = true;
      break;
    }
  }

  if (messageSent) {
    Serial.println("Le message a été envoyé avec succès !");
  } else {
    Serial.println("Erreur lors de l'envoi du message.");
  }

  delay(5000);
}
