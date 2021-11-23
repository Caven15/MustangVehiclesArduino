/******************************************************************************************************************************/
/******************************************************************************************************************************/

/**************************************/
/***** Release By Alexandre Claes *****/
/**************************************/

// programme de voiture RC lié a  L'application (Bluethoot Electronic) => android playstore.
// fait durant la formation (Module complémentaire A.M.N) au centre du cesep de nivelles.
// utilisation du composant => hc 06 (module bluethoot)


// Mapping de l'interface graphique de l'application (Bluethoot Electronic)

/*
  Démarrage véhicule        => P
  Arret véhicule            => q
  Start Clignotant gauche   => C
  Stop Clignotant gauche    => c
  Start Clognotant droit    => D
  Stop Clognotant droit     => d
  start 4 Clignoant         => H
  stop 4 Clignoant          => h
  start Petit phare         => F
  Stop  Petit phare         => f
  start Grand phare         => B
  Stop  Grand phare         => b
  Direction avant           => X
  Changement sens direction => M
  vitesse véhicule (0)      => I
  vitesse véhicule (1)      => J
  vitesse véhicule (2)      => K
  vitesse véhicule (3)      => L
*/
/******************************************************************************************************************************/
/******************************************************************************************************************************/

#include "pitches.h"
#include <Servo.h>

#define Moteur_In_1 6                                                   // sortie moteur dc 1
#define Moteur_In_2 7                                                   // sortie moteur dc 2
#define Vitesse_Moteur 8                                                // definition de la sortie pour la vitesse moteur 

Servo Direction;


const int GRAND_PHARE_AVANT_GAUCHE = 22;
const int GRAND_PHARE_AVANT_DROIT = 24;

const int PETIT_PHARE_AVANT_GAUCHE = 26;
const int PETIT_PHARE_AVANT_DROIT = 28;
const int PETIT_PHARE_ARRIERE_GAUCHE = 30;
const int PETIT_PHARE_ARRIERE_DROIT = 32;

const int CLIGNOTANT_AVANT_GAUCHE = 34;
const int CLIGNOTANT_AVANT_DROIT = 36;
const int CLIGNOTANT_ARRIERE_GAUCHE = 38;
const int CLIGNOTANT_ARRIERE_DROIT = 40;

long Temps;                                                             // variables qui stock la mesure du temps
boolean Etat_Led;                                                       // tocke l'état de valeur led dans un true/false
char Data_In;                                                           // donnée entré par l'utilisateur

unsigned long Last_Time = 0;                                            // temps depuis la dernière mis a jour
int Update_interval = 100;                                              // temps d'inhterval en milliseconde pour mettre a jour les valeurs
int Direction_X;                                                        // donnée recue pour la direction
int Direction_Y;                                                        // donnée recue pour l'accélération
int Rotation_Direction = 0;
int Avant_arriere = false;

int Verouillage[] = {NOTE_B5, NOTE_AS5, NOTE_A5};                       // note de mélodie de verouillage
int Deverouillage[] = {NOTE_A5, NOTE_AS5, NOTE_B5};                     // note de mélodie de déverouillage
int PerteSignal[] = {NOTE_D8};                                          // note de mélodie si le module bluethoot perd le signal
int noteDurations[] = {10};                                             // variable qui donne la durée des notes

/**************************************************************************************************************/

