
//Include libraries copied from VESC

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 #include "VescUart.h"
#include "datatypes.h"

#define OLED_RESET 4
/*-------------------------------------------DEBUT DE LA PARTIE A MODIFIER SELON VOTRE ESKATE (on ne change pas les noms de variables juste les valeurs)--------------------------*/
#define MAXCURRENT 20       //courant max moteur en Ampère
#define MAXBRAKECURRENT 10  //courant max au freinage moteur en Ampère
#define MAXVOLTAGE 42       // tension en Volt de la batterie quand elle est pleinement chargée (4,2 x nbre de S)
#define MINVOLTAGE 33       // tension en Volt de la batterie quand elle est vide (3,2 x nbre de S) perso j'ajoute 1V pour éviter la décharge profonde
#define RAYONROUES 45       // rayon des roues en millimètre
#define POLES 12            // nombre de PAIRES de poles du moteur 
#define RAPPORT 1           // rapport poulie-courroie (si hub moteur alors RAPPORT = 1)
/*-------------------------------------------FIN DE LA PARTIE A MODIFIER (vous pouvez modifier la suite si vous savez ce que vous faites à vos risques et périls !!!!!!) ----------------------------------------------*/
#define DEBUG

struct bldcMeasure measuredValues;
 float current;
 float brakeCurrent;
 byte niveauChargeSkate;
 int positionPotar;
 int niveauBatterie = 0;
 byte positionRect = 117;
 static float fullCharge;
static float coef ;
byte vitesse = 0;


Adafruit_SSD1306 display(OLED_RESET);

void setup() 
{
  fullCharge = MAXVOLTAGE - 1; // prise en compte de la chute de tension causé par les éléments entre la batterie et le VESC (BMS, PowerSwitch...)
  coef = 100 / (fullCharge - MINVOLTAGE); // coefficient pour le pourcentage de la batterie restante dans le skate
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C 
  display.clearDisplay();
  display.setTextColor(WHITE); 

 Serial.begin(115200);
}


	
// the loop function runs over and over again until power down or reset
void loop() {
  

  // récupération de la position de la gachette  
 positionPotar = analogRead(A0);
 
 // récupération du niveau de la batterie
 niveauBatterie = analogRead(A1);
   
    if (positionPotar >= 270)
    {
      current = (positionPotar - 270) * MAXCURRENT / 783;
      VescUartCanSetCurrent(current);
      VescUartSetCurrent(current);   
    }
    
   else if (positionPotar <= 245)
   { 
     brakeCurrent = MAXBRAKECURRENT - MAXBRAKECURRENT * positionPotar / 245 ;
     VescUartCanSetCurrentBrake(brakeCurrent);
     VescUartSetCurrentBrake(brakeCurrent);
   }
   
   else 
   { 
      current = 0;
     brakeCurrent = 0;
     
     VescUartCanSetCurrent(0);
     VescUartSetCurrent(0);
     VescUartCanSetCurrentBrake(0);
     VescUartSetCurrentBrake(0);
   }

 VescUartGetValue(measuredValues);
    
   if (measuredValues.inpVoltage >= fullCharge)
   {
     niveauChargeSkate = 100;
   }
   
    else if (measuredValues.inpVoltage <= MINVOLTAGE)
   {
     niveauChargeSkate = 0;
   }

   else
   {
     niveauChargeSkate = (byte)((measuredValues.inpVoltage-MINVOLTAGE)*coef); // calcul du pourcentage de charge du skate
   }
 // calcul de la vitesse en km/h  
   vitesse = (byte) ((measuredValues.rpm  / 1000)  * (325 / POLES) * RAPPORT * RAYONROUES / 1000)  ; // (325 = 120 * pi * coefficient correctif = 120 * pi * 0.862)
   
  display.clearDisplay();
  display.display();
  
 // affichage du niveau de charge du skate 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(niveauChargeSkate);
  display.println(F(" %")); 
  
//dessin du niveau de charge de la pile de la télécommande

  display.fillRect(92,3,3,2,WHITE);
  display.drawRect(95,0,28,8,WHITE);

  if (niveauBatterie >= 869 && niveauBatterie <= 1023)
  {
    positionRect = 97 ;
  }

  else if (niveauBatterie >= 818 && niveauBatterie < 869)
  {
    positionRect = 102 ;
  }

  else if (niveauBatterie >= 767 && niveauBatterie < 818)
  {
    positionRect = 107 ;
  }

  else if (niveauBatterie >= 716 && niveauBatterie < 767)
  {
     positionRect = 112 ;
  }

    else if (niveauBatterie >= 666 && niveauBatterie < 716)
  {
    positionRect = 117 ;
  }
  
  
  for (positionRect ; positionRect<=117 ; positionRect+=5)
  {
    display.setCursor(positionRect,1);
    display.fillRect(positionRect,1,4,6,WHITE);
  }

// affichage de la vitesse calculée 
  display.setTextSize(2); 
  display.setCursor(30,15);
  display.print(vitesse); 
  display.print(F("km/h"));   

    display.display();
    


}





