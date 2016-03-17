#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "functions.c"

int main( int argc, const char** argv ) {
	init_7Seg();
    clear_7Seg();
    usleep(1000000);
    display_on_7Seg('0', 'y');
    usleep(1000000);
    display_on_7Seg('1', 'n');
    usleep(1000000);
    display_on_7Seg('2', 'y');
    usleep(1000000);
    display_on_7Seg('3', 'n');
    usleep(1000000);
    display_on_7Seg('4', '.');
    usleep(1000000);
    display_on_7Seg('5', 'n');
    usleep(1000000);
    display_on_7Seg('6', 'y');
    usleep(1000000);
    display_on_7Seg('7', 'n');
    usleep(1000000);
    display_on_7Seg('8', 'y');
    usleep(1000000);
    display_on_7Seg('9', 'n');
    usleep(1000000);
    return 0;
}
