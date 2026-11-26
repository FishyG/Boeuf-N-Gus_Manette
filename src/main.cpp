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
#include "Rfid.h"

const int LED_ALIM = 22;
const int LED_BURGER[6] = { 24, 25, 26, 27, 28, 29};

const int BOUTON_SEND = 30;

// Définir les tags (même ordre que LED_BURGER) --------------------------
const char *TAGS[6] = {
    "1600C3C98A",   // Bob
    "1600BE42E8",   // Patrick
    "1600C255DD",   // Carlos
    "0E008E9C42",   // Crab
    "1600CA59E8",   // Sandy
    "08003C1409",   // Plankton
};

const char *NOMS[6] = { "Bob", "Patrick", "Carlos", "Crab", "Sandy", "Plankton" };

char tag[20];
int burger_choisi = -1;   // -1 = aucun tag reconnu

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
    static bool dernier_etat = HIGH;
    bool lecture = digitalRead(BOUTON_SEND);
    bool out = lecture == LOW && dernier_etat == HIGH;

    dernier_etat = lecture;

    return out;
}

// Retourne l'index du tag (0 à 5), ou -1 si inconnu
int trouver_tag(const char *id)
{
    for (int i = 0; i < 6; ++i)
    {
        if (strncmp(id, TAGS[i], 10) == 0) return i;
    }
    return -1;
}

// Allume seulement la LED de l'index donné (logique inversée : LOW = allumée)
void allumer_led_burger(int index)
{
    for (int i = 0; i < 6; ++i)
    {
        digitalWrite(LED_BURGER[i], (i == index) ? LOW : HIGH);
    }
}


void setup() {
    BoardInit();
    setupRfid();

    Serial.println("======BOOT=====");

    // init des DEL_BURGER_X
    for (int i = 0; i < 6; ++i)
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
    if (lireRfid(tag))
    {
        Serial.print("RFID lu: ");
        Serial.println(tag);

        int index = trouver_tag(tag);
        if (index >= 0)
        {
            burger_choisi = index;
            allumer_led_burger(index);
        }
        else
        {
            Serial.println("Tag inconnu");
        }
    }

    if (bouton_appuye())
    {
        if (burger_choisi >= 0)
        {
            Serial.print("Burger envoyé : ");
            Serial.println(NOMS[burger_choisi]);

            envoyer_commande(burger_choisi + 1);   // commande 1 à 6
            burger_choisi = -1;                    // enlève la sélection
            allumer_led_burger(-1);                // éteint toutes les LEDs
        }
        else
        {
            Serial.println("Aucun burger choisi");
        }
    }
}