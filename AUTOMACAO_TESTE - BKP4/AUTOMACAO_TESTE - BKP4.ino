
#include <Ethernet.h>
#include <SPI.h>

/*** Pinos 4, 10, 11, 12 e 13 pinos de comunicação ethernet, não podem ser usados ***/
const int LedPin = 6;                                        // D6  - Pino Led
const int RelePin = 7;                                       // D7  - Pino do Rele (Lampada)
const int TriggerPin = 8;                                    // D8  - Pino Trigger
const int EchoPin = 9;                                       // D9  - Pino Echo 
const int TempPin = A0;                                      // A0  - Entrada do sensor analógico de temperatura LM35DZ
const int CorrentePin = A1;                                  // A1  - Entrada do sensor de corrente ACS712_05

const int dispositivosPins[6] = { A0, A1, A2, A3, A4, A5 };  // vetor com todos dispositivos analogicos
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
float temperatura = 0;                                       // valor atual da temperatura
float temperatura_ant = 0;                                   // Valor anterior da temepratura para comparacao
float corrente = 0;                                          // Valor atual da corrente
float corrente_ant = 0;                                      // Valor anterior da corrente para comparacao
int statusLampada = 0;
int statusLampada_ant = 0;

int statusDispositivo_ant[6] = {0};                          // Status atual dos dispostivos
int statusDispositivo[6] = {0};                              // Status anterior dos dispostivos para comparacao

boolean hcsr04_ativado = false;               //som          //
boolean lm35_ativado   = false;               //temp         // Variaveis booleanas para ativar ou os recursos
boolean acs712_ativado = false;               //corrente     //

String data     = "";                                        // Variavel que receberá os dados a ser enviado ao webserver
String data_aux = "";                                        // Variavel auxiliar


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
  pinMode(RelePin, OUTPUT);                                 //
  pinMode(4, OUTPUT);                                 //
  pinMode(4, OUTPUT);                                 //
  pinMode(5, OUTPUT);                                 //
  
  Serial.begin(9600);                                       // Taxa de dados da porta de saída Serial para debugar resultados no pc
  
  Ethernet.begin(mac, ip, gateway, subnet);                 // Inicializa o dispositivo na rede
  //server.begin();                                           // Inicializa o dispositivo como servidor
  
  Serial.print("IP do Arduino: ");                          
  for (byte thisByte = 0; thisByte < 4; thisByte++) {       // LOOP em bytes para imprimir o IP do arduino
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
  
  delay(100);
}