void setup() {                                                          // Chargement et définition des sortie/entrée

  Serial.begin(9600);
  Temps = millis();                                                     // initialisation du temps
  Direction.attach(A7);                                                 // affectation direction a la pin
  Direction.write(90);                                                  // positionnement au centre par default -> 90°

  pinMode(GRAND_PHARE_AVANT_GAUCHE, OUTPUT);                            // définition des sortie de la carte arduino
  pinMode(GRAND_PHARE_AVANT_DROIT, OUTPUT);

  pinMode(PETIT_PHARE_AVANT_GAUCHE, OUTPUT);
  pinMode(PETIT_PHARE_AVANT_DROIT, OUTPUT);
  pinMode(PETIT_PHARE_ARRIERE_GAUCHE, OUTPUT);
  pinMode(PETIT_PHARE_ARRIERE_DROIT, OUTPUT);

  pinMode(CLIGNOTANT_AVANT_GAUCHE, OUTPUT);
  pinMode(CLIGNOTANT_ARRIERE_GAUCHE, OUTPUT);

  pinMode(CLIGNOTANT_AVANT_DROIT, OUTPUT);
  pinMode(CLIGNOTANT_ARRIERE_DROIT, OUTPUT);

  pinMode(Vitesse_Moteur, OUTPUT);
  pinMode(Moteur_In_1, OUTPUT);
  pinMode(Moteur_In_2, OUTPUT);

  digitalWrite(Moteur_In_1, HIGH);                                    // définition du sens de rotation initiale des moteurs
  digitalWrite(Moteur_In_2, LOW);

  digitalWrite(GRAND_PHARE_AVANT_GAUCHE, 0);                          // définition des pin a low
  digitalWrite(GRAND_PHARE_AVANT_DROIT, 0);

  digitalWrite(PETIT_PHARE_AVANT_GAUCHE, 0);
  digitalWrite(PETIT_PHARE_AVANT_DROIT, 0);
  digitalWrite(PETIT_PHARE_ARRIERE_GAUCHE, 0);
  digitalWrite(PETIT_PHARE_ARRIERE_DROIT, 0);

  digitalWrite(CLIGNOTANT_AVANT_GAUCHE, 0);
  digitalWrite(CLIGNOTANT_ARRIERE_GAUCHE, 0);

  digitalWrite(CLIGNOTANT_AVANT_DROIT, 0);
  digitalWrite(CLIGNOTANT_ARRIERE_DROIT, 0);

}

/**************************************************************************************************************/

