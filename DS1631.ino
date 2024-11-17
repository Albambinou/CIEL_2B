#include <Wire.h>

const int led_R = 5;
const int led_V = 6;
const int led_B = 7;
const int therm = 9;

// Définition de l'adresse des capteurs
#define DS1631_ADDR 0x94 >> 1
#define DS1631_ADDR2 0x90 >> 1

//*************************************************
//                  SETUP
//*************************************************
void setup() 
{
  pinMode(led_R, OUTPUT);
  pinMode(led_V, OUTPUT);
  pinMode(led_B, OUTPUT);
  pinMode(therm, INPUT);
  
    // Établissement de la connexion série PC <--> Arduino
    Serial.begin(9600);
    Serial.println();
    Serial.println("Configuration du capteur ...");
    Serial.println();
    Serial.println("_______________________________");
    Serial.println(" DS1631 Capteur de température ");
    Serial.println("_______________________________");
    Serial.println();

    // On rejoint le bus I2C comme maître
    Wire.begin();

    // L'accès au registre de configuration suppose qu'on arrête toutes les mesures en cours de conversion
    /////////////////////////////////////////////////////////////////////////////////////////////
    Wire.beginTransmission(DS1631_ADDR);
    Wire.write(0x22); // La conversion courante est stoppée.
    int error1 = Wire.endTransmission();

    if (error1 != 0)
    {
        Serial.print("Erreur détectée, valeur: ");
        Serial.println(error1);
        Serial.println("Le composant n'est pas détecté, vérifier votre câblage 1 (adresse...");
        delay(5000);
    }

    //_____________________________________________________________________________________________
    Wire.beginTransmission(DS1631_ADDR2);
    Wire.write(0x22); // La conversion courante est stoppée.
    int error2 = Wire.endTransmission();    

    if (error2 != 0)
    {
        Serial.print("Erreur détectée, valeur: ");
        Serial.println(error2);
        Serial.println("Le composant n'est pas détecté, vérifier votre câblage 2 (adresse...");
        delay(5000);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////
    // Lecture de la configuration courante
    Wire.beginTransmission(DS1631_ADDR);
    Wire.write(0xAC); // Code commande @Acess Config
    Wire.endTransmission();

    Wire.requestFrom(DS1631_ADDR, 1);
    Wire.available();
    int ancienne_config = Wire.read();

    Serial.print("Configuration actuelle avant modification 1 : 0x");
    Serial.print(ancienne_config, HEX);
    Serial.println();

    // Écriture de la data de configuration dans le registre config
    Wire.beginTransmission(DS1631_ADDR);
    Wire.write(0xAC);
    Wire.write(0x0C); 
    Wire.endTransmission();

    // Lecture de la configuration actuelle après modification
    Wire.requestFrom(DS1631_ADDR, 1);
    Wire.available();
    int config_actuelle = Wire.read();

    Serial.print("Configuration actuelle après modification 1 : 0x");
    Serial.print(config_actuelle, HEX);
    Serial.println();

    // Début de la conversion de T°
    Wire.beginTransmission(DS1631_ADDR);
    Wire.write(0x51); // Code commande de start
    Wire.endTransmission();

    /////////////////////////////////////////////////////////////////////////////////////////////
    // Lecture de la configuration courante
    Wire.beginTransmission(DS1631_ADDR2);
    Wire.write(0xAC); // Code commande @Acess Config
    Wire.endTransmission();

    Wire.requestFrom(DS1631_ADDR2, 1);
    Wire.available();
    int ancienne_config2 = Wire.read();

    Serial.print("Configuration actuelle avant modification 2 : 0x");
    Serial.print(ancienne_config2, HEX);
    Serial.println();

    // Écriture de la data de configuration dans le registre config
    Wire.beginTransmission(DS1631_ADDR2);
    Wire.write(0xAC);
    Wire.write(0x0C); 
    Wire.endTransmission();

    // Lecture de la configuration actuelle après modification
    Wire.requestFrom(DS1631_ADDR2, 1);
    Wire.available();
    int config_actuelle2 = Wire.read();

    Serial.print("Configuration actuelle après modification 2 : 0x");
    Serial.print(config_actuelle2, HEX);
    Serial.println();

    // Début de la conversion de T°
    Wire.beginTransmission(DS1631_ADDR2);
    Wire.write(0x51); // Code commande de start
    Wire.endTransmission();

   // Début de la conversion de T°
    Wire.beginTransmission(DS1631_ADDR2);
    Wire.write(0x51); // Code commande pour démarrer la conversion
    Wire.endTransmission();

 // Configuration de Tl (température basse)
    Wire.beginTransmission(DS1631_ADDR);
    Wire.write(0xA2);     // Code commande pour écrire Tl
    Wire.write(0x18);     // Partie entière
    Wire.write(0x80);     // Partie décimale
    Wire.endTransmission();

 // Configuration de Th (température haute)
    Wire.beginTransmission(DS1631_ADDR);
    Wire.write(0xA1);     // Code commande pour écrire Th
    Wire.write(0x19);     // Partie entière
    Wire.write(0x80);     // Partie décimale
    Wire.endTransmission();

}

/////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
    Serial.println("____________________________________________________");

    // Lecture de la température
    Wire.beginTransmission(DS1631_ADDR);
    Wire.write(0xAA); // Code commande Lecture T°
    Wire.endTransmission();

    // Lecture de 2 octets
    Wire.requestFrom(DS1631_ADDR, 2);
    Serial.println("Capteur 1 :");
    Serial.print("Octet en attente de lecture : ");
    Serial.println(Wire.available());

    int T_MSB = Wire.read();
    int T_LSB = Wire.read();
    
    // On affiche les valeurs binaires brutes
    Serial.print("T_MSB = ");
    Serial.println(T_MSB, BIN);
    Serial.print("T_LSB = ");
    Serial.println(T_LSB, BIN);

    Wire.beginTransmission(DS1631_ADDR2);
    Wire.write(0xAA); // Code commande Lecture T°
    Wire.endTransmission();

    // Lecture de 2 octets
    Wire.requestFrom(DS1631_ADDR2, 2);
    Serial.println("--------------------------------");
    Serial.println("Capteur 2 :");
    Serial.print("Octet en attente de lecture : ");
    Serial.println(Wire.available());

    int T_MSB2 = Wire.read();
    int T_LSB2 = Wire.read();

    // On affiche les valeurs binaires brutes
    Serial.print("T_MSB2 = ");
    Serial.println(T_MSB2, BIN);
    Serial.print("T_LSB2 = ");
    Serial.println(T_LSB2, BIN);
    Serial.println("--------------------------------");
    
    // Calcul de la partie entière
    int partie_entiere = T_MSB & 0b01111111;
    int partie_entiere2 = T_MSB2 & 0b01111111;

    float partie_decimale = 0.00;
    float partie_decimale2 = 0.00;
    
    unsigned int masque = 0x80;
    unsigned int masque2 = 0x80;

    float poids = 0.5;
    float poids2 = 0.5;
    
    for (int i = 0; i < 4; i++)
    {
        if ((T_LSB & masque) == masque)
        {
            partie_decimale += poids;
        }
        masque = masque >> 1;
        poids = poids / 2;
    }

    Serial.print("Valeur décimale correspondante : Capteur 1 = ");
    // On affiche le signe moins si le signe est à 1
    if (T_MSB >= 0x80) Serial.print("-");

    // Et la température
    Serial.print((T_MSB & 0b01111111) + partie_decimale, 1);
    Serial.print(" °C");
    Serial.println();

    for (int j = 0; j < 4; j++)
    {
        if ((T_LSB2 & masque2) == masque2)
        {
            partie_decimale2 += poids2;
        }
        masque2 = masque2 >> 1;
        poids2 = poids2 / 2;
    }

    Serial.print("Valeur décimale correspondante : Capteur 2 = ");
    // On affiche le signe moins si le signe est à 1
    if (T_MSB2 >= 0x80) Serial.print("-");

    // Et la température
    Serial.print((T_MSB2 & 0b01111111) + partie_decimale2, 1);
    Serial.print(" °C");
    Serial.println();

    if (digitalRead(therm) == LOW)
    {
      analogWrite(led_R , 150);
      analogWrite(led_V, 0);
      analogWrite(led_B, 0);
    }
    else if (digitalRead(therm) == HIGH)
    {
      analogWrite(led_R, 0);
      analogWrite(led_V, 150);
      analogWrite(led_B, 0);
    }
    else
    {
      analogWrite(led_R, 0);
      analogWrite(led_V, 0);
      analogWrite(led_B, 0);
    }
    delay(1000);
}
