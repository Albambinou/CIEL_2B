#include <SPI.h>
#include <SD.h>
#include <DHT.h>

// ===============================
// Configuration du capteur DHT11
// ===============================

#define DHTPIN 2          // Pin de données du capteur DHT11
#define DHTTYPE DHT11     // Type de capteur DHT11

DHT dht(DHTPIN, DHTTYPE);  // Création de l'objet DHT pour accéder au capteur

// ===============================
// Variables pour la carte SD
// ===============================

#define sdCardPinChipSelect 53  // Pin de sélection de la carte SD
#define nomDuFichier "degrev2.xls"  // Nom du fichier pour enregistrer les données

File monFichier;  // Déclaration de l'objet fichier pour écrire sur la carte SD

// ===============================
// Variable pour le numéro de la température
// ===============================

int temperatureNumber = 1;  // Compteur de mesure (numéro de la température)


// ===============================
// Initialisation (setup)
// ===============================

void setup() {
  
  // Initialisation de la liaison série pour afficher les messages
  Serial.begin(9600);
  Serial.println(F("Programme de mesure de température avec DHT11 et enregistrement sur Carte SD"));

  // ===============================
  // Initialisation du capteur DHT11
  // ===============================
  dht.begin();

  // ===============================
  // Initialisation de la carte SD
  // ===============================
  Serial.println(F("Initialisation de la carte SD :"));
  
  if (!SD.begin(sdCardPinChipSelect))
  {
    // Si l'initialisation échoue, afficher un message d'erreur et stopper le programme
    Serial.println(F("Échec de l'initialisation !"));
    while (1);  // Bloque l'exécution pour éviter tout autre action
  }
  
  Serial.println(F("Initialisation terminée."));

  // ===============================
  // Création du fichier sur la carte SD
  // ===============================
  monFichier = SD.open(nomDuFichier, FILE_WRITE);
  
  if (monFichier)
  {
    // Si le fichier s'ouvre correctement, ajouter l'en-tête
    Serial.println(F("Fichier '" nomDuFichier "' ouvert avec succès."));
    monFichier.println("Numéro de mesure,Température (°C),Humidité");  // En-tête des données
    monFichier.close();  // Fermer le fichier après l'écriture de l'en-tête
  }
  else
  {
    // Si l'ouverture du fichier échoue, afficher un message d'erreur et stopper le programme
    Serial.println(F("Échec d'ouverture du fichier pour écriture."));
    while (1);  // Bloque l'exécution
  }
}


// ===============================
// Boucle principale (loop)
// ===============================

void loop() {
  
  // ===============================
  // Lecture de la température du capteur DHT11
  // ===============================
  float temperature = dht.readTemperature();
  float humidite = dht.readHumidity();
  
  if (isnan(temperature))
  {
    // Vérifier si la lecture a échoué (retourne NaN si erreur)
    Serial.println(F("Erreur de lecture du capteur DHT11 !"));
    return;  // Quitte la boucle pour éviter de continuer avec des valeurs invalides
  }

  // ===============================
  // Écriture des données dans le fichier
  // ===============================
  monFichier = SD.open(nomDuFichier, FILE_WRITE);
  
  if (monFichier)
  {
    // Si le fichier s'ouvre correctement, écrire les données
    monFichier.print(temperatureNumber);  // Numéro de la mesure (température)
    monFichier.print(",");  // Séparateur entre le numéro et la température
    monFichier.print(temperature);  // Température mesurée
    monFichier.print(",");
    monFichier.println(humidite);     // Humiditée mesurée
    monFichier.close();  // Fermer le fichier après écriture

    // Affichage des données enregistrées sur le moniteur série
    Serial.print(F("Données enregistrées : "));
    Serial.print(temperatureNumber);
    Serial.print(F(" - "));
    Serial.print(temperature);
    Serial.print(F(" °C"));
    Serial.print(F(" - "));
    Serial.print(humidite);
    Serial.println(F(" %"));

    // ===============================
    // Incrémentation du numéro de mesure
    // ===============================
    temperatureNumber++;  // Incrémente le numéro de mesure pour la prochaine lecture
  }
  else
  {
    // Si l'ouverture du fichier échoue, afficher un message d'erreur
    Serial.println(F("Échec d'ouverture du fichier pour écriture."));
  }

  // ===============================
  // Attendre avant de prendre une nouvelle mesure
  // ===============================
  delay(1000);  // Attendre 1 seconde avant la prochaine lecture
}
