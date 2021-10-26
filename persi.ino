//feito por: @leandro_mucciaroni   www.youtube.com/lmucciaroni

#include "SoftwareSerial.h" //bluetooth
#include <Servo.h> //servo

/* pinos utilizados:
  Digitais:
  0 bluetooth
  1 bluetooth
  2 botao pra subir
  3 botao pra descer
  4 servo motor das aletas
  8 motor in a
  9 motor in b
  12 sensor efeito hall
  Analogicos:

*/


Servo aletas; //inicia um servo motor para controlar as aletas

int pinoldr = A0; //ldr conectado em A0
int pdservo = 4; //pino do servo

int ldr; //armazena leitura do ldr
int val = 0; //armnazena valor ldr filtrado

char bt; //armazena caracteres recebidos pelo bluetooth

int estado = 0; //verificar sensor - caso n esteja funcionando mude para false

//pinos dos botoes
int botao1 = 2; //botao para subir
int botao2 = 3; //botao para descer

//pinos do motor
int ina = 5;
int inb = 6;

int sensor = 7; //pino do fim de curso de efeito hall

int s1 = 8;
int s2 = 9;
int s3 = 10;
int s4 = 11;
int s5 = 12;

int modo = 1;



void leituraldr()   //parte responsavel por ler os valores do sensor de luminosidade
{

  for (int i = 0; i < 50; i++)
  {
    ldr = analogRead(pinoldr); //le o ldr
    val += ldr; //soma 50 leituras do ldr
  }
  val = val / 50; //faz a media das leituras do ldr

  val = map(val, 0, 1023, 0, 180); //mapeia val entre 0 e 180
  if (val < 0) {
    val = -1 * val;
  }
  if ( bt == 'B') //veridica se ainda está no modo AUTOMATICO
  {

    modo = 1;

  }

  Serial.println(val); //printa val

  delay(50);//aguarda 01s

}

void servo() //controle do servo de acordo com o valor do ldr
{
  //testar a intensidade luminosa e alterar nos if de acordo com o que julgar necessário (necessario calibração)

  if (val > 80 )
  {
    aletas.write(180); //aletas 100% abertas
  }
  else if (val > 40 && val <= 80)
  {
    aletas.write(90); //aletas no meio
  }
  else if (val <= 40)
  {
    aletas.write(0); //aletas 100% fechadas
  }
  if ( bt == 'B') //veridica se ainda está no modo AUTOMATICO
  {

    modo = 1;

  }

  delay(50);//aguarda 01.s
}

void setup()
{
  Serial.begin(9600); //inicia serial

  pinMode(ina, OUTPUT); //pino 9 como saida  -pino motor
  pinMode(inb, OUTPUT); //pino 10 como saida  -pino motor

  pinMode(sensor, INPUT);  //pino 12 como entrada
  pinMode(11, INPUT);  //pino 11 como entrada

  pinMode(botao1, INPUT);  //pino botao como entrada
  pinMode(botao2, INPUT);  //pino botao como entrada

  pinMode(s1, INPUT);  //pino botao como entrada
  pinMode(s2, INPUT);  //pino botao como entrada
  pinMode(s3, INPUT);  //pino botao como entrada
  pinMode(s4, INPUT);  //pino botao como entrada
  pinMode(s5, INPUT);  //pino botao como entrada
  

  aletas.attach(pdservo); //servo das aletas
}

void loop()
{

  if (Serial.available() > 0)
  {
    bt = Serial.read(); //le a serial (o que o bluetooth enviar)
    Serial.println(bt); //printa isso
  }
  if (bt == 'A')
  {
    modo = 2; //automatico
  }
  if (bt == 'B')
  {
    modo = 1; //manual
  }

  // =================================================== Manual =====================================================================
  // Os valores recebidos para modo e posição serão enviados por bluetooth pela serial ou pelo botão

  while (modo == 1)
  {
    delay(100);
    digitalWrite(ina, LOW); //para o motor
    digitalWrite(inb, LOW); //para o motor


    if (Serial.available() > 0)
    {
      bt = Serial.read(); //le a serial (o que o bluetooth enviar)
      Serial.println(bt); //printa isso
    }

    //usando bluetooth
    while (bt == 'S' && digitalRead(sensor) == estado) //sobe
    {
      if (Serial.available() > 0)
      {
        bt = Serial.read(); //le a serial (o que o bluetooth enviar)
        Serial.println(bt); //printa isso
      }
      //subindo
      digitalWrite(ina, HIGH);
      digitalWrite(inb, LOW);
    }

    while (bt == 'D') //desce
    {
      if (Serial.available() > 0)
      {
        bt = Serial.read(); //le a serial (o que o bluetooth enviar)
        Serial.println(bt); //printa isso
      }
      //descendo
      digitalWrite(ina, LOW);
      digitalWrite(inb, HIGH);

    }

    //usando os botoes
    while (digitalRead(botao1) == 1 && digitalRead(sensor) == estado) //sobe
    {
      //subindo
      digitalWrite(ina, HIGH);
      digitalWrite(inb, LOW);
      Serial.println('S'); //printa isso
    }

    while (digitalRead(botao2) == 1) //desce
    {
      //descendo
      digitalWrite(ina, LOW);
      digitalWrite(inb, HIGH);
      Serial.println('D'); //printa isso

    }


    //controle manual do servo pela barra no app
    if ( bt == 'G' ||digitalRead(s5))
    {

      aletas.write(0);
    }

    if ( bt == 'H'||digitalRead(s4 ))
    {

      aletas.write(45);
    }

    if ( bt == 'I'||digitalRead(s3))
    {

      aletas.write(90);
    }


    if ( bt == 'J'||digitalRead(s2))
    {

      aletas.write(135);
    }

    if ( bt == 'K'||digitalRead(s1))
    {

      aletas.write(180);
    }

    if ( bt == 'A') //veridica se ainda está no modo manual
    {

      modo = 2;

    }

  }

  // ==================================================================Automatico=============================================================================
  // só define a abertura das pás de acordo com o sensor de luminosidade
  // obs: o sensor pode ser substituido por um de temperatura (necessario calibração)

  while ( modo == 2)
  {
    delay(100);
    digitalWrite(ina, LOW); //para o motor
    digitalWrite(inb, LOW); //para o motor

    if (Serial.available() > 0)
    {
      bt = Serial.read(); //le a serial (o que o bluetooth enviar)
      Serial.println(bt); //printa isso
    }
     if ( bt == 'B') //veridica se ainda está no modo automatico
    {
      modo = 1;
    }
    leituraldr(); //le o ldr

    delay(100);//aguarda 0.4s

    servo(); //controla o servo de acordo com a leitura do ldr

    if (val > 40 && val <= 80)
    {
      //usando os botoes pra subir e descer
      while (digitalRead(botao1) == 1 && digitalRead(sensor) == estado) //sobe
      {
        //subindo
        digitalWrite(ina, HIGH);
        digitalWrite(inb, LOW);
        Serial.println('S'); //printa isso
      }

      while (digitalRead(botao2) == 1) //desce
      {
        //descendo
        digitalWrite(ina, LOW);
        digitalWrite(inb, HIGH);
        Serial.println('D'); //printa isso

      }
    }

    if ( bt == 'B') //veridica se ainda está no modo automatico
    {
      modo = 1;
    }



  }
}
