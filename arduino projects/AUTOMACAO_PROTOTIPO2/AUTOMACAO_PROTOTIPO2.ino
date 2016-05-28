
#include <Ethernet.h>
#include <SPI.h>

/*** Pinos 4, 10, 11, 12 e 13 pinos de comunicação ethernet, não podem ser usados ***/

const int TempPins[1] =     { A0 }; 						// A0    - Entrada dos sensores de temperatura LM35

const int CorrentePins[5] = { A1, A2, A3, A4, A5 };           // A1/5  - Entrada dos sensores de corrente ACS712_05
const int AmpSensores[5]  = {  5, 20,  0,  0,  0 };           // A1/5  - Entrada dos sensores de corrente ACS712_05
const int RelesPins[5] =    {  5,  6,  0,  0,  0};                         // D5/6  - Saída dos Reles

const int LedPin = 7;                                        // D7  - Pino Led
const int TriggerPin = 8;                                    // D8  - Pino Trigger
const int EchoPin = 9;                                       // D9  - Pino Echo 

const int seg = 1000;                                        // 1 Seg em Milisegundos
const float conversaoAD = 0.00488828125;                     // Conversão da entrada analógica/digital. Arduino trabalha com 10bits, 0 a 1023 intervalos (5V : 1024 Intervalos = 4,88mV por intervalo).

/*** Configurações de Rede do Arduino ***/
byte mac[] =     { 0x90,0xA2,0xDA,0x00,0x55,0x8D };          // endereço MAC da placa Ethernet, caso a placa não venha com o endereço próprio utilize este.
byte ip[] =      { 192, 168, 0, 10 };                        // ip do arduino
byte gateway[] = { 192, 168, 0, 1 };                         // ip do roteador
byte subnet[] =  { 255, 255, 255, 0 };                       // Mascára de rede

char webserver_ip[] =  { "192.168.0.2" };                    // IP do servidor (webserver - VirtualMachine) para conexão caso não consiga resolver o nome
char webserver_dns[] = { "http://automacao.hopto.org/" };    // Nome do servidor para conexão
int porta = 80;                                              // Porta de conexão ao servidor (usando 80, pois assim não é necessários digitar porta no navegador)

EthernetClient cliente;                                      // Client para conexão ao servidor
EthernetServer server(porta);                                // Cria o servidor na porta desejada

/*
// avaliando melhor forma de armazenar dados (em teste)
struct struct {
   int ambiente; 
   int dispositivo;
   int local;
   int statusDisp;
} est_dispositivo;

est_dispositivo dispositivos[6];
*/

#include "COMUM.h"                                          
#include "ACS712_CORRENTE.h"
#include "ETHERNET_W5100.h"                                  // Bibliotecas que fiz em arquivos separados para tornar codigo mais simples
#include "HCSR04_SOM.h"
#include "LM35_TEMPERATURA.h"

long tempoSom = 0;                                           // Tempo de resposta ondas sonoras
long tempoDelay = 0;                                         // Tempo de delay a ser inserido no sistema
float temperatura = 0.0;                                       // valor atual da temperatura
float temperatura_ant = 0.0;                                   // Valor anterior da temepratura para comparacao
float corrente = 0;                                          // Valor atual da corrente
int statusDispositivob = 0;
int statusDispositivob_ant = 0;

int statusDispositivo_ant[2] = {0};                          // Status atual dos dispostivos
int statusDispositivo[2] = {0};                              // Status anterior dos dispostivos para comparacao

boolean hcsr04_ativado = false;               //som          //
boolean lm35_ativado   = false;               //temp         // Variaveis booleanas para ativar ou os recursos
boolean acs712_ativado = false;               //corrente     //

String data = "";                                            // Variavel que receberá os dados a ser enviado ao webserver

/*
// Lampada Sala Teto
dispositivo[0] = (record_type) {3,1,1}; 
dispositivo[1] = (record_type) {3,3,2}; 
dispositivo[2] = (record_type) {3,3,3}; 
dispositivo[3] = (record_type) {3,3,4}; 
dispositivo[4] = (record_type) {3,3,5}; 
dispositivo[5] = (record_type) {3,3,6}; 

// Lampada Sala Teto
dispositivo[0].ambiente = 3;       Sala
dispositivo[0].dispositivo = 3;    Tomada
dispositivo[0].local = 2;          Local X

// Ventilador Sala
dispositivo[1].ambiente = 3;
dispositivo[1].dispositivo = 4;

// Lampada Sala Teto
dispositivo[0].ambiente = 3;
dispositivo[0].dispositivo = 5;
dispositivo[0].local = 2;

// Ventilador Sala
dispositivo[1] = (record_type) {3,6,1}; // tetar setar cada var rapidamente.
ou
dispositivo = (record_type) { {1,2,3}, {1,2,3}, {1,2,3}, {1,2,3}, {1,2,3}, {1,2,3} }; // tetar setar cada var rapidamente.

*/

