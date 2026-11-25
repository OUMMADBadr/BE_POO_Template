
#ifndef CAPTEUR_H
#define CAPTEUR_H

#include <Arduino.h>

class Capteur {

protected:
    String nom;
    bool actif;

public:

    Capteur(const String& nomCapteur)
        : nom(nomCapteur), actif(true) {}

    virtual void initialiser() = 0;

    virtual bool lireValeur(float& valeur) = 0;

    void activer() {
        actif = true;
    }

    void desactiver() {
        actif = false;
    }

    bool estActif() const {
        return actif;
    }

    String getNom() const {
        return nom;
    }

    virtual ~Capteur() {}
};

#endif
