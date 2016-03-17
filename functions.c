// init all
void init_all();
// button
void init_button();
int read_button();
// led
void init_led_gpio_RGB();
void set_ledsGpio(char* red, char* green, char* blue);
void setLedMode(int mode);
// slider
void init_slider();
int read_slider();
// pwm
void init_pwm();
void set_pwm_value(int val);
void cmd_servo_hard(char angle);
// eeprom
void init_eeprom();
void read_eeprom(unsigned char* buffer, int size);
void write_eeprom(unsigned char* buffer, int size, int padding);
// others
char convertToAngle(int value);
void displayBuffer(unsigned char* buffer, int size);
// 7Seg

/*
	INIT ALL
*/

void init_all() {
	init_button();
    init_led_gpio_RGB();
	init_slider();
	init_pwm();
	init_eeprom();
}



/*
	BUTTON
*/

void init_button() {
	FILE* fp = NULL;

	// init gpio 22 in
	fp = fopen("/sys/class/gpio/export","w");
	fprintf(fp,"22");
	fclose(fp);
	fp = fopen("/sys/class/gpio/gpio22/direction","w");
	fprintf(fp,"in");
	fclose(fp);
}

int read_button()
{
	FILE *fp = NULL;
	int value;
	fp = fopen("/sys/class/gpio/gpio22/value","r");
	fscanf(fp,"%d",&value);
	fclose(fp);
	return value;
}



/*
	LED
*/

void init_led_gpio_RGB() {
	FILE* fp = NULL;

	// init gpio 7 out
	fp = fopen("/sys/class/gpio/export","w");
	fprintf(fp,"7");
	fclose(fp);
	fp = fopen("/sys/class/gpio/gpio7/direction","w");
	fprintf(fp,"out");
	fclose(fp);

	// init gpio 50 out
	fp = fopen("/sys/class/gpio/export","w");
	fprintf(fp,"50");
	fclose(fp);
	fp = fopen("/sys/class/gpio/gpio50/direction","w");
	fprintf(fp,"out");
	fclose(fp);

	// init gpio 51 out
	fp = fopen("/sys/class/gpio/export","w");
	fprintf(fp,"51");
	fclose(fp);
	fp = fopen("/sys/class/gpio/gpio51/direction","w");
	fprintf(fp,"out");
	fclose(fp);
}

void set_ledsGpio(char* red, char* green, char* blue) {
	FILE* fp = NULL;

	fp = fopen("/sys/class/gpio/gpio7/value","w");
	fprintf(fp, red);
	fclose(fp);

	fp = fopen("/sys/class/gpio/gpio50/value","w");
	fprintf(fp, green);
	fclose(fp);

	fp = fopen("/sys/class/gpio/gpio51/value","w");
	fprintf(fp, blue);
	fclose(fp);
}

void setLedMode(int mode) {
    printf("Mode : %d\n", mode);
	if(mode == 0) {
		set_ledsGpio("0", "1", "0");
	}
	else if (mode == 1) {
		set_ledsGpio("0", "0", "1");
	}
	else if (mode == 2) {
		set_ledsGpio("1", "0", "0");
	}
	else if (mode == 3) {
		set_ledsGpio("1", "1", "1");
	}
	else {
		set_ledsGpio("0", "0", "0");
	}
}



/*
	SLIDER
*/

#define PATH_TO_SLIDER "/sys/devices/ocp.3/helper.15/AIN5"

void init_slider() {
	// Activer les convertisseurs
	system("echo cape-bone-iio > /sys/devices/bone_capemgr.9/slots");
}

// Lecture de la valeur du slider
int read_slider() {
	FILE* fp = NULL;	// pointeur de fichier
	int value = 0;
	fp = fopen(PATH_TO_SLIDER,"r");
	fscanf(fp, "%d", &value);
	fclose (fp);
	return value * 1000000 / 1800.0; // Valeur entre 0 et 1 000 000
}



/*
	PWM
*/

void init_pwm() {
	system("echo am33xx_pwm > /sys/devices/bone_capemgr.9/slots");
	system("echo bone_pwm_P9_14 > /sys/devices/bone_capemgr.9/slots");
	// Laisse un petit temps au systeme pour creer les fichiers dessous
	usleep(1000000);
	system("echo 0 > /sys/devices/ocp.3/pwm_test_P9_14.16/polarity");
	system("echo 20000000 > /sys/devices/ocp.3/pwm_test_P9_14.16/period");
	system("echo 1500000 > /sys/devices/ocp.3/pwm_test_P9_14.16/duty");
}

void set_pwm_value(int val) {
	FILE* fp = NULL;	// pointeur de fichier
	fp = fopen("/sys/devices/ocp.3/pwm_test_P9_14.16/duty","w");
	fprintf(fp, "%d", val);
	fclose(fp);
}

