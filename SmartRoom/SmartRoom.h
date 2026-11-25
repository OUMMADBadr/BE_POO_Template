
#ifndef SMART_ROOM_H
#define SMART_ROOM_H

#include <Arduino.h>

#include "CapteurSHT31.h"
#include "CapteurUltrason.h"
#include "CapteurLuminosite.h"

#include "MoteurPorte.h"
#include "LED.h"
#include "Buzzer.h"

#include "AfficheurLCD.h"
#include "PorteAutomatique.h"

class SmartRoom {

private:

    // Broches des composants
    static const uint8_t PIN_ULTRASON = D5;
    static const uint8_t PIN_SERVO = D6;
    static const uint8_t PIN_LED = D7;
    static const uint8_t PIN_BUZZER = D3;

    // Creation des capteurs
    CapteurSHT31 temperature;
    CapteurUltrason ultrason;
    CapteurLuminosite luminosite;

    // Creation des actionneurs
    MoteurPorte moteur;
    LED led;
    Buzzer buzzer;

    // Autres composants
    AfficheurLCD afficheur;
    PorteAutomatique porte;

    // Mesures
    float temperatureMesuree;
    float humiditeMesuree;
    float distanceMesuree;
    float luminositeMesuree;

    bool mesureTemperatureValide;
    bool mesureDistanceValide;
    bool mesureLuminositeValide;

    // Etats des actionneurs et alertes
    bool etatLed;
    bool alerteTemperature;

    // Gestion du temps
    unsigned long derniereLecture;
    unsigned long dernierAffichage;

    int pageLCD;

    const unsigned long intervalleLecture = 500;
    const unsigned long intervalleAffichage = 2500;

public:

    // ======================================
    // CONSTRUCTEUR
    // ======================================

    SmartRoom()

        : ultrason(PIN_ULTRASON),
          luminosite(A0),
          moteur(PIN_SERVO),
          led(PIN_LED),
          buzzer(PIN_BUZZER),
          porte(moteur) {

        temperatureMesuree = 0;
        humiditeMesuree = 0;
        distanceMesuree = 0;
        luminositeMesuree = 0;

        mesureTemperatureValide = false;
        mesureDistanceValide = false;
        mesureLuminositeValide = false;

        etatLed = false;
        alerteTemperature = false;

        derniereLecture = 0;
        dernierAffichage = 0;

        pageLCD = 0;
    }

    // ======================================
    // INITIALISATION GENERALE
    // ======================================

    void initialiser() {

        Serial.begin(115200);

        Serial.println();
        Serial.println("======================");
        Serial.println("     SMART ROOM");
        Serial.println("======================");

        // Initialisation du bus I2C
        Wire.begin(D2, D1);

        // Initialisation des capteurs
        temperature.initialiser();
        ultrason.initialiser();
        luminosite.initialiser();

        // Initialisation des actionneurs
        moteur.initialiser();
        led.initialiser();
        buzzer.initialiser();

        // Initialisation de l'afficheur
        afficheur.initialiser();

        // Initialisation de la porte
        porte.initialiser();

        delay(1500);

        Serial.println("SMART ROOM PRET !");
    }

    // ======================================
    // LECTURE DES CAPTEURS
    // ======================================

    void lireCapteurs() {

        mesureTemperatureValide =
            temperature.lireValeur(temperatureMesuree);

        if (mesureTemperatureValide) {

            humiditeMesuree =
                temperature.getHumidite();
        }

        mesureDistanceValide =
            ultrason.lireValeur(distanceMesuree);

        mesureLuminositeValide =
            luminosite.lireValeur(luminositeMesuree);

        // Affichage des mesures sur le moniteur serie
        if (mesureTemperatureValide) {

            Serial.print("Temperature : ");
            Serial.print(temperatureMesuree);
            Serial.print(" C | Humidite : ");
            Serial.print(humiditeMesuree);
            Serial.println(" %");
        }

        if (mesureDistanceValide) {

            Serial.print("Distance : ");
            Serial.print(distanceMesuree);
            Serial.println(" cm");
        }

        if (mesureLuminositeValide) {

            Serial.print("Luminosite : ");
            Serial.println(luminositeMesuree);
        }
    }

    // ======================================
    // GESTION DE LA PORTE
    // ======================================

    void gererPorte() {

        if (mesureDistanceValide) {

            porte.mettreAJour(distanceMesuree);
        }
    }

    // ======================================
    // GESTION DE L'ECLAIRAGE
    // ======================================

    void gererEclairage() {

        if (!mesureLuminositeValide) {
            return;
        }

        // Seuil initial a calibrer
        if (luminosite.estSombre(500)) {

            led.activer();
            etatLed = true;
        }
        else {

            led.desactiver();
            etatLed = false;
        }
    }

    // ======================================
    // GESTION DES ALERTES
    // ======================================

    void gererAlertes() {

        alerteTemperature =
            mesureTemperatureValide &&
            temperatureMesuree > 30.0;

        if (alerteTemperature) {

            buzzer.activer();
        }
        else {

            buzzer.desactiver();
        }
    }

    // ======================================
    // AFFICHAGE LCD
    // ======================================

    void actualiserLCD() {

        if (millis() - dernierAffichage <
            intervalleAffichage) {
            return;
        }

        dernierAffichage = millis();

        if (mesureTemperatureValide &&
            temperatureMesuree > 30.0) {

            afficheur.afficherAlerte(
                "Temp trop elevee"
            );

            return;
        }

        if (pageLCD == 0) {

            if (mesureTemperatureValide) {

                afficheur.afficherEnvironnement(
                    temperatureMesuree,
                    humiditeMesuree
                );
            }
            else {

                afficheur.afficherAlerte(
                    "Erreur SHT31"
                );
            }

            pageLCD = 1;
        }

        else {

            if (mesureDistanceValide) {

                afficheur.afficherPorte(
                    distanceMesuree,
                    porte.estOuverte()
                );
            }
            else {

                afficheur.afficherAlerte(
                    "Erreur distance"
                );
            }

            pageLCD = 0;
        }
    }

    // ======================================
    // FONCTION PRINCIPALE
    // ======================================

    void mettreAJour() {

        if (millis() - derniereLecture >=
            intervalleLecture) {

            derniereLecture = millis();

            lireCapteurs();

            gererPorte();

            gererEclairage();

            gererAlertes();
        }

        actualiserLCD();
    }

    // ======================================
    // ACCES AUX DONNEES POUR BLYNK
    // ======================================

    // Temperature
    float getTemperature() {
        return temperatureMesuree;
    }

    // Humidite
    float getHumidite() {
        return humiditeMesuree;
    }

    // Distance
    float getDistance() {
        return distanceMesuree;
    }

    // Luminosite
    float getLuminosite() {
        return luminositeMesuree;
    }

    // Validite des mesures
    bool temperatureValide() {
        return mesureTemperatureValide;
    }

    bool distanceValide() {
        return mesureDistanceValide;
    }

    bool luminositeValide() {
        return mesureLuminositeValide;
    }

    // Etat de la porte
    bool porteOuverte() {
        return porte.estOuverte();
    }

    // Etat de la LED
    bool getEtatLed() {
        return etatLed;
    }

    // Etat de l'alerte temperature
    bool getAlerteTemperature() {
        return alerteTemperature;
    }

};

#endif
