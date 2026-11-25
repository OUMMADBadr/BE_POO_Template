
#ifndef LED_H
#define LED_H

#include "Actionneur.h"

class LED : public Actionneur {

private:
    int broche;

public:

    LED(int pin)
        : Actionneur("LED"), broche(pin) {}

    void initialiser() override {

        pinMode(broche, OUTPUT);

        desactiver();

        Serial.println("LED initialisee");
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
