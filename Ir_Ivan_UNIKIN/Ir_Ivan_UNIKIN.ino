
// Bibliothèques
#include <SoftwareSerial.h>

// Initialisation du module GSM
SoftwareSerial GSM(0, 1); // SIM800L Tx & Rx is connected to Arduino #8 & #9

// Les portes
int P1 = A0;
int P2 = A1;
int P3 = A2;
int P4 = A3;
int Pir = 2;
// Moteur
int MT = 6;
int DM = 5;

// Déclaration du relais de mise en marche
int RELAY = 4;

// Numéro de téléphone
char phone_no[] = "+243997780844";

// Variable
char inchar;

void setup(){// put your setup code here, to run once

Serial.begin(9600);//Begin serial communication with Arduino and Arduino IDE (Serial Monitor)  
GSM.begin(9600);   //Begin serial communication with Arduino and SIM800L

pinMode(P1, INPUT_PULLUP);
pinMode(P2, INPUT_PULLUP);
pinMode(P3, INPUT_PULLUP);
pinMode(P4, INPUT_PULLUP);
pinMode(Pir, INPUT);
pinMode(DM, INPUT_PULLUP);
pinMode(MT, OUTPUT);
pinMode(RELAY, OUTPUT);

  // Initialisation du module GSM
  Serial.println("Initializing....");
  initModule("AT", "OK", 1000);
  initModule("ATE1", "OK", 1000);
  initModule("AT+CPIN?", "READY", 1000);
  initModule("AT+CMGF=1", "OK", 1000);
  initModule("AT+CNMI=2,2,0,0,0", "OK", 1000);
  Serial.println("Initialized Successfully"); 

}

void loop(){

if(GSM.available() >0){inchar=GSM.read(); Serial.print(inchar); 
 if(inchar=='R'){inchar=GSM.read(); 
 if(inchar=='I'){inchar=GSM.read();
 if(inchar=='N'){inchar=GSM.read();
 if(inchar=='G'){initModule("ATH","OK",1000); // this command is used for Call busy
    }
   }
  }
 }

else if(inchar=='M'){delay(10); inchar=GSM.read(); 
     if(inchar=='O'){delay(10); inchar=GSM.read();
     if(inchar=='T'){delay(10); inchar=GSM.read();

     if(inchar=='1'){digitalWrite(MT, 1); sendSMS(phone_no,"MOTEUR est activé");}
     if(inchar=='A'){digitalWrite(RELAY, 1); sendSMS(phone_no,"securité est activé");}
     if(inchar=='D'){digitalWrite(RELAY, 0); sendSMS(phone_no,"securité est desactivé");}
else if(inchar=='0'){digitalWrite(MT, 0); sendSMS(phone_no,"MOTEUR Désactivé");}
     }
    }
  }
}


if (digitalRead (P1)==LOW){sendSMS(phone_no,"PORTE AVANT GAUCHE ouverte ");}
if (digitalRead (P2)==LOW){sendSMS(phone_no,"PORTE AVANT DROITE ouverte");}
if (digitalRead (P3)==LOW){sendSMS(phone_no,"PORTE ARRIERE GAUCHE ouverte");}
if (digitalRead (P4)==LOW){sendSMS(phone_no,"PORTE ARRIERE DROITE ouverte");}
if (digitalRead (Pir)==HIGH){sendSMS(phone_no,"QUELQU'UN EST DANS LA VOITURE");}
if (digitalRead (DM)==LOW){callUp(phone_no);digitalWrite(MT,HIGH);}

delay(5);
}



void sendSMS(char *number, char *msg){
GSM.print("AT+CMGS=\"");GSM.print(number);GSM.println("\"\r\n"); //AT+CMGS=”Mobile Number” <ENTER> - Assigning recipient’s mobile number
delay(500);
GSM.println(msg); // Message contents
delay(500);
GSM.write(byte(26)); //Ctrl+Z  send message command (26 in decimal).
delay(3000);  
}

void callUp(char *number){
GSM.print("ATD + "); GSM.print(number); GSM.println(";"); //Call to the specific number, ends with semi-colon,replace X with mobile number
delay(20000);       // wait for 20 seconds...
GSM.println("ATH"); //hang up
delay(100);  
}


void initModule(String cmd, char *res, int t){
while(1){
    Serial.println(cmd);
    GSM.println(cmd);
    delay(100);
    while(GSM.available()>0){
       if(GSM.find(res)){
        Serial.println(res);
        delay(t);
        return;
       }else{Serial.println("Error");}}
    delay(t);
  }
}
