#include <SoftwareSerial.h>    
SoftwareSerial GSM(8, 9); 

char phone_no[]="+243979356229";  

#define bt_M  2 
#define BZ  4
char inchar; 

void setup(){

Serial.begin(9600);  
GSM.begin(9600);   

pinMode(bt_M, INPUT_PULLUP); 
  
Serial.println("Initializing....");
initModule("AT","OK",1000);                
initModule("ATE1","OK",1000);             
initModule("AT+CPIN?","READY",1000);    
initModule("AT+CMGF=1","OK",1000);        
initModule("AT+CNMI=2,2,0,0,0","OK",1000);  
Serial.println("Initialized Successfully"); 

}

void loop(){

if(GSM.available() >0){inchar=GSM.read(); Serial.print(inchar); 
     if(inchar=='A'){delay(10); inchar=GSM.read(); 
     if(inchar=='L'){delay(10); inchar=GSM.read();
     if(inchar=='T'){delay(10); inchar=GSM.read();

     if(inchar=='1'){digitalWrite (BZ,HIGH); sendSMS(phone_no,"alerte est activée");}
else if(inchar=='0'){digitalWrite (BZ,LOW); sendSMS(phone_no,"alerte Désactivée");}
     }
    }
  }
}


if(digitalRead (bt_M) == 0){sendSMS(phone_no,"Quelqu'un vient de forcer la porte svp");
}


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
