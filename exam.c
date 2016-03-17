#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "functions.c"

int main( int argc, const char** argv ) {

	// Initialisation (prend au moins une seconde à cause du pwm)
	init_all();

	// Variables;
	int isButtonFree = 1;
	int buttonPreviouslyPushed = 0;
	int mode = 0;
	int sliderValue = 0;
	int posInTab = 0;
	int posWhenReadingTab = 0;
	char* tab = malloc(SIZE_EEPROM);
	char* readingTab = malloc(SIZE_EEPROM);
	int isReadingTabInitalized = 0;

	// Affiche le mode 1 (mode = 0)
	setLedMode(mode);

    while (1) {
		// On lit la valeur du bouton
		isButtonFree = read_button();

		// On regarde si le bouton est appuyé, s'il est appuyé on met buttonPreviouslyPushed à 1
		if(!isButtonFree) {
			buttonPreviouslyPushed = 1;
		}

		// Si on a relaché le bouton (buttonPreviouslyPushed mais le bouton n'est plus activé maintenant)
		if(buttonPreviouslyPushed && isButtonFree){
			// On remet le boolean à 0
			buttonPreviouslyPushed = 0;
			// Si on etait en mode numero 3 (mode = 2), on enregistre dans l'eeprom le tableau
			if(mode == 2) {
				write_eeprom(tab, posInTab, 0);
			}
			// On change le mode
			mode++;
			mode %= 4;
			setLedMode(mode);
		}

		// Si on est sur le mode 1
		if(mode == 0) {
			sliderValue = read_slider();
			set_pwm_value(1000000+sliderValue);
		}

		// Si on est sur le mode 3
		if(mode == 2) {
			sliderValue = read_slider();
			set_pwm_value(1000000+sliderValue);
			// notre sliderValue est entre 0 et 1 000 000, on la convertie en angle
			tab[posInTab] = convertToAngle(sliderValue);
			posInTab++;
			tab[posInTab] = 127;
			// Si on arrive au maximum on change de mode
			if(posInTab >= SIZE_EEPROM - 1 ) {
				buttonPreviouslyPushed = 1;
			}
			// Attente de 50ms pour le mode 3 uniquement
	        usleep(50000);
		}

		// Si on est sur le mode 4
		if(mode == 3) {
			// On lit l'eeprom une seule fois
			if(!isReadingTabInitalized) {
				printf("Lecture de l'eeprom ... ");
	 			read_eeprom(readingTab, posInTab);
				printf("end\n");
				isReadingTabInitalized = 1;
			}
			// On dit au servo de se déplacer à l'angle
			cmd_servo_hard(readingTab[posWhenReadingTab]);

			// On parcourt le tableau, si on atteint le max ou que on trouve 127 on repart à 0
			posWhenReadingTab++;
			if(posWhenReadingTab >= posInTab || readingTab[posWhenReadingTab] == 127) {
				posWhenReadingTab = 0;
				printf("On recommence !\n");
				// Le message de ce printf s'accumule et tous les messages ne s'affichent que au changement de mode
			}
			// Attente de 50ms pour le mode 4 uniquement
	        usleep(50000);
		}

		// Temps de pause dans la boucle infinie pour alleger le processeur
		// Evite egalement les rebonds du bouton
        usleep(1000);
    }
}
