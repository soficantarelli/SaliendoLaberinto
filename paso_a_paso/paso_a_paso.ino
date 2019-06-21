#include <AFMotor.h>
#define STOPPED 0
#define FOLLOWING_LINE 1
#define NO_LINE 2
#define CONT_LINE 3
#define POS_LINE 4
#define RIGHT_TURN 5
#define LEFT_TURN 6

#define IZQUIERDA -1
#define DERECHA 1

AF_DCMotor motor1(1, MOTOR12_64KHZ); 
AF_DCMotor motor2(2, MOTOR12_64KHZ); 

const int SensorIzquierdo = A0; //izquierda
const int Sensor1 = A1;
const int Sensor2 = A2;
const int Sensor3 = A3;
const int SensorDerecho = A4; //derecha

int Sensores[5]= {0, 0, 0, 0, 0};
int valorizquierdo = 0, valor1 = 0, valor2 = 0, valor3 = 0, valorderecho = 0;
const int boton = A5;
int mode = 0;
int error = 0;
int Kp = 20;
unsigned int estado = 0; 
int control = 0;

char camino[100] = " ";
unsigned char LongitudCamino = 0;
int indiceCamino = 0;

void setup() {
  Serial.begin(9600);
  pinMode(boton, INPUT);
  motor1.setSpeed(160); 
  motor2.setSpeed(120); 
}

