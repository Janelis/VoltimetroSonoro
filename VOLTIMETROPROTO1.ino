//Inicialização de bibliotecas
#include <SD.h>                            // Biblioteca para acessar o cartão de memoria
#include <TMRpcm.h>                        // Biblioteca que executa os arquivos de audio
#include <SPI.h>

TMRpcm player;                             // Objeto "player", utilizado pela biblioteca TMRpcm. Nos define qual o nome do objeto que seguira as instruções contidas na biblioteca TMRpcm

#define SD_ChipSelectPin 10                //Pino responsável pelo pino CS no módulo de cartão de memória, Chip Select
#define pinoLeitura A0
#define pinoBotao 11
#define pinoLED 8                          //Pino com LED vermelho, caso haja algo errado com o cartão de memória do aparelho

bool debug = 1;                            //Debug, trocar para 1 caso queira observar algum valor pela serial


float valorLeitura = 0;                      //Valor da leitura
float leituraAcum = 0;                       //Acumulador de leituras
int n = 0;                                 //Contador para o while loop
int k = 0;                                 //Contador para o loop que compoe a fala
int tempoLeitura = 750;
int decimal = 0;


void setup() {

  player.speakerPin = 9;                   //Pino 9 no caso do Arduino Uno. Deve ser um pino PWM
  pinMode(pinoLED, OUTPUT);                     //LED interno do Arduino, utilizado para indicar a leitura do cartão
  pinMode(pinoLeitura, INPUT);
  pinMode(pinoBotao, INPUT);

  if (debug == 1) {                        //Comunicação serial para debug
    Serial.begin(9600);
    while (!SD.begin(SD_ChipSelectPin)) {  //Observa se o cartão SD está inserido
      Serial.println("SD não encontrado");
      digitalWrite(pinoLED, HIGH);
      delay(1000);
    }
    Serial.println("SD Ok!");
    digitalWrite(pinoLED, LOW);

  }
  else {
    while (!SD.begin(SD_ChipSelectPin)) {  //Observa se o cartão SD está inserido, porém sem a comunicação na Serial
      digitalWrite(pinoLED, HIGH);
      delay(1000);
    }
    digitalWrite(pinoLED, LOW);
  }



}



void loop() {

  //leitura dos valores do pino analogico, caso o botao seja pressionado
  //  if (digitalRead(pinoBotao) == 1) {
  while (n < 10) {
    valorLeitura = float(analogRead(pinoLeitura));
    leituraAcum = leituraAcum + valorLeitura;
    n++;
  }
  valorLeitura = leituraAcum / 10;    //Retorna a média das 10 leituras feitas no while loop
  float leituraVolts = mapFloat(valorLeitura, 0.0, 1023.0, 0.0, 4.99); //Converte a média das 10 leituras para volts
  n = 0;

  String splitLeitura = String(leituraVolts, 2); //Converte a leitura para uma string, para ser avaliada e para que sejam executados os sons adequados que compõem a fala
  char digito1 = splitLeitura[0];
  char digito2 = splitLeitura[1];
  char digito3 = splitLeitura[2];
  char digito4 = splitLeitura[3];

  Serial.print(digito1);
  Serial.print(digito2);
  Serial.print(digito3);
  Serial.println(digito4);

  //Processar os digitos da leitura e executar os sons!!!

  switch (digito1) {
    case '0': player.play("0.wav"); break;
    case '1': player.play("1.wav"); break;
    case '2': player.play("2.wav"); break;
    case '3': player.play("3.wav"); break;
    case '4': player.play("4.wav"); break;
    case '5': player.play("5.wav"); break;
    case '6': player.play("6.wav"); break;
    case '7': player.play("7.wav"); break;
    case '8': player.play("8.wav"); break;
    case '9': player.play("9.wav"); break;
  }
  delay(tempoLeitura);
  player.play("virgula.wav");
  delay(tempoLeitura);
  switch (digito3) {
    case '0':
      player.play("0.wav");
      break;
    case '1':
      decimal = 2;
      break;
    case '2':
      player.play("20.wav");
      decimal = 1;
      break;

    case '3':
      player.play("30.wav");
      decimal = 1;
      break;

    case '4':
      player.play("40.wav");
      decimal = 1;
      break;

    case '5':
      player.play("50.wav");
      decimal = 1;
      break;

    case '6':
      player.play("60.wav");
      decimal = 1;
      break;

    case '7':
      player.play("70.wav");
      decimal = 1;
      break;

    case '8':
      player.play("80.wav");
      decimal = 1;
      break;

    case '9':
      player.play("90.wav");
      decimal = 1;
      break;
  }
  if (decimal = 1) {
    delay(tempoLeitura);
    player.play("E.wav");
    delay(tempoLeitura);
    switch (digito4) {
      case '0': player.play("0.wav"); break;
      case '1': player.play("1.wav"); break;
      case '2': player.play("2.wav"); break;
      case '3': player.play("3.wav"); break;
      case '4': player.play("4.wav"); break;
      case '5': player.play("5.wav"); break;
      case '6': player.play("6.wav"); break;
      case '7': player.play("7.wav"); break;
      case '8': player.play("8.wav"); break;
      case '9': player.play("9.wav"); break;
    }
    decimal = 0;
  }
  else if (decimal = 2) {
    switch (digito4) {
      case '0': player.play("10.wav"); break;
      case '1': player.play("11.wav"); break;
      case '2': player.play("12.wav"); break;
      case '3': player.play("13.wav"); break;
      case '4': player.play("14.wav"); break;
      case '5': player.play("15.wav"); break;
      case '6': player.play("16.wav"); break;
      case '7': player.play("17.wav"); break;
      case '8': player.play("18.wav"); break;
      case '9': player.play("19.wav"); break;
    }
    decimal = 0;
  }
  delay(tempoLeitura);
  player.play("volts.wav");
  delay(10000);
  leituraAcum = 0;
}




float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


////Este trecho do programa foi retirado de https://www.blogdarobotica.com/2021/05/24/entenda-o-efeito-bounce-e-como-fazer-debounce-no-arduino-ao-apertar-um-botao/
////e é uma tradução do exemplo "Debounce" presente na IDE Arduino nativamente, responsável por evitar múltiplos sinais do botão por acionamento
//
//void debounceBotao() {
//  int leitura = digitalRead(pinoBotao);                   //A variável leitura recebe a leitura do pino do botão: HIGH (pressionado) ou LOW (Desacionado)
//  if (leitura != ultimoStatusBotao) {                     //Se a leitura atual for diferente da leitura anterior
//    tempoUltimoDebounce = millis();                       //Reseta a variável tempoUltimoDebounce atribuindo o tempo atual para uma nova contagem
//  }
//  if ((millis() - tempoUltimoDebounce) > tempoDebounce) { //Se o resultado de (tempo atual - tempoUltimoDebounce) for maior que o tempo que determinamos (tempoDebounce), ou seja, já passou os 50 milissegundos que é o tempo que o botão precisa ficar pressionado para ter a certeza de que ele realmente foi pressionado? Se sim faça:
//    if (leitura != statusBotao) {                         //Verifica se a leitura do botão mudou, ou seja, se é diferente do status que o botão tinha da última vez. Se sim, faça:
//      statusBotao = leitura;                              //statusBotao recebe o que foi lido na variável leitura (pressionado = 1 e não pressionado = 0)
//    }
//  }
//  ultimoStatusBotao = leitura;                            //Atualiza a variável ultimoStatusBotao para o que foi lido na variável leitura
//}
