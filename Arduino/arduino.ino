#include "HX711.h"
#include <Servo.h>
#include <Wire.h>

#define ESP32_ADDR 8

// Celda de carga 1
#define DOUT1  A1
#define CLK1  A0

// Celda de carga 2
#define DOUT2  A5
#define CLK2  A4

HX711 scale1;
HX711 scale2;

// Servomotor
Servo myServo;
int servoPin = 9;  // Pin donde está conectado el servo
int inicio = 60;  // Ángulo inicial
int final = 120;  // Ángulo final
int pos = inicio;  // Variable de posición del servo
int command = 0;  // Variable para almacenar el comando desde el monitor serial


void setup() {
  Wire.begin();

  Serial.begin(115200);
  
  scale1.begin(DOUT1, CLK1);
  scale1.set_scale(220);  // Ajustamos el factor de escala para la primera celda de carga
  scale1.tare();       // Tara la primera celda de carga
  
  delay(2000);  // Esperar 2 segundos para que se estabilice

  scale2.begin(DOUT2, CLK2);
  scale2.set_scale(220);  // Ajustamos el factor de escala para la segunda celda de carga
  scale2.tare();       // Tara la segunda celda de carga

  delay(2000);  // Esperar 2 segundos para que se estabilice
  
  Serial.println("Load cells are ready");

  // Configurar el servo
  myServo.attach(servoPin);
  myServo.write(pos);

  delay(1000);
}

void loop() {
  long reading1 = scale1.get_units(10);
  long reading2 = scale2.get_units(10);
  
  Serial.print("Load Cell 1 Weight: ");
  Serial.print(reading1*-1);
  Serial.print(" grams, ");
  
  Serial.print("Load Cell 2 Weight: ");
  Serial.println(reading2);

  // Concatenamos los valores de las celdas de carga en un solo mensaje
  String msg = String(reading1*-1) + "$" + String(reading2);

  // Enviamos el mensaje por I2C al ESP32
  Wire.beginTransmission(ESP32_ADDR);
  Wire.write(msg.c_str());
  Wire.endTransmission();

  // Se dispensa comida si el peso de la celda de carga 1 cumple con la condiciones
  if(reading1*-1 > 100 && reading1*-1 < 200){
    myServo.write(final);
    delay(1000);
    myServo.write(inicio);
    delay(1000);
  }
  
  delay(1000);
}