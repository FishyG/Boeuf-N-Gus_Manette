
#include "Rfid.h"


void setupRfid() {
  Serial3.begin(9600);
}

bool lireRfid(char id_tag[20]) {
  char crecu;
  static byte i = 0;
  static bool incoming = false;

  while (Serial3.available())
  {
    crecu=Serial3.read();     // lit le ID-12
    switch(crecu)
    {
      case 0x02:
        // START OF TRANSMIT
        i=0;
        incoming=1;
        break;
      case 0x03:
        // END OF TRANSMIT
        incoming=0;
        // Affiche le code recu sans valider le checksum
        for( i=0 ; i<10 ; i++ )
        {
          Serial.print(id_tag[i]);
        }
        Serial.println("");
        id_tag[i] = '\0';
        return 1;
      default:
        Serial.print(crecu);
        if(incoming){
          id_tag[i++] = crecu;
        }
        break;
    }
  }
  return 0;
}

int verifierTag(char id_tag[20], char tag_attendu[20]) {
  for (int i = 0; i < 10; i++) {
    if (id_tag[i] != tag_attendu[i]) {
      return 0; // Les tags ne correspondent pas
    }
  }
  return 1; // Les tags correspondent
}

