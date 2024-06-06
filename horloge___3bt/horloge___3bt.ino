#include "Wire.h"
#include <LiquidCrystal.h>
#define DS3231_I2C_ADDRESS 0x68
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int etatActuel = 0;
int etatUltime = 0;
int compteur = 0;
int MoinsAnterieur =0;
int etatPlus = 0;
int etatMoins = 0;
int PlusAnterieur =0;

byte decToBcd(byte val){
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val){
  return ( (val/16*10) + (val%16) );
} 

void TrouverDate(byte *secondes,byte *minutes,byte *heure,byte *jourSemaine,byte *jourMois,byte *mois,byte *annee){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
 
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
 
  *secondes     = bcdToDec(Wire.read() & 0x7f);
  *minutes      = bcdToDec(Wire.read());
  *heure         = bcdToDec(Wire.read() & 0x3f);
  *jourSemaine    = bcdToDec(Wire.read());
  *jourMois       = bcdToDec(Wire.read());
  *mois          = bcdToDec(Wire.read());
  *annee         = bcdToDec(Wire.read());
}
 
void VoirDate(){
    byte secondes, minutes, heure, jourSemaine, jourMois, mois, annee;
    String s, m, d, mth, h, mty;
     
    TrouverDate(&secondes, &minutes, &heure, &jourSemaine, &jourMois, &mois, &annee);
   
    if (secondes < 10) { s = "0" + String(secondes); } else { s = String(secondes); }
    if (minutes < 10) { m = "0" + String(minutes); } else { m = String(minutes); }
    h = String(heure);
    if (jourMois < 10) { d = "0" + String(jourMois); } else { d = String(jourMois); }
    if (mois < 10) { mth = "0" + String(mois); } else { mth = String(mois); }
    if (annee < 10) { mty = "0" + String(annee); } else { mty = String(annee); }
     
    char* days[] = { "NA", "Lu", "Ma", "Me", "Je", "Ve", "Sa", "Di" };
     
    lcd.clear();
    lcd.noBlink();
    
    lcd.setCursor(4,0);
    
    lcd.print(h + ":" + m + ":" + s);
    
    lcd.setCursor(1,1);
    
    lcd.print(String(days[jourSemaine]) + " " + d + "/" + mth + "/20" + mty);   
}

void modifier_temps(int type){
  byte secondes, minutes, heure, jourSemaine, jourMois, mois, annee;
  TrouverDate(&secondes, &minutes, &heure, &jourSemaine, &jourMois, &mois, &annee);
  Serial.println("pressé");
  int addition = 0;
  int soustraction = 0;

  if(etatMoins != MoinsAnterieur){
    if(etatMoins == HIGH){
      soustraction = 1;
    }
  }
  if(etatPlus != PlusAnterieur){
    if(etatPlus == HIGH){
      addition = 1;
    }
  }
  
  if(type == 1){ // modifier l'heure
    String h;
    h = String(heure);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Modifier l'heure");
    lcd.setCursor(0, 1);
    lcd.print(h); 
    lcd.blink();
    if(soustraction == 1){
      heure = heure-1;
    }
    if(addition == 1){
      heure = heure+1;
    }
    if(heure > 23) heure=0;  
  }

  if(type == 2){ // modifier les minutes
    String m;
    m = String(minutes);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Modifier minutes");
    lcd.setCursor(0, 1);
    lcd.print(m); 
    lcd.blink();
    if(soustraction == 1){
      minutes = minutes-1;
    }
    if(addition == 1){
      minutes = minutes+1;
    }
    if(minutes > 59) minutes=0;  
  }

  if(type == 3){ // modifier le jour de la semaine
    String d;
    d = String(jourSemaine);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Modifier jour semaine");
    lcd.setCursor(0, 1);
    lcd.print(d); 
    lcd.blink();
    if(soustraction == 1){
      jourSemaine = jourSemaine-1;
    }
    if(addition == 1){
      jourSemaine = jourSemaine+1;
    }
    if(jourSemaine == 8) jourSemaine=1;
    if(jourSemaine == 0) jourSemaine=7;  
  }

  if(type == 4){ // modifier le jour du mois
    String m;
    m = String(jourMois);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Modifier jour mois");
    lcd.setCursor(0, 1);
    lcd.print(m); 
    lcd.blink();
    if(soustraction == 1){
      jourMois = jourMois-1;
    }
    if(addition == 1){
      jourMois = jourMois+1;
    }
    if(jourMois == 32) jourMois=1;
    if(jourMois == 0) jourMois=31;  
  }

  if(type == 5){ // modifier mois
    String ms;
    ms = String(mois);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Modifier mois");
    lcd.setCursor(0, 1);
    lcd.print(ms); 
    lcd.blink();
    if(soustraction == 1){
      mois = mois-1;
    }
    if(addition == 1){
      mois = mois+1;
    }
    if(mois == 13) mois=1;
    if(mois == 0) mois=12;  
  }

  if(type == 6){ // modifier annee
    String a;
    a = String(annee);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Modifier annee");
    lcd.setCursor(0, 1);
    lcd.print(a); 
    lcd.blink();
    if(soustraction == 1){
      annee = annee-1;
    }
    if(addition == 1){
      annee = annee+1;
    }
    if(annee == 100) annee=1;
    if(annee == 0) annee=99;  
  }
    
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);
  Wire.write(decToBcd(secondes));
  Wire.write(decToBcd(minutes));
  Wire.write(decToBcd(heure));
  Wire.write(decToBcd(jourSemaine));
  Wire.write(decToBcd(jourMois));
  Wire.write(decToBcd(mois));
  Wire.write(decToBcd(annee));
  Wire.endTransmission();
}

void setup(){
  byte secondess, minutes, heure, jourSemaine, jourMois, mois, annee;
  Wire.begin();
  lcd.begin(16, 2);
  //Serial.begin(9600);
  pinMode(6,INPUT);  // bouton - menu
  pinMode(5,INPUT);  // bouton - moins
  pinMode(4,INPUT);  // bouton - plus
}
 
void loop(){
  etatActuel=digitalRead(6);
  etatMoins=digitalRead(5);
  etatPlus=digitalRead(4);
  //Serial.println(etatActuel);
  if(etatActuel != etatUltime){
    if(etatActuel == HIGH){
      compteur = compteur +1;
      //Serial.println(compteur);
    }
  }
  
  if(compteur == 0){
    VoirDate();
    delay(1000);
  }else if(compteur == 1){
    modifier_temps(1);  // modifier l'heure
    delay(500);
  }else if(compteur == 2){
    modifier_temps(2);  // modifier les minutes
    delay(500);
  }else if(compteur == 3){
    modifier_temps(3);  // modifier jour de la semaine
    delay(500);
  }else if(compteur == 4){
    modifier_temps(4);  // modifier jour du mois
    delay(500);
  }else if(compteur == 5){
    modifier_temps(5);  // modifier mois
    delay(500);
  }else if(compteur == 6){
    modifier_temps(6);  // modifier annee
    delay(500);
  }else if(compteur == 7){
    compteur = 0;
    delay(1000);
  }
}
