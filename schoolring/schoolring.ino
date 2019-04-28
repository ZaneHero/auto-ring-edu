#include <Wire.h>
#include <iarduino_RTC.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define default_scheme 1
#define vsc_day_scheme 0
#define relay_pin 4
#define rotation A0
#define button_left 8
#define button_center 12
#define button_right 10
#define short_ring 1000

LiquidCrystal_I2C lcd(0x27, 16, 2);
iarduino_RTC time( RTC_DS3231);
byte schemes = 2; //Количество схем, кроме 0
byte serf = EEPROM.read(0);
byte a;
int temp = -10;
char* temptime = "jhgg";
int temp2 = -10;


void setup()
{
	a = serf;
	//EEPROM.write(0, 1); //Запиьс значение в энергонезависимую память для сохранения выбора схемы 
	//time.settime(0, 18, 10, 28, 4, 19, 7); //Установка времени (Секунды, минуты, часы, дата, месяц, год, день недели)
	time.period(0.1);
	Serial.begin(115200);
	time.begin();
	pinMode(rotation, INPUT);
	pinMode(relay_pin, OUTPUT);
	pinMode(button_left, INPUT_PULLUP);
	pinMode(button_center, INPUT_PULLUP);
	pinMode(button_right, INPUT_PULLUP);
	digitalWrite(relay_pin, HIGH);
	digitalWrite(button_left, HIGH);
	digitalWrite(button_center, HIGH);
	digitalWrite(button_right, HIGH);
	lcd.init();
	lcd.backlight();
}

int potentiometr() {
	long value = analogRead(rotation)*10;
	if (value > 999) {
		return value;
	}
	else {
		return 10;
	}
	
}

void ring(int leight) {
	digitalWrite(relay_pin, LOW);
	delay(leight);
	digitalWrite(relay_pin, HIGH);
}

void potentiometer_to_display() {
	if (potentiometr() < 10000) {
		if (potentiometr() != temp) {
			temp = potentiometr();
			lcd.setCursor(13, 1);
			lcd.print(" ");
			lcd.setCursor(14, 1);
			lcd.print(potentiometr() / 100);
		}
	}
	else {
		if (potentiometr() > 999) {
			if (potentiometr() != temp) {
				lcd.setCursor(13, 1);
				lcd.print(potentiometr() / 100);
			}
		}
		else {
			if (potentiometr() != temp) {

				lcd.setCursor(14, 1);
				lcd.print("10");
			}
		}


	}
}


void time_to_display() {
	char* a = time.gettime("H:i");
	if (a != temptime) {
		lcd.setCursor(0, 0);
		lcd.print(time.gettime("H:i"));
	}
	lcd.setCursor(6, 0);
	switch (time.weekday)
	{
	case 1:
	{
		lcd.print("Ponedelnik");
		break;
	}
	case 2:
	{
		lcd.print("Vtornik");
		break;
	}
	case 3:
	{
		lcd.print("Sreda");
		break;
	}
	case 4:
	{
		lcd.print("Chetverg");
		break;
	}
	case 5:
	{
		lcd.print("Pyatnitsa");
		break;
	}
	case 6: {
		lcd.print("Subbota");
		break;
	}
	case 0: {
		lcd.print("Voskresene");
		break;
	}
	}
}

byte button_check(byte a) {
	
	if (digitalRead(button_left)==0) {
		if (a == 0) {
			a = schemes;
			delay(100);
		}
		else {
		a = a - 1;
		delay(50);
		}
	}
	if (digitalRead(button_right) == 0) {
		if (a == schemes) {
			a = 0;
			delay(100);
		}
		else {
			a = a + 1;
			delay(100);
		}
	}

	
	if (digitalRead(button_center)==0) {
		EEPROM.write(0, a);
		delay(50);
	}
	scheme_to_display(a);
	return a;
}

void scheme_to_display(byte a) {
	lcd.setCursor(0, 1);
	if (a != temp2) {
	temp2 = a;
		switch (a)
		{
		case 1:
		{
			lcd.print("          ");
			lcd.setCursor(0, 1);
			lcd.print("Standart");
			break;
		}
		case 2:
		{
			lcd.print("          ");
			lcd.setCursor(0, 1);
			lcd.print("Custom");
			break;
		}

		case 0: {
			lcd.print("          ");
			lcd.setCursor(0, 1);
			lcd.print("Subbota");
			break;
		}
		}
	}
	if (EEPROM.read(0) != a) {
		lcd.setCursor(10, 1);
		lcd.print("#");
	}
	else {
		lcd.setCursor(10, 1);
		lcd.print(" ");
	}
}

void eeprom_update() {
	int rrt = EEPROM.read(0);
	if (time.Hours == 1) {
		if ((time.weekday != 0) && (temp2!= rrt)) {
			EEPROM.update(0, default_scheme);
			lcd.clear();
			lcd.setCursor(0, 1);
			lcd.print("Standart");
				if (potentiometr() < 10000) {
					
						temp = potentiometr();
							lcd.setCursor(13, 1);
							lcd.print(" ");
							lcd.setCursor(14, 1);
						lcd.print(potentiometr() / 100);
					
				}
				else {
						lcd.setCursor(13, 1);
						lcd.print(potentiometr() / 100);
				}
		}

		else{
			if (temp2 != rrt) {
				EEPROM.update(0, vsc_day_scheme);
					a = vsc_day_scheme;
					lcd.clear();
					lcd.setCursor(0, 1);
					lcd.print("Subbota");
					if (potentiometr() < 10000) {

						temp = potentiometr();
							lcd.setCursor(13, 1);
							lcd.print('\0');
							lcd.setCursor(14, 1);
						lcd.print(potentiometr() / 100);

					}
					else {
						lcd.setCursor(13, 1);
						lcd.print(potentiometr() / 100);
					}
			}
		}
	}
	
}

void scheme_check(int n) {
	switch (n)
	{
	case 1: {
	
		
	}
	case 2: {

	}
	case 0: {

	}

	}
}

void loop()
{
	
	//lcd.clear();
	potentiometer_to_display();
	time_to_display();
	a=button_check(a);
	eeprom_update();
	scheme_check(a);
	delay(30);
	
}
	
