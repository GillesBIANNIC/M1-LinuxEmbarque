#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "functions.c"

int main( int argc, const char** argv ) {
	init_slider();
	init_pwm();

	int value = 0;		// entier pour stocker les lecture des valeurs
	while(1) {
		value = read_slider();
		set_pwm_value(1000000+value);

		// Petit temps de pause car boucle infinie
		usleep(1000);
	}
    return 0;
}
