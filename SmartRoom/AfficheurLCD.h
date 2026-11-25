
#ifndef AFFICHEUR_LCD_H
#define AFFICHEUR_LCD_H

#include <Arduino.h>
#include <Wire.h>
#include <rgb_lcd.h>

class AfficheurLCD {

private:
    rgb_lcd lcd;

public:

    void initialiser() {

        lcd.begin(16, 2);

        lcd.setRGB(0, 150, 255);

        lcd.clear();

        lcd.setCursor(0, 0);
        lcd.print("SMART ROOM");

        lcd.setCursor(0, 1);
        lcd.print("Demarrage...");
    }

    void afficherEnvironnement(
        float temperature,
        float humidite
    ) {

        lcd.clear();

        lcd.setRGB(0, 150, 255);

        lcd.setCursor(0, 0);
        lcd.print("Temp:");
        lcd.print(temperature, 1);
        lcd.print(" C");

        lcd.setCursor(0, 1);
        lcd.print("Hum:");
        lcd.print(humidite, 1);
        lcd.print(" %");
    }

    void afficherPorte(
        float distance,
        bool porteOuverte
    ) {

        lcd.clear();

        lcd.setRGB(0, 255, 0);

        lcd.setCursor(0, 0);
        lcd.print("Dist:");
        lcd.print(distance, 1);
        lcd.print(" cm");

        lcd.setCursor(0, 1);
        lcd.print("Porte:");

        if (porteOuverte) {
            lcd.print("OUVERTE");
        }
        else {
            lcd.print("FERMEE");
        }
    }

    void afficherAlerte(const char* message) {

        lcd.clear();

        lcd.setRGB(255, 0, 0);

        lcd.setCursor(0, 0);
        lcd.print("!!! ALERTE !!!");

        lcd.setCursor(0, 1);
        lcd.print(message);
    }
};

#endif
