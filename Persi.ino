//feito por: 2018 @leandro_mucciaroni   www.youtube.com/lmucciaroni
#include "SoftwareSerial.h" //usado para o controle via bluetooth
#include <Servo.h> //controle do servo
Servo myservo;
int ldr = 0;
int ldr[50]; 
char posicao;
int modo;
int i = 0;
int val = 0;

void leituraldr() { //parte responsavel por ler os valores do sensor de luminosidade
  //val = map(lf, 0, 1023, 0, 180); //parte opcional, estava tentando fazer um filtro para os valores do ldr
  for (i = 0; i < 49; i++)
    ldr [ i + 1 ] = ldr [i];

  ldr [0] = analogRead(ldr);
  val = 0;

  for (i = 0; i < 50; i++)
    val = val + ldr[i];

  val = val / i;
  Serial.println(val);
  delay(500);
}

void servo() {
  if (val > 301 ) {
    myservo.write(180);
  }
  else if (val > 101 && val < 300) {
    myservo.write(100);
  }
  else if (val < 100) {
    myservo.write(0);
  }
  delay(500);
}

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(12, INPUT);
  pinMode(11, INPUT);
  myservo.attach(3);
}

void loop() {
  if (Serial.available() > 0) {
    posicao = Serial.read();
    Serial.println(posicao);
  }
  if (posicao == 'A') {

    modo = 2;
  }
  if (posicao == 'B') {
    modo = 1;
  }
  // =================================================== Manual =====================================================================
  // Os valores recebidos para modo e posição serão enviados por bluetooth pela serial
  while ( modo == 1) {
    delay(100);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    if (Serial.available() > 0) {
      posicao = Serial.read();
      Serial.println(posicao);
    }


    while (posicao == 'S' && digitalRead(12) == 1) {
      if (Serial.available() > 0) {
        posicao = Serial.read();
        Serial.println(posicao);
      }
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
    }

    while (posicao == 'D') {
      if (Serial.available() > 0) {
        posicao = Serial.read();
        Serial.println(posicao);
      }
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);

    }


    if ( posicao == 'G') {

      myservo.write(0);
    }

    if ( posicao == 'H') {

      myservo.write(45);
    }

    if ( posicao == 'I') {

      myservo.write(90);
    }


    if ( posicao == 'J') {

      myservo.write(135);
    }

    if ( posicao == 'K') {

      myservo.write(180);
    }

    if ( posicao == 'A') {

      modo = 2;


    }

  }

  // ==================================================================Automatico=============================================================================
// só define a abertura das pás de acordo com o sensor de luminosidade
// obs: o sensor pode ser substituido por um de temperatura
  while ( modo == 2) {

    if (Serial.available() > 0) {
      posicao = Serial.read();
      Serial.println(posicao);
    }

    leituraldr();
    servo();

    if ( posicao == 'B') {

      modo = 1;
    }

  }
}
