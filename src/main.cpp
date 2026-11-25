/*
Projet: main.cpp robo swag
Equipe: P2
Auteurs: Jessy, RAJOUTER VOS CNOMS ICI
Description: Code epreuve du combattant
Date: Aujourd'hui lmao
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <LibRobus.h>  // Pour utiliser RobUS
#include "../lib/Rfid.h"

const int LED_ALIM = 22;
const int LED_BURGER[6] = { 24, 25, 26, 27, 28, 29};

const int BOUTON_SEND = 30;

IRsend irsend;

uint32_t messageNEC(uint8_t adresse, uint8_t commande)
{
  return ((uint32_t)adresse << 24)
       | ((uint32_t)(uint8_t)~adresse << 16)
       | ((uint32_t)commande << 8)
       | (uint8_t)~commande;
}

void envoyer_commande(int commande)
{
    uint32_t message = messageNEC(0x17, commande);
    irsend.sendNEC(message, 32);
    Serial.print("Envoyé : 0x");
    Serial.println(message, HEX);
    delay(500);
}

bool bouton_appuye()
{
    return false;
}


void setup() {
    BoardInit();
    setupRfid();

    // init des DEL_BURGER_X
    for (int i = 0; i < 6; ++i)<
    {
        digitalWrite(LED_BURGER[i], HIGH);
        pinMode(LED_BURGER[i], OUTPUT);
    }

    // LED Alimentation
    pinMode(LED_ALIM, OUTPUT);
    digitalWrite(LED_ALIM, LOW);

    pinMode(BOUTON_SEND, INPUT);
}

void loop() {
    if (lireRfid())
    {

    }

    if (bouton_appuye())
    {
        envoyer_commande(1);
    }
}
