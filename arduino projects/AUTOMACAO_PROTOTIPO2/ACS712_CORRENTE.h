/**********************************************************************************************************************
 
*  Função para encontrar a corrente do dispositivo - SENSOR ACS712.

*  @param pin - Numeração do pino analógico a checar a corrente
*  @param ampSensor - amperagem máx. do sensor utilizado.

************************************************************************************************************************/
float Corrente(int pino, int ampSensor) {

  float sensibilidadeSensor;
  
  if (ampSensor == 5)  sensibilidadeSensor = 0.185;                      // Sensor de  5A - 185mV por 1A. Sensibilidade do sensor de acordo com datasheet. 
  if (ampSensor == 20) sensibilidadeSensor = 0.100;                      // Sensor de 20A - 100mV por 1A. Sensibilidade do sensor de acordo com datasheet.                                
  if (ampSensor == 30) sensibilidadeSensor = 0.660;                      // Sensor de 30A - 100mV por 1A. Sensibilidade do sensor de acordo com datasheet.                                
  
  int n = 100;
  float sensorValor_aux = 0;
  float sensorValor = 0;

  for(int i=0; i<n; i++){
    sensorValor_aux = (analogRead(pino) - 512);                      // le o valor do sensor e subtrai 1024/2 que equivale a 512 unidades do arduino = 0 Amp.
    sensorValor += abs(sensorValor_aux);                             // somatória dos valores absolutos (RMS).
  }
  
  sensorValor = (sensorValor / n) * conversaoAD;                    // finaliza o calculo da médida converte o valor para volts.
  float corrente = (sensorValor / sensibilidadeSensor);               // calcula a corrente considerando a sensibilidade do sernsor.

  //if (corrente < 0.04) corrente = 0.0;                            // POR ALGUMA RAZÃO, QUANDO CONECTA O MODULO DE RELE EM QUALQUER PINO DIGITAL QUE ESTEJA COMO OUTPUT,
                                                                    // A PRECISÃO DA CORRENTE DIMINUI. COM O CABO DESCONECTADO, PODEMOS FAZER A CONDIÇÃO DA CORRENTE < 0.04 
                                                                    // COM O CABO CONECTADO A CORRENTE = 0 PASSA PARA 0.16 (arredondei para 0.2), UM AUMENTO SIGNIFICATIVO.
                                                                    
  if (corrente < 0.3) corrente = 0.0;                                                                    
  
  return corrente;                                                  // Retorna corrente do dispositivo.
}


/**********************************************************************************************************************
 
*  Função para encontrar a corrente Média do dispositivo - SENSOR ACS712.

*  @param pino - Numeração do pino analógico a checar a corrente
*  @param ampSensor - amperagem máx. do sensor utilizado.
*  @param qtde - qtde de vezes que deverá ser somado os valores para determinar a media

************************************************************************************************************************/
float CorrenteMedia(int pino, int ampSensor, int qtde) {
  return Media(Corrente(pino, ampSensor), qtde);                                                  // Retorna corrente média do dispositivo.
}



/**********************************************************************************************************************
 
*  Função para verificar se o dispositivo esta ligado ou não

*  @param disp - Numeração do pino analógico a checar a corrente
*  @param nMedia - qtde de medicoes para realizar media
*  @param ampSensor - amperagem máx. do sensor de corrente utilizado neste no dispositivo.

************************************************************************************************************************/
int StatusDispositivo(int pino, int ampSensor) {
 
  int statusDispositivo = 0;
  
  float corrente = Corrente(pino, ampSensor);
  if (corrente > 0.0) statusDispositivo = 1;

  return statusDispositivo;
  
}