void leerSensores() {
  valorizquierdo = analogRead(SensorIzquierdo);
  valor1 = analogRead(Sensor1);
  valor2 = analogRead(Sensor2);
  valor3 = analogRead(Sensor3);
  valorderecho = analogRead(SensorDerecho);
  
  if (valorizquierdo < 750)
    Sensores[0] = 0;
  else
    Sensores[0] = 1;
  if (valor1 < 880)
    Sensores[1] = 0;
  else
    Sensores[1] = 1;    
  if (valor2 < 6)
    Sensores[2] = 0;
  else
    Sensores[2] = 1;
  if (valor3 < 880)
    Sensores[3] = 0;
  else
    Sensores[3] = 1;
  if (valorderecho < 750)
    Sensores[4] = 0;
  else
    Sensores[4] = 1;
    
  Serial.print("Sensor Izquierdo ");
  Serial.println(Sensores[0]);
  Serial.println(valorizquierdo);
  Serial.print("Sensor 1 ");
  Serial.println(Sensores[1]);
  Serial.println(valor1);
  Serial.print("Sensor 2 ");
  Serial.println(Sensores[2]);
  Serial.println(valor2);
  Serial.print("Sensor 3 ");
  Serial. println(Sensores[3]);
  Serial.println(valor3);
  Serial.print("Sensor Derecho ");
  Serial.println(Sensores[4]);
  Serial.println(valorderecho);
  
  if((Sensores[0] == 1)&&(Sensores[1] == 1)&&(Sensores[2] == 1)&&(Sensores[3] == 1)&&(Sensores[4] == 1)) {
    mode = CONT_LINE; error = 0;
  }
  else {
    if((Sensores[0] == 0)&&(Sensores[4] == 1)) {
      mode = RIGHT_TURN; error = 0;
    }
    else {
      if((Sensores[0] == 1)&&(Sensores[4] == 0)) {
        mode = LEFT_TURN; error = 0;
      }
      else {
        if ((Sensores[0] == 0)&&(Sensores[1] == 0)&&(Sensores[2] == 0)&&(Sensores[3] == 0)&&(Sensores[4] == 0)) {
          mode = NO_LINE; error = 0;
        }
        else {
          if ((Sensores[0] == 0)&&(Sensores[1] == 0)&&(Sensores[2] == 0)&&(Sensores[3] == 1)&&(Sensores[4] == 1)) {
            mode = FOLLOWING_LINE; error = 3;
          }
          else {
            if ((Sensores[0] == 0)&&(Sensores[1] == 0)&&(Sensores[2] == 0)&&(Sensores[3] == 1)&&(Sensores[4] == 0)) {
              mode = FOLLOWING_LINE; error = 2;        
            }
              else {
              if ((Sensores[0] == 0)&&(Sensores[1] == 0)&&(Sensores[2] == 1)&&(Sensores[3] == 1)&&(Sensores[4] == 0)) {
                mode = FOLLOWING_LINE; error = 1;
              }
              else {
                if ((Sensores[0] == 0)&&(Sensores[1] == 0)&&(Sensores[2] == 1)&&(Sensores[3] == 0)&&(Sensores[4] == 0)) {
                  mode = FOLLOWING_LINE; error = 0;
                }
                else {
                  if ((Sensores[0] == 0)&&(Sensores[1] == 1)&&(Sensores[2] == 1)&&(Sensores[3] == 0)&&(Sensores[4] == 0)) {
                    mode = FOLLOWING_LINE; error = -1;
                  }
                  else {
                    if ((Sensores[0] == 0)&&(Sensores[1] == 1)&&(Sensores[2] == 0)&&(Sensores[3] == 0)&&(Sensores[4] == 0)) {
                      mode = FOLLOWING_LINE; error = -2;
                    }
                    else {
                      if ((Sensores[0] == 1)&&(Sensores[1] == 1)&&(Sensores[2] == 0)&&(Sensores[3] == 0)&&(Sensores[4] == 0)) {
                        mode = FOLLOWING_LINE; error = -3;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

void motorStop(int time) {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(time);
}

void motorForward() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}



void motorTurn(int direction, int time) {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(500);
  switch(direction) {
    case 1:
      motor1.setSpeed(140 + Kp*direction); 
      motor2.setSpeed(120 + Kp*direction);
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
      break;
    case 2:
      motor1.setSpeed(140 + Kp*direction); 
      motor2.setSpeed(120 + Kp*direction);
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
      time = 160; 
      break;
    case 3:
      motor1.setSpeed(140 + Kp*direction); 
      motor2.setSpeed(120 + Kp*direction);
      motor1.run(FORWARD);
      motor2.run(BACKWARD); 
      time = 200;  
      break;
    case -1:
      motor1.setSpeed(140 + Kp*(-direction)); 
      motor2.setSpeed(120 + Kp*(-direction)); 
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
      break;
    case -2:
      motor1.setSpeed(140 + Kp*(-direction)); 
      motor2.setSpeed(120 + Kp*(-direction)); 
      motor1.run(BACKWARD);
      motor2.run(FORWARD);     
      time = 160;
      break;
    case -3:
      motor1.setSpeed(140 + Kp*(-direction)); 
      motor2.setSpeed(120 + Kp*(-direction)); 
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
      time = 200;
      break;
  }
  delay(time);
  motorStop(500);
}

void runExtraInch(void) {
  motorForward();
  delay(500);
  motorStop(1000);
}

void goAndTurn(int direction, int degrees) {
  motorForward();
  delay(900);
  motorTurn(direction, degrees);  
}

void followingLine(void) {
  if (error == 0) {
    motorForward();    
  }
  else {
    motorTurn(error, 120);
  }
}

void endMaze() {
  motorStop(500);
  mode = STOPPED;
  estado = 1;
}


void resolverLaberinto(void) {
  while (!estado) {
    motor1.setSpeed(160);
    motor2.setSpeed(120);
    leerSensores();
    switch (mode) {
      case NO_LINE:
        motorStop(1000);
        goAndTurn(IZQUIERDA, 2550);
        intersecciones('B');
        break;
      case CONT_LINE:
        motorStop(1000);
        runExtraInch();
        leerSensores();
        if (mode == CONT_LINE) 
          endMaze();
        else {
          goAndTurn(IZQUIERDA, 1287);
          intersecciones('L');
        }
        break;
      case RIGHT_TURN:
        motorStop(1000);
        runExtraInch();
        leerSensores();
        if (mode == NO_LINE) {
          goAndTurn(DERECHA, 1325);
          intersecciones('R');
        }
        else 
          intersecciones('S');  
        break;
      case LEFT_TURN:
        motorStop(1000);
        runExtraInch();
        goAndTurn(IZQUIERDA, 1250);
        intersecciones('L');
        break;
      case FOLLOWING_LINE:
        followingLine();
        break;
    }
  }
}

void intersecciones(char direccion) {
  camino[LongitudCamino] = direccion;
  LongitudCamino++;
   Serial.print("camino[LongitudCamino]: ");
  Serial.println(direccion);
 
   simplificarCamino();
}

void simplificarCamino() {
  if (LongitudCamino < 3 || camino[LongitudCamino-2] != 'B')
    return;

  int anguloTotal = 0;
  int i;

  for (i = 1; i <= 3; i++) {
   
    switch (camino[LongitudCamino-i]) {
      case 'R':
        anguloTotal += 90;
        break;
      case 'L':
        anguloTotal += 270;
        break;
      case 'B':
        anguloTotal += 180;
        break;
    }
  }

  anguloTotal = anguloTotal % 360;

  switch(anguloTotal) {
   /*  Serial.print("Camino");
    Serial.println(anguloTotal);*/
    case 0:
      camino[LongitudCamino-3] = 'S';
      break;
    case 90:
      camino[LongitudCamino-3] = 'R';
      break;
    case 180:
      camino[LongitudCamino-3] = 'B';
      break;
    case 270:
      camino[LongitudCamino-3] = 'L';
      break;
  }

  LongitudCamino -= 2; 
}

void optimizarLaberinto(void) {
  while (!estado) {
    motor1.setSpeed(160);
    motor2.setSpeed(120);
    leerSensores();
    switch (mode) {
      case FOLLOWING_LINE:
        followingLine();
        break;
      case CONT_LINE:
        if (indiceCamino >= LongitudCamino){
          leerSensores();
          while(mode = FOLLOWING_LINE){
            followingLine();
            leerSensores();
          }
          endMaze();
        }
        else {
          cambiarDireccion(camino[indiceCamino]);
          indiceCamino++;
        }
        break;
      case RIGHT_TURN:
        if (indiceCamino >= LongitudCamino) {
          leerSensores();
          while(mode = FOLLOWING_LINE){
            followingLine();
            leerSensores();
          }
          endMaze();
        }
        else {
          cambiarDireccion(camino[indiceCamino]);
          indiceCamino++;
        } 
        break;
      case LEFT_TURN:
        if (indiceCamino >= LongitudCamino){
          leerSensores();
          while(mode = FOLLOWING_LINE){
            followingLine();
            leerSensores();
          }
          endMaze();
        }
        else {
          cambiarDireccion(camino[indiceCamino]);
          indiceCamino++;
        }
        break;
    }
  }  
}

void cambiarDireccion(char dir) { //CAMBIAR TIEMPOS!!!!!!!!!!!!!!!!!!!!
  switch(dir) {
    case 'L': // Girar a la Izquierda
      goAndTurn(IZQUIERDA, 1250);      
      break;   
    case 'R': // Girar a la Derecha
      goAndTurn(DERECHA, 1325);     
      break;   
    case 'B': // Girar 180
      goAndTurn(IZQUIERDA, 2550);     
      break;   
    case 'S': // Seguir derecho
      runExtraInch();
      break;
  }
}

void loop() {
  if(digitalRead(boton) == HIGH && control == 0) {
    leerSensores();
    resolverLaberinto();
    control = 1;
  }
  indiceCamino = 0;
  estado = 0;
 if(digitalRead(boton) == HIGH && control == 1) {
    optimizarLaberinto();
    control = 0;
    estado = 0;
    indiceCamino = 0;
    LongitudCamino = 0;
 
  }
}

