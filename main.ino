#include <time.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define DETECT 2 // pin 2 for  sensor

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const float radio = 0.65;             //radio de la rueda en metros
const float Pi = 3.1416;
const float pRueda = radio * radio * Pi; //perimetro de la rueda
int mod1;
double distancia, velocidad, velocidadAnterior = 0;
long int vueltas = 0;
unsigned long time1, time2, dif, now, later, reset = 0;



void setup() {
  pinMode(DETECT, INPUT);//define detect input pin
  lcd.begin(16, 2);
  lcd.print("BICIBOT");
  delay(5000);
}

void loop() {
  int detected = digitalRead(DETECT); // read Hall sensor

  if (detected == HIGH) {
    //Las proximas lineas controlan la incertidumbre del sensor hall.
    delay(200);
    while (detected == HIGH) {
      detected = digitalRead(DETECT);
    }

    //calculo de la distancia
    vueltas += 1;
    distancia = pRueda * vueltas;

    //calculo de velocidad.
    //se utiliza la diferencia entre 2 tiempos para ver cuanto demmora en hacer una vuelta.

    if (vueltas % 2 == 1) {
      time1 = millis();
      if (time2 != 0) {
        dif = time1 - time2;
      }
    }
    else if (vueltas % 2 == 0) {
      time2 = millis();
      dif = time2 - time1;
    }
    //Se calcula la velocidad.
    velocidad = (float)dif;
    velocidad = velocidad / 1000;     //se guardan los segundos en la variable
    velocidad = pRueda / velocidad;   //distancia que se recorrio en el tiempo guardado en la variable velocidad es el perimetro de la rueda.
  }


  //En la proxima seccion de codigo se maneja el display intercalando la velocidad y la distancia cada 5 seg.
  now = millis();
  mod1 = (now / (1000 * 10)) % 2;

  if (now - 2500 > reset) {
    reset = now;
    if (velocidad == velocidadAnterior)
      velocidad = 0;
    else
      velocidadAnterior = velocidad;
  }

  if (now - 300 > later) {
    later = now;
    if (mod1 == 0) {
      lcd.clear();
      lcd.print("Distancia:");
      lcd.setCursor(0, 1);
      if (distancia < 1000) {
        lcd.print(distancia);
        lcd.print(" mts");
      }
      else {
        lcd.print(distancia / 1000);
        lcd.print(" Km");
      }
    }
    else {
      lcd.clear();
      lcd.print("Velocidad:");
      lcd.setCursor(0, 1);
      lcd.print(velocidad * 3.6); //se multiplica por 3.6 para pasarlo de m/s a Km/h
      lcd.print("Km/h");
    }
  }
}


