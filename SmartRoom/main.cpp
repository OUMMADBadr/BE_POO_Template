
#include <Arduino.h>

#include "SmartRoom.h"
#include "BlynkConfig.h"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// ======================================
// OBJETS
// ======================================

SmartRoom chambre;
BlynkTimer timer;

// Affiche une confirmation uniquement après la connexion effective au cloud.
BLYNK_CONNECTED()
{
    Serial.println("BLYNK_CONNECTED: appareil en ligne sur Blynk Cloud.");
}

// ======================================
// RECONNEXION BLYNK
// ======================================

unsigned long derniereTentativeBlynk = 0;

const unsigned long INTERVALLE_RECONNEXION = 10000;

// ======================================
// VERIFICATION CONNEXION BLYNK
// ======================================

void verifierConnexionBlynk()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Wi-Fi deconnecte !");
        WiFi.reconnect();
        return;
    }

    if (!Blynk.connected())
    {
        unsigned long maintenant = millis();

        if (maintenant - derniereTentativeBlynk >=
            INTERVALLE_RECONNEXION)
        {
            derniereTentativeBlynk = maintenant;

            Serial.println("Tentative de connexion Blynk...");

            if (Blynk.connect(3000))
            {
                Serial.println("Blynk connecte !");
            }
            else
            {
                Serial.println("Echec connexion Blynk.");
            }
        }
    }
}

// ======================================
// ENVOI DES DONNEES VERS BLYNK
// ======================================

void envoyerDonnees()
{
    if (!Blynk.connected())
    {
        return;
    }

    // TEMPERATURE : V0
    if (chambre.temperatureValide())
    {
        Blynk.virtualWrite(V0, chambre.getTemperature());
    }

    // HUMIDITE : V1
    if (chambre.temperatureValide())
    {
        Blynk.virtualWrite(V1, chambre.getHumidite());
    }

    // DISTANCE : V2
    if (chambre.distanceValide())
    {
        Blynk.virtualWrite(V2, chambre.getDistance());
    }

    // LUMINOSITE : V3
    if (chambre.luminositeValide())
    {
        Blynk.virtualWrite(V3, chambre.getLuminosite());
    }

    // ALERTE TEMPERATURE : V4
    Blynk.virtualWrite(
        V4,
        chambre.getAlerteTemperature() ? 1 : 0
    );

    // ETAT DE LA PORTE : V6
    Blynk.virtualWrite(
        V6,
        chambre.porteOuverte() ? 1 : 0
    );

    // ETAT DE LA LED : V7
    Blynk.virtualWrite(
        V7,
        chambre.getEtatLed() ? 1 : 0
    );

    Serial.println("Donnees envoyees vers Blynk.");
}

// ======================================
// INITIALISATION
// ======================================

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("===== SMART ROOM =====");

    // Initialisation du systeme
    chambre.initialiser();

    // Connexion Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.println("Connexion au hotspot...");

    unsigned long debutConnexion = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - debutConnexion < 20000)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Wi-Fi connecte !");
        Serial.print("Adresse IP : ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println("Echec connexion Wi-Fi.");
    }

    // Configuration explicite du serveur Blynk Cloud.
    Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 8080);
    Serial.println("Blynk configure sur blynk.cloud:8080.");

    // Empeche Blynk.run() de lancer une tentative automatique au premier tour.
    // La connexion sera lancee avec timeout par le controle periodique.
    Blynk.disconnect();

    // La connexion Blynk sera tentee par le controle periodique.
    derniereTentativeBlynk = millis() - INTERVALLE_RECONNEXION;

    // Le controle tourne souvent, mais les tentatives restent espacees de 10 s.
    timer.setInterval(1000L, verifierConnexionBlynk);

    // Envoi des mesures toutes les 2 secondes
    timer.setInterval(2000L, envoyerDonnees);

    Serial.println("SMART ROOM PRET !");
}

// ======================================
// BOUCLE PRINCIPALE
// ======================================

void loop()
{
    // Mise a jour des capteurs et actionneurs
    chambre.mettreAJour();

    // Lancer le controle avant Blynk.run(), qui peut attendre le serveur.
    timer.run();

    // Blynk.run() doit tourner souvent pour entretenir la connexion au cloud.
    Blynk.run();
}