void loop() {                                                         // Boucle principale

  Data_In = Serial.read();                                      // affectation de la valeur entrée au gsm a la variable ActionUser

  /**************************************************************************************************************/

  /****************************************************************************/
                                  /*- direction - */
  /****************************************************************************/
  
  if (Data_In == 'X') {                                         // si l'utilisateur presse la touche
    Direction_X = Serial . parseInt ( ) ;                       // affectation valeur joistick a la variables direction_x
    Direction_X = map(Direction_X, 0, 1023, 0, 180);            // mapping des la valeur du pas pour passer de 1023 a 180 pour l'axe du servomoteur
    Direction.write(Direction_X);                               // déplace le servo moteur a la valeur indiquer par l'utilisateur

    Serial.println("axe direction =>");
    Serial.println(Direction_X);                                // retour de la position du servomoteur dans le sérial
  }


  /****************************************************************************/
                                /*- accélération - */
  /****************************************************************************/
  if (Data_In == 'I') {                                         // si l'utilisateur presse la touche

    //tmrpcm.play("Start.wav");
    Serial.println("Vitesse 0");
    analogWrite(Vitesse_Moteur, 0);                             // passage de la rotation du moteur a ...
  }
  if (Data_In == 'J') {
    Serial.println("Vitesse 1 (85)");
    analogWrite(Vitesse_Moteur, 85);
  }
  if (Data_In == 'K') {
    Serial.println("Vitesse 2 (170)");
    analogWrite(Vitesse_Moteur, 170);
  }
  if (Data_In == 'L') {
    Serial.println("Vitesse 3 (255)");
    analogWrite(Vitesse_Moteur, 255);
  }

  /****************************************************************************/
                       /*- changement de sens accélération- */
  /****************************************************************************/
  if (Data_In == 'M') {
    Serial.println("rotation moteur avant");
    digitalWrite(Moteur_In_1, HIGH);
    digitalWrite(Moteur_In_2, LOW);
  }
  else if (Data_In == 'm') {
    Serial.println("rotation moteur arrière");
    digitalWrite(Moteur_In_1, LOW);
    digitalWrite(Moteur_In_2, HIGH);
  }

  /****************************************************************************/
                  /*- Condition qui allume ou eteint les phare - */
  /****************************************************************************/

  if (Data_In == 'F') {                                         // si l'utilisateur presse la touche...
    Serial.println("petit phare allumé");
    digitalWrite(PETIT_PHARE_AVANT_GAUCHE, 1);                  // passage a l'état high de la sortie 6 (phare)
    digitalWrite(PETIT_PHARE_AVANT_DROIT, 1);
    digitalWrite(PETIT_PHARE_ARRIERE_GAUCHE, 1);
    digitalWrite(PETIT_PHARE_ARRIERE_DROIT, 1);
  }

  else if (Data_In == 'f') {                                    // sinon si l'utilisateur presse la touche...
    Serial.println("petit phare éteint");
    digitalWrite(PETIT_PHARE_AVANT_GAUCHE, 0);                  // passage a l'état low de la sortie 6
    digitalWrite(PETIT_PHARE_AVANT_DROIT, 0);
    digitalWrite(PETIT_PHARE_ARRIERE_GAUCHE, 0);
    digitalWrite(PETIT_PHARE_ARRIERE_DROIT, 0);
  }
  /****************************************************************************/
              /*- Condition qui allume ou eteint les grand phare avant- */
  /****************************************************************************/
  if (Data_In == 'B') {
    Serial.println("grand phare avant allumé");
    digitalWrite(GRAND_PHARE_AVANT_GAUCHE, 1);
    digitalWrite(GRAND_PHARE_AVANT_DROIT, 1);

  }

  else if (Data_In == 'b') {
    Serial.println("grand phare avant éteint");
    digitalWrite(GRAND_PHARE_AVANT_GAUCHE, 0);
    digitalWrite(GRAND_PHARE_AVANT_DROIT, 0);
  }

  /****************************************************************************/
              /*- Condition qui allume ou eteint les clignotant gauche - */
  /****************************************************************************/

  if (Data_In == 'C') {

    if ((millis() - Temps) > 600)                                // fonction millis permet de ne pas mettre le programme en pause pendant le changement d'état de la led
    {
      Etat_Led = !Etat_Led;                                      // on inverse l'état de la LED
      Serial.println("clignotant gauche");
      digitalWrite(CLIGNOTANT_AVANT_GAUCHE, Etat_Led);           // on allume ou éteint
      digitalWrite(CLIGNOTANT_ARRIERE_GAUCHE, Etat_Led);
      Temps = millis();                                          // on stocke la nouvelle heure
    }

  }
  /****************************************************************************/
              /*- Condition qui allume ou eteint les clignotant droit - */
  /****************************************************************************/
  if (Data_In == 'D') {
    if ((millis() - Temps) > 600)
    {
      Etat_Led = !Etat_Led;
      Serial.println("clignotant droit");
      digitalWrite(CLIGNOTANT_AVANT_DROIT, Etat_Led);
      digitalWrite(CLIGNOTANT_ARRIERE_DROIT, Etat_Led);
      Temps = millis();
    }
  }
  /****************************************************************************/
                /*- Condition qui allume ou eteint les 4 clignotant  - */
  /****************************************************************************/
  if (Data_In == 'H') {
    if ((millis() - Temps) > 600)
    {
      Etat_Led = !Etat_Led;
      Serial.println("4 clignotant");
      digitalWrite(CLIGNOTANT_AVANT_DROIT, Etat_Led);
      digitalWrite(CLIGNOTANT_ARRIERE_DROIT, Etat_Led);
      digitalWrite(CLIGNOTANT_AVANT_GAUCHE, Etat_Led);
      digitalWrite(CLIGNOTANT_ARRIERE_GAUCHE, Etat_Led);
      Temps = millis();
    }
  }
}
