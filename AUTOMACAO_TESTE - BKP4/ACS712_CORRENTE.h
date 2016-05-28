
/**********************************************************************************************************************
* 
*  trabalhando
*
************************************************************************************************************************/
float Corrente(int CorrentePino) {
  
  float sensibilidadeSensor = 0.185;                                   // 185mV por 1A. Sensibilidade do sensor de acordo com datasheet.
      
  int sensorValor_aux = 0;
  float sensorValor = 0;
  int sensorValorMedio = Media_sensorValor(CorrentePino, 100);        //Valor medio da entrada quando não há corrente é o valor médio da voltagem do pino (2,5V).
    
  for(int i=0; i<100; i++){
    sensorValor_aux = (analogRead(CorrentePino) - sensorValorMedio); // le o sensor na pino analogico e ajusta o valor lido ja que a saída do sensor é (1023)vcc/2 para corrente = 0.
    sensorValor += abs(sensorValor_aux);                             // somatória dos valores absolutos.
  }
    
  sensorValor = (sensorValor / 100) * conversaoAD;                   // finaliza o calculo da médida converte o valor para volts.
  float corrente = (sensorValor /sensibilidadeSensor);               // calcula a corrente considerando a sensibilidade do sernsor.
  
  return corrente;                                                   // Retorna corrente do dispositivo.
  
}


/**********************************************************************************************************************
* 
*  Função para encontrar a corrente do dispositivo - SENSOR ACS712-05.
*
************************************************************************************************************************/
float Corrente() {
  
float sensibilidadeSensor = 0.185;                                   // 185mV por 1A. Sensibilidade do sensor de acordo com datasheet.
    
int sensorValor_aux = 0;
float sensorValor = 0;
int sensorValorMedio = Media_sensorValor(CorrentePin, 100);         //Valor medio da entrada quando não há corrente é o valor médio da voltagem do pino (2,5V).
  
  for(int i=0; i<100; i++){
    sensorValor_aux = (analogRead(CorrentePin) - sensorValorMedio);  // le o sensor na pino analogico A0 e ajusta o valor lido ja que a saída do sensor é (1023)vcc/2 para corrente = 0.
    sensorValor += abs(sensorValor_aux);                             // somatória dos valores absolutos.
  }
  
  sensorValor = (sensorValor / 100) * conversaoAD;                   // finaliza o calculo da médida converte o valor para volts.
  float corrente = (sensorValor /sensibilidadeSensor);               // calcula a corrente considerando a sensibilidade do sernsor.

  return corrente;                                                   // Retorna corrente do dispositivo.
}
