#ifndef CAPTEUR_SHT31_H
#define CAPTEUR_SHT31_H

#include "Capteur.h"
#include <Adafruit_SHT31.h>
#include <math.h>

class CapteurSHT31 : public Capteur {

private:
    Adafruit_SHT31 capteur;
    float humidite;
    bool disponible;

public:
    CapteurSHT31()
        : Capteur("SHT31"), humidite(0), disponible(false) {}

    void initialiser() override {
        disponible = capteur.begin(0x44);
        actif = disponible;

        if (disponible) {
            Serial.println("Capteur SHT31 initialise");
        } else {
            Serial.println("Erreur : capteur SHT31 introuvable");
        }
    }

    bool lireValeur(float& temperature) override {
        if (!actif || !disponible) {
            return false;
        }

        const float nouvelleTemperature = capteur.readTemperature();
        const float nouvelleHumidite = capteur.readHumidity();
        if (isnan(nouvelleTemperature) || isnan(nouvelleHumidite)) {
            return false;
        }

        temperature = nouvelleTemperature;
        humidite = nouvelleHumidite;
        return true;
    }

    float getHumidite() const {
        return humidite;
    }
};

#endif