void cmd_servo_hard(char angle) {
	if( angle < -45 | angle > 45 ) {
		printf("function cmd_servo_hard, error angle too high");
		return;
	}
	angle += 45;
	int value = 1000000 + angle*1000000/90;
	set_pwm_value(value);
}



/*
	EEPROM
*/

#define SIZE_EEPROM 32768
#define PATH_TO_EEPROM "/sys/bus/i2c/devices/1-0050/eeprom"

void init_eeprom() {
	// Pour indiquer au kernel que la présence d’un composant mémoire de type 24c256 à une adresse donnée :
	system("echo 24c256 0x50 > /sys/bus/i2c/devices/i2c-1/new_device");
}

void read_eeprom(unsigned char* buffer, int size) {
	FILE* fp = NULL;
	int i = 0;
	fp = fopen(PATH_TO_EEPROM,"r");
	for(i=0; i<size; i++) {
		fscanf(fp, "%c", &buffer[i]);
	}
	fclose (fp);
}

void write_eeprom(unsigned char* buffer, int size, int padding) {
	FILE* fp = NULL;
	int i = 0;
	fp = fopen(PATH_TO_EEPROM,"w");
	for(i = 0; i < (size+padding); i++) {
		fprintf(fp, "%c", buffer[i]);
	}
	fclose (fp);
}



/*
	OTHERS
*/

char convertToAngle(int value) {
	// on convertie la valeur entre 0 et 90 puis on retire 45 pour etre entre -45 et 45
	return (value / 1000000.0 * 90) -45 ;
}

void displayBuffer(unsigned char* buffer, int size) {
	printf("\n");
	int i = 0;
	for(i=0; i<size; i++) {
		printf("%c", buffer[i]);
	}
	printf("\n");
}



/*
	7SEGMENTS
*/

void init_7Seg() {
	FILE* fp = NULL;	// pointeur de fichier

	// init gpio 48 out
	// clear for 7Seg
	fp = fopen("/sys/class/gpio/export","w");
	fprintf(fp,"48");
	fclose(fp);
	fp = fopen("/sys/class/gpio/gpio48/direction","w");
	fprintf(fp,"out");
	fclose(fp);

	// latch 5
	// data 4
	// clock 2
}

void clear_7Seg()
{
	FILE *fp = NULL;
	fp = fopen("/sys/class/gpio/gpio48/value","w");
	fprintf(fp,"0");
	fclose(fp);
	fp = fopen("/sys/class/gpio/gpio48/value","w");
	fprintf(fp,"1");
	fclose(fp);
}

void clock_beat_7Seg() {
	FILE* fp = NULL;
	fp = fopen("/sys/class/gpio/gpio2/value","w");
	fprintf(fp,"1");
	fclose(fp);
	fp = fopen("/sys/class/gpio/gpio2/value","w");
	fprintf(fp,"0");
	fclose(fp);
}

void latch_beat_7Seg() {
	FILE* fp = NULL;
	fp = fopen("/sys/class/gpio/gpio5/value","w");
	fprintf(fp,"1");
	fclose(fp);
	fp = fopen("/sys/class/gpio/gpio5/value","w");
	fprintf(fp,"0");
	fclose(fp);
}

void display_on_7Seg(char c, char dot) {
	char nb0[8] = "00000011";
	char nb1[8] = "10011111";
	char nb2[8] = "00100101";
	char nb3[8] = "00001101";
	char nb4[8] = "10011001";
	char nb5[8] = "01001001";
	char nb6[8] = "01000001";
	char nb7[8] = "00011111";
	char nb8[8] = "00000001";
	char nb9[8] = "00001001";
	char* number = nb0;

	switch (c) {
		case '0':
			number = nb0;
			break;
		case '1':
			number = nb1;
			break;
		case '2':
			number = nb2;
			break;
		case '3':
			number = nb3;
			break;
		case '4':
			number = nb4;
			break;
		case '5':
			number = nb5;
			break;
		case '6':
			number = nb6;
			break;
		case '7':
			number = nb7;
			break;
		case '8':
			number = nb8;
			break;
		case '9':
			number = nb9;
	}

	// dot or not
	if(dot == 'y' || dot == '.') {
		number[7] = '0';
	}
	else {
		number[7] = '1';
	}

	FILE* fp = NULL;	// pointeur de fichier
	int i;
	for(i=7; i>=0; i--)
	{
		fp = fopen("/sys/class/gpio/gpio4/value","w");
		fprintf(fp,"%c",number[i]);
		fclose(fp);
		clock_beat_7Seg();
	}

	latch_beat_7Seg();
}
