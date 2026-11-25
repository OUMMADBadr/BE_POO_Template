
#ifndef CAPTEUR_LUMINOSITE_H
#define CAPTEUR_LUMINOSITE_H

#include "Capteur.h"

class CapteurLuminosite : public Capteur {

private:
    int broche;

public:

    CapteurLuminosite(int pin)
        : Capteur("Luminosite"), broche(pin) {}

    void initialiser() override {

        pinMode(broche, INPUT);

        Serial.println("Capteur luminosite initialise");
    }

    bool lireValeur(float& valeur) override {

        if (!actif) {
            return false;
        }

        valeur = analogRead(broche);

        return true;
    }

    bool estSombre(float seuil = 500) {

        float luminosite;

        if (!lireValeur(luminosite)) {
            return false;
        }

        return luminosite < seuil;
    }
};

#endif
