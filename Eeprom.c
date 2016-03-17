#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "functions.c"

int main( int argc, const char** argv ) {
	init_eeprom();
	unsigned char* tableau = malloc(TAILLE_EEPROM);

	printf("Lecture de la mémoire EEPROM :");
	read_eeprom(tableau, 256);
	displayBuffer(tableau, 256);

	printf("Ecriture dans la mémoire EEPROM :");
	int padding = 10;
	scanf("%s", &tableau[padding]);
	write_eeprom(tableau, strlen(&tableau[padding]), padding);
 	read_eeprom(tableau, 256);
	displayBuffer(tableau, 256);

	free(tableau);
    return 0;
}
