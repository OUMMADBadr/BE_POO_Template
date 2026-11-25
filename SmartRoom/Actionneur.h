
#ifndef ACTIONNEUR_H
#define ACTIONNEUR_H

#include <Arduino.h>

class Actionneur {

protected:
    String nom;
    bool actif;

public:

    Actionneur(const String& nomActionneur)
        : nom(nomActionneur), actif(false) {}

    virtual void initialiser() = 0;

    virtual void activer() = 0;

    virtual void desactiver() = 0;

    bool estActif() const {
        return actif;
    }

    String getNom() const {
        return nom;
    }

    virtual ~Actionneur() {}
};

#endif
