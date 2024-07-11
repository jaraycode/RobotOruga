#include <SoftwareSerial.h>

// CREDENCIALES BLUETOOTH
// NOMBRE: ROBOT
// CODIGO PARIDAD: 2468
// MODO AT1 ESCLAVO

char dato = ' ';

const int Trigger = 7; //Pin digital 2 para el Trigger del sensor
const int Echo = 8; //Pin digital 3 para el Echo del sensor

int motor1pin1 = 2;
int motor1pin2 = 3;

int motor2pin1 = 4;
int motor2pin2 = 5;

int bluetoothRX = 6; // VER TEMA DE LOS PUERTOS A USAR
int bluetoothTX = 9;

//int gpsRX = 12; // VER TEMA DE LOS PUERTOS A USAR
//int gpsTX = 13;

int ENA = 10;
int ENB = 11;

SoftwareSerial BT(bluetoothRX, bluetoothTX);
//SoftwareSerial gps(gpsTX, gpsRX);

unsigned long previousMillis = 0; // guardará la última vez que se actualizó la distancia
const long interval = 5000;

void setup() {

  Serial.begin(115200);//iniciailzamos la comunicación
  BT.begin(9600); // iniciar comunicación bluetooth
  // gps.begin(9600); // iniciar comunicaión gps
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT); //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0

  // put your setup code here, to run once:
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  
}

void forward(char c) {
  BT.write("Avanzar"); // Convertir a minúscula y enviar de vuelta
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
}

void backward(char c) {
  BT.write("Retroceso"); // Convertir a minúscula y enviar de vuelta
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
}

void left(char c){
  BT.write("Izquierda"); // Convertir a minúscula y enviar de vuelta
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
}

void right(char c){
  BT.write("Derecha"); // Convertir a minúscula y enviar de vuelta  
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
}

void stopIt(char c){
  BT.write("Deteniendose..."); // Convertir a minúscula y enviar de vuelta
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
}


void action(char c){
  if (isUpperCase(c) && (c == 'S')) {
    stopIt(c);
  }
  if (isUpperCase(c) && (c == 'F')) {
    forward(c);
  }
  if (isUpperCase(c) && (c == 'B')) {
    backward(c);
  }
  if (isUpperCase(c) && (c == 'R')) {
    left(c);
  }
  if (isUpperCase(c) && (c == 'L')) {
    right(c);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(ENA, 200); //ENA pin
  analogWrite(ENB, 200); //ENB pin
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    long t; //timepo que demora en llegar el eco
    long d; //distancia en centimetros

    digitalWrite(Trigger, HIGH);
    delayMicroseconds(1); //Enviamos un pulso de 10us
    digitalWrite(Trigger, LOW);

    t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
    d = t/59; //escalamos el tiempo a una distancia en cm

    if (d <= 25) {
      action('S');
    }
  }
  if (BT.available()) { // Si hay datos disponibles para leer
  char c = BT.read(); // Leer el dato
  action(c);
  }
}