void setup(){
  
  pinMode(TriggerPin,OUTPUT);                               //
  pinMode(EchoPin,INPUT);                                   // Configurando pinos digitais para 'saida'
  pinMode(LedPin, OUTPUT);                                  //                        
  
  for (int i=0; i<2; i++) 
    pinMode(RelesPins[i], OUTPUT);                          // Definindos pinos digitais de reles como 'saída'
  
  Serial.begin(9600);                                       // Taxa de dados da porta Serial para debugar resultados no pc
  
  Ethernet.begin(mac, ip, gateway, subnet);                 // Inicializa o dispositivo na rede
  server.begin();                                           // Inicializa o dispositivo como servidor
  
  Serial.print("IP do Arduino: ");                          
  for (byte thisByte = 0; thisByte < 4; thisByte++) {       // LOOP em bytes para imprimir o IP do arduino
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
  
  delay(seg);
}

void loop(){ 
  
  // Configuração para ativar dispositivos (apenas para facilitar testes neste momento)
  if (data == "som#ativar") hcsr04_ativado = true;
  if (data == "som#desativar") hcsr04_ativado = false;
  
  if (data == "corrente#ativar") hcsr04_ativado = true;
  if (data == "corrente#desativar") hcsr04_ativado = false;
  
  if (data == "temp#ativar") hcsr04_ativado = true;  
  if (data == "temp#desativar") hcsr04_ativado = false;  
  
 
  if (hcsr04_ativado) {
    
    digitalWrite(TriggerPin, LOW);                            // Coloca o pino Trigger em LOW
    delayMicroseconds(2);                                     // Mantem ativado por 2us
    digitalWrite(TriggerPin, HIGH);                           // Coloca o pino Trigger em HIGH
    delayMicroseconds(10);                                    // Mantem ativado por 10us
    digitalWrite(TriggerPin, LOW);                            // Desliga o Trigger para então medir o tempo de resposta das ondas sonoras
  
    tempoSom = pulseIn(EchoPin, HIGH);                        // Aguarda o pino echo ficar com status HIGH e retorna a duração em microsegundos
    float distancia = Distancia(tempoSom);                    // Calcula distancia do objeto -> hcsr04_som.h
    
    if (distancia > 0  && distancia < 0.2)   PiscarLampada(LedPin, 1, 1000);         // Se distancia entre 0 e 20 cm pisca 1x 
    if (distancia > 0.2 && distancia < 0.4)  PiscarLampada(LedPin, 2, 1000);         // Se distancia entre 20 e 40 cm pisca 2x
    if (distancia > 0.4 && distancia < 0.7)  PiscarLampada(LedPin, 3, 1000);         // Se distancia entre 40 e 70 cm pisca 3x
    if (distancia > 0.70) {}                                                         // Se distancia maior que 70 cm NÃO pisca
    
    Serial.print("Tempo(Som): ");
    Serial.print(tempoSom/2);
    Serial.println(" us");
  
    Serial.print("Distancia: ");  
    Serial.print(ConversaoUnidade(distancia, "m", "cm"));         // hcsr04_som.h -> melhorar funcao
    Serial.println(" cm");
    Serial.println();
    
  }
  
  
  if (lm35_ativado) {
    
    temperatura_ant = temperatura;
    temperatura = Temperatura(TempPins[0]);                                    // acs712_corrente.h
    
    Serial.print(temperatura);
    Serial.println(" C");
    Serial.println();
    
    if ( temperatura <= (temperatura_ant - 0.5) || temperatura >= temperatura_ant + 0.5) {
      
      Serial.print("Temp. antiga: ");    
      Serial.println(temperatura_ant);
      Serial.print("Temp. atual: ");    
      Serial.println(temperatura);
      Serial.println();
    
      data = "bd=15|2|0|";
      //data += String(temperatura, 2);                                    // forma de envio dos dados => #ar#= 'ambiente''dispositivo''local''status'
      
      EnviaDados(webserver_ip, data, "POST");                           // ethernet_w5100.h
    
    }
    
  }
  
  // FUNCIONANDO
  if (acs712_ativado) {
    
    statusDispositivob_ant = statusDispositivob;
    statusDispositivob = StatusDispositivo(CorrentePins[0], 20);
    
    if (statusDispositivob == 1) {
      corrente = CorrenteMedia(CorrentePins[0], 20, 10000);
      Serial.print(corrente, 3);
      Serial.println(" A");
    }
    
    if ( statusDispositivob != statusDispositivob_ant) {
    
      //if (statusDispositivob == 1) corrente = CorrenteMedia(CorrentePins[0], 20, 10000);
      
      Serial.print("Status antigo: ");    
      Serial.println(statusDispositivob_ant);
      Serial.print("Status atual: ");    
      Serial.println(statusDispositivob);
      Serial.println();
    
      data = "bd=15|1|6|";
      data += String(statusDispositivob);                                    // forma de envio dos dados => #ar#= 'ambiente''dispositivo''local''status'
      
      EnviaDados(webserver_ip, data, "POST");                           // ethernet_w5100.h
    
    }
    
    data = "";
    data = RecebeDados();
    
    if (data != "" && data != "-1") {
          
      Serial.print("Dados recebidos: ");
      Serial.println(data);
          
      if (data.charAt(2) == '1' || data.charAt(2) == '0') {
        
        if (data.charAt(1) == '5') {
          if (data.charAt(2) == '1') {
            
            digitalWrite(5, LOW); 
            delay(1);
            if ( StatusDispositivo(CorrentePins[1], 5) == 0 ) digitalWrite(5, HIGH); 
            
          } else {
            
            digitalWrite(5, HIGH);
            delay(1);
            if ( StatusDispositivo(CorrentePins[1], 5) == 1 ) digitalWrite(5, LOW); 
            
          }
          
        }
          
        if (data.charAt(1) == '6') {
          if (data.charAt(2) == '1') {
            
            digitalWrite(6, LOW); 
            delay(1);
            if ( StatusDispositivo(CorrentePins[0], 20) == 0 ) digitalWrite(6, HIGH); 
            
          } else {
            
            digitalWrite(6, HIGH);
            delay(1);
            if ( StatusDispositivo(CorrentePins[0], 20) == 1 ) digitalWrite(6, LOW); 
            
          } 
        } 
      }
    } // fim  
  
  // EM TESTE   
  } else {  
      
  for (int i=0; i<2; i++) {
  
    statusDispositivo_ant[i] = statusDispositivo[i];
    corrente = CorrenteMedia(CorrentePins[i], AmpSensores[i], 10);
    if (corrente > 0.0) statusDispositivo[i] = 1; else statusDispositivo[i] = 0; 
    
    /* Debug para saber se está lendo os 2 dispositivos e qual a corrente neles.
    if (statusDispositivo[i] == 1) {
      
      corrente = CorrenteMedia(CorrentePins[i], AmpSensores[i], 10000);
      
      Serial.println("-----------");
      Serial.print("Disp. ");
      Serial.println(i);
      Serial.println("-----------");
        
      Serial.print("Status ant: ");
      Serial.println(statusDispositivo_ant[i]);
      Serial.print("Status Atl: ");
      Serial.println(statusDispositivo[i]);
      
      Serial.print("Corrente = ");    
      Serial.print(corrente, 3);
      Serial.println(" A");
   
    }*/
    
    if ( statusDispositivo[i] != statusDispositivo_ant[i]) {
      
      corrente = CorrenteMedia(CorrentePins[i], AmpSensores[i], 10000);
      
      Serial.println("-----------");
      Serial.print("Disp. ");
      Serial.println(i);
      Serial.println("-----------");
        
      Serial.print("Status ant: ");
      Serial.println(statusDispositivo_ant[i]);
      Serial.print("Status Atl: ");
      Serial.println(statusDispositivo[i]);
      
      Serial.print("Corrente = ");    
      Serial.print(corrente, 3);
      Serial.println(" A");
    
      //if (statusDispositivo[i] == 1) 
        //corrente = CorrenteMedia(CorrentePins[i], AmpSensores[i], 10000);
      
      //Serial.print(i);
      //Serial.print("Status antigo: ");    
      //Serial.println(statusDispositivo_ant[i]);
      //Serial.print("Status atual: ");    
      //Serial.println(statusDispositivo[i]);
      //Serial.println();
      //Serial.print(corrente, 3);
      //Serial.println(" A");
      
      data = "bd=15|1|";
      data += String(RelesPins[i])+"|";                                    // identifica o pino digital do dispositivo
      data += String(statusDispositivo[i]);                                    // forma de envio dos dados => #ar#= 'ambiente''dispositivo''local''status'
      
      Serial.println(data);
      Serial.println();
      //EnviaDados(webserver_ip, data, "POST");                           // ethernet_w5100.h
    
    }
    delay(seg);
    
  } //fim for
  
  data = "";
  data = RecebeDados();
  
  if (data != "" && data != "-1") {
        
    Serial.print("Dados recebidos: ");
    Serial.println(data);
        
    if (data.charAt(2) == '1' || data.charAt(2) == '0') {
      
      if (data.charAt(1) == '5') {
        if (data.charAt(2) == '1') {
          
          digitalWrite(5, LOW); 
          delay(1);
          if ( StatusDispositivo(CorrentePins[0], 5) == 0 ) digitalWrite(RelesPins[0], HIGH); 
          
        } else {
          
          digitalWrite(5, HIGH);
          delay(1);
          if ( StatusDispositivo(CorrentePins[0], 5) == 1 ) digitalWrite(RelesPins[0], LOW); 
          
        }
        
      }
        
      if (data.charAt(1) == '6') {
        if (data.charAt(2) == '1') {
          
          digitalWrite(6, LOW); 
          delay(1);
          if ( StatusDispositivo(CorrentePins[1], 20) == 0 ) digitalWrite(RelesPins[1], HIGH); 
          
        } else {
          
          digitalWrite(6, HIGH);
          delay(1);
          if ( StatusDispositivo(CorrentePins[1], 20) == 1 ) digitalWrite(RelesPins[1], LOW); 
          
        }
        
      }
      
    }
  } 
 
 }
 
 delay(seg/2);
  
} // FIM LOOP

