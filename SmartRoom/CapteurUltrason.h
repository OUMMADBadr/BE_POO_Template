
#ifndef CAPTEUR_ULTRASON_H
#define CAPTEUR_ULTRASON_H

#include "Capteur.h"

class CapteurUltrason : public Capteur {

private:
    int broche;

public:

    CapteurUltrason(int pin)
        : Capteur("Ultrason"), broche(pin) {}

    void initialiser() override {

        pinMode(broche, OUTPUT);
        digitalWrite(broche, LOW);

        Serial.println("Ultrason initialise");
    }

    bool lireValeur(float& distance) override {

        if (!actif) {
            return false;
        }

        // Declenchement de la mesure
        pinMode(broche, OUTPUT);

        digitalWrite(broche, LOW);
        delayMicroseconds(2);

        digitalWrite(broche, HIGH);
        delayMicroseconds(12);

        digitalWrite(broche, LOW);

        // Reception de l'echo
        pinMode(broche, INPUT);

        unsigned long duree =
            pulseIn(broche, HIGH, 30000);

        if (duree == 0) {
            return false;
        }

        // Conversion en centimetres
        distance = duree * 0.0343 / 2.0;

        return true;
    }
};

#endif
