
#ifndef BUZZER_H
#define BUZZER_H

#include "Actionneur.h"

class Buzzer : public Actionneur {

private:
    int broche;

public:

    Buzzer(int pin)
        : Actionneur("Buzzer"), broche(pin) {}

    void initialiser() override {

        pinMode(broche, OUTPUT);

        desactiver();

        Serial.println("Buzzer initialise");
    }

    void activer() override {

        digitalWrite(broche, HIGH);

        actif = true;
    }

    void desactiver() override {

        digitalWrite(broche, LOW);

        actif = false;
    }
};

#endif
