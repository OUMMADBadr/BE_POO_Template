
#ifndef PORTE_AUTOMATIQUE_H
#define PORTE_AUTOMATIQUE_H

#include <Arduino.h>
#include "MoteurPorte.h"

enum EtatPorte {
    FERMEE,
    OUVERTE,
    ATTENTE_FERMETURE
};

class PorteAutomatique {

private:

    MoteurPorte& moteur;

    EtatPorte etat;

    float seuilDistance;

    unsigned long dernierPassage;

    const unsigned long delaiFermeture = 3000;

public:

    PorteAutomatique(MoteurPorte& m)
        : moteur(m),
          etat(FERMEE),
          seuilDistance(30),
          dernierPassage(0) {}

    void initialiser() {

        moteur.fermer();

        etat = FERMEE;
    }

    void mettreAJour(float distance) {

        if (distance > 2 && distance < seuilDistance) {

            dernierPassage = millis();

            if (etat == FERMEE) {

                moteur.ouvrir();
            }

            etat = OUVERTE;
        }

        else {

            if (etat == OUVERTE) {

                etat = ATTENTE_FERMETURE;
            }

            if (etat == ATTENTE_FERMETURE) {

                if (millis() - dernierPassage >=
                    delaiFermeture) {

                    moteur.fermer();

                    etat = FERMEE;
                }
            }
        }
    }

    bool estOuverte() const {

        return etat != FERMEE;
    }

    void reglerSeuil(float seuil) {

        seuilDistance = seuil;
    }
};

#endif

