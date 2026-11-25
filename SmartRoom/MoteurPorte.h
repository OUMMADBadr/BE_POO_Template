
#ifndef MOTEUR_PORTE_H
#define MOTEUR_PORTE_H

#include "Actionneur.h"
#include <Servo.h>

class MoteurPorte : public Actionneur {

private:
    Servo servo;

    int broche;
    int angleFerme;
    int angleOuvert;

public:

    MoteurPorte(int pin)
        : Actionneur("Moteur porte"),
          broche(pin),
          angleFerme(0),
          angleOuvert(90) {}

    void initialiser() override {

        servo.attach(broche);

        fermer();

        Serial.println("Moteur porte initialise");
    }

    void ouvrir() {

        servo.write(angleOuvert);

        actif = true;

        Serial.println("Porte ouverte");
    }

    void fermer() {

        servo.write(angleFerme);

        actif = false;

        Serial.println("Porte fermee");
    }

    void activer() override {
        ouvrir();
    }

    void desactiver() override {
        fermer();
    }

    void reglerAngles(int ferme, int ouvert) {

        angleFerme = constrain(ferme, 0, 180);
        angleOuvert = constrain(ouvert, 0, 180);
    }
};

#endif
