#include <QTRSensors.h>
//////Constantes para motores - NAO MEXER/////////////////
#define BACKWARD 0
#define FORWARD 1
//////////////////////////////////////////////////////////
const int IN1= 13,
	  IN2= 12,
	  IN3= 11,
	  IN4= 10;

QTRSensorsAnalog qtra((unsigned char[]) { A0,A1,A2 }, 3);

unsigned int sensors[3];

int erro = 0, deltaTime = 0, last_time = 0, P = 0, I = 0, D = 0, last_erro = 0;
const int Motors = 150;

const float KP = 0.10, KI = 0, KD = 0;

void setup() {
  calibrar(200,200);
}

void loop() {
  sigaLinha();
}


void sigaLinha(){
  int position = qtra.readLine(sensors);
  erro = 3500 - position; //Segue a linha preta(devido a montagem invertida dos motores)
  //erro = position - 3500; //"Foge" da linha preta(devido a montagem invertida dos motores)
  deltaTime = millis() - last_time;
  last_time = millis();
  last_erro = erro;

  P = erro * KP;
  I += (erro * KI) * deltaTime;
  D = (last_erro - erro) * KD / deltaTime;

  int velocidade = P + I + D;

  int m1 = Motors - velocidade;
  int m2 = Motors + velocidade;

  m1 = constrain(m1, -250, 250);
  m2 = constrain(m2, -250, 250);
  
  move(m1, m2);
  
}



void calibrar(int vel1, int vel2){
  move(200, -200);
    //Faz o robï¿½ girar para a Direita com a mesma velocidade em ambos os motores
  for(int x = 0; x < 90; x++){
    qtra.calibrate(); //Calibra os sensores
    delay(20); //Aguarda 0.02 segundos antes de prosseguir o loop
  }
  
  while(qtra.readLine(sensors) < 3000 || qtra.readLine(sensors) > 3900) move(-200, 200);
}
/*------------------------------------------*/

int setSpeeds(int vel) { 
  if(vel < 0) return -vel; //Se o valor (velocidade neste caso) entregue a funï¿½ï¿½o for negativo, retorne o oposto desse valor, isto ï¿½, multiplique o valor negativo por "-1".
  return vel; //...Retorne o mesmo valor
}

int sentido(int vel){
  if(vel < 0) return BACKWARD; //Se o valor (velocidade neste caso) entregue a funï¿½ï¿½o for negativo, retorne o sentido como sendo para TRï¿½S, caso contrï¿½rio...
  return FORWARD; //...Retorne o sentido como sendo para FRENTE
}

void move(int V1, int V2){
  if((sentido(V1)) == FORWARD) {
    analogWrite(IN1, (setSpeeds(V1)));
    analogWrite(IN2, 0);
  }
  else if((sentido(V1)) == BACKWARD) {
   analogWrite(IN1, 0); 
   analogWrite(IN2, (setSpeeds(V1)));
  }
  
  if((sentido(V2)) == FORWARD) {
    analogWrite(IN3, (setSpeeds(V2)));
    analogWrite(IN4, 0);
  }
  else if((sentido(V2)) == BACKWARD) {
    analogWrite(IN3, 0); 
    analogWrite(IN4, (setSpeeds(V2)));
  }
}