void loop(){ 
 
  
  // apenas para teste... com isso envio um comando da pagina web que permite ativar ou nao funções
  //data = RecebeDados();
  if (data == "c#c1") acs712_ativado = true; else acs712_ativado = false;     // Ativando/Desativando recurso de corrente
  if (data == "c#s1") acs712_ativado = true; else acs712_ativado = false;     // Ativando/Desativando recurso de corrente
  if (data == "c#c1") acs712_ativado = true; else acs712_ativado = false;     // Ativando/Desativando recurso de corrente
 
  
  if (hcsr04_ativado) {
    
    digitalWrite(TriggerPin, LOW);                            // Coloca o pino Trigger em LOW
    delayMicroseconds(2);                                     // Mantem ativado por 2us
    digitalWrite(TriggerPin, HIGH);                           // Coloca o pino Trigger em HIGH
    delayMicroseconds(10);                                    // Mantem ativado por 10us
    digitalWrite(TriggerPin, LOW);                            // Desliga o Trigger para então medir o tempo de resposta das ondas sonoras
  
    tempoSom = pulseIn(EchoPin,HIGH);                         // Aguarda o pino echo ficar com status HIGH e retorna a duração em microsegundos
    float distancia = Distancia(tempoSom);                    // Calcula distancia do objeto -> hcsr04_som.h
    
    if (distancia > 0  && distancia < 0.2)   PiscarLampada(RelePin, 1, 1000);         // Se distancia entre 0 e 20 cm pisca 1x 
    if (distancia > 0.2 && distancia < 0.4)  PiscarLampada(RelePin, 2, 1000);         // Se distancia entre 20 e 40 cm pisca 2x
    if (distancia > 0.4 && distancia < 0.7)  PiscarLampada(RelePin, 3, 1000);         // Se distancia entre 40 e 70 cm pisca 3x
    if (distancia > 0.70) {}                                                          // Se distancia maior que 70 cm NÃO pisca
    
    Serial.print("Tempo(Som): ");
    Serial.print(tempoSom/2);
    Serial.println(" us");
  
    Serial.print("Distancia: ");  
    Serial.print(ConversaoUnidade(distancia, "m", "cm"));         // hcsr04_som.h -> melhorar funcao
    Serial.println(" cm");
    Serial.println();
    
  }
  
  if (lm35_ativado) {
    
    temperatura = Temperatura();                                    // acs712_corrente.h
    
    Serial.print(temperatura);
    Serial.println(" C");
    Serial.println();
    
    /* teste inicial da corrente
    corrente = Media(Corrente(), 10000);                           
    Serial.print(corrente, 1);
    Serial.println(" A");
    */
  }
  
  // praticamente pronto, mas ainda em teste e falta concluir, mas so posso fazer quando os 
  // cabos que faltam chegarem, pois não consigo ligar mais de um dispositivo simultaneo no arduino
  if (acs712_ativado == true) {
    
    data="";
    
    for (int i=0; i<6; i++) {
      
      Serial.println(i);                                            // verificaStatusDispositivo(parametro) -> funcao no final deste arquivo -> REALOCAR
      statusDispositivo_ant[i] = statusDispositivo[i];              // armazeno status dos dispositivos para poder compara e verificar se houve mudança do status
      statusDispositivo[i] = VerificaStatusDispositivo(i);          // nova leitura para armazenar estado atual dos dispositivos
      
      if ( statusDispositivo[i] != statusDispositivo_ant[i] ) {     // Se forem iguais é porque não houve mudança no estado, logo, não há o que atualizar na página para o usuário
  
        Serial.print("Status dispositivo antigo: ");    
        Serial.println(statusDispositivo_ant[i]);
        Serial.print("Status dispositivo atual: ");    
        Serial.println(statusDispositivo[i]);
        Serial.println("\n\n");
        
        //data += "lampada=";
        data += "#ar#=";                                             // #ar# é a chave para gravar dados no banco quando chegar os dados no servidor.
        data += String(statusDispositivo[i]);                        // forma de envio dos dados => #ar#= 'ambiente''dispositivo''local''status'
        
        // aqui tenho que criar os if para montar a sequencia de
        // dados da variavel 'data' de acordo com a porta analofica
        // pensando em fazer com estrututras mas ainda estou avaliando
        EnviaDados(webserver_ip, data, "POST");                       // ethernet_w5100.h
      
       }
    }
    
    String dadoRecebido = RecebeDados();                              // recebe dados da página web, como: deslitar/ligar um dispositivo ou qualquer configuração que eu queria enviar da página
  
    data="";
    
    if (dadoRecebido != "") {                                         // Se receber dados, grvo no banco de dados
      Serial.print("Dado recebido: ");
      Serial.println(dadoRecebido);
      Serial.println();
      data  = "#ar#=";
      data += String(dadoRecebido);
      EnviaDados(webserver_ip, dadoRecebido, "POST");                  // ethernet_w5100.h
    }
    
  }
  
  
  
  if (acs712_ativado == false) {
  
  // primeira funcao criada para apenas como teste.. é a versão da funcao acima porem para um unico dispositivo.
  // FUNCIONANDO
  statusLampada_ant = statusLampada;
  statusLampada = VerificaStatusDispositivo();                        // funcao no final deste arquivo -> apagar após sistema funcionando

  //est_dispositivo[0].status = VerificaStatusDispositivo();
  
  if ( statusLampada != statusLampada_ant ) {

    Serial.print("Status lampada antigo: ");    
    Serial.println(statusLampada_ant);
    Serial.print("Status lampada atual: ");    
    Serial.println(statusLampada);
    Serial.println();
    
    data = "lampada=";
    data = "bd=6";
    
    data += String(statusLampada);
    
    EnviaDados(webserver_ip, data, "POST");                           // ethernet_w5100.h
  
  }
  
  data = "";
  data = RecebeDados();
  Serial.print("data: ");
  Serial.println(data);

  if (data != "" && data != "-1") {
    
    Serial.println("entrou");
        
    if (data.charAt(1) == '1' || data.charAt(1) == '0') {
      
      if (data.charAt(0) == '1') 
        if (data.charAt(1) == '1') digitalWrite(1, HIGH); else digitalWrite(1, LOW);
        
      if (data.charAt(0) == '2')
        if (data.charAt(1) == '1') digitalWrite(2, HIGH); else digitalWrite(2, LOW);
        
      if (data.charAt(0) == '3')
        if (data.charAt(1) == '1') digitalWrite(3, HIGH); else digitalWrite(3, LOW);
        
      if (data.charAt(0) == '4')
        if (data.charAt(1) == '1') digitalWrite(4, HIGH); else digitalWrite(4, LOW);
        
      if (data.charAt(0) == '5')
        if (data.charAt(1) == '1') digitalWrite(5, HIGH); else digitalWrite(5, LOW);
        
      if (data.charAt(0) == '6') 
        if (data.charAt(1) == '1') digitalWrite(6, LOW); else digitalWrite(6, HIGH);
    
    }
  }
 
 }
  
  delay(seg*3);
  
} // FIM LOOP


// funcao para verificar status dos dispositivos
// envio o pino do dispositivo por parametro
int VerificaStatusDispositivo(int disp) {
 
  int statusDispositivo;
  
  corrente = Media( Corrente(disp), 10000 );        // comum.h
  Serial.println();
  Serial.print(corrente, 1);
  Serial.println(" A");
  
  if (corrente >= 0.3) {
    statusDispositivo = 1;
  } else {
    statusDispositivo = 0;
  }
   
  return statusDispositivo;
  
}

// funcao para verificar status dos dispositivos (correntePin)
int VerificaStatusDispositivo() {
  
  int statusDispositivo;
  
  corrente = Media(Corrente(), 10000);        // comum.h
  Serial.println();
  Serial.print(corrente, 1);
  Serial.println(" A");
  
  if (corrente >= 0.2) {
    statusDispositivo = 1;
  } else {
    statusDispositivo = 0;
  }
   
  return statusDispositivo;
}



