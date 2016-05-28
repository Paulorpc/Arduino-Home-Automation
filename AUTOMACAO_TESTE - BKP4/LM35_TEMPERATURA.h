/**********************************************************************************************************************
* 
*  Função para ler temperatura em graus Celcius - SENSOR LM35
*
************************************************************************************************************************/
float Temperatura() {
  
  float sensibilidadeSensor = 0.010;                       // 10mV equivalem a 1C. Sensibilidade do sensor de acordo com datasheet. 
  
  long sensorValor = analogRead(TempPin);                  // Valor do sensor de temperatura em intervalos.
  float temp_mV = sensorValor * conversaoAD;               // Sensor trabalha com 5V, logo temos 5000mV dividido por 1023 passos.
  float temperatura = temp_mV / sensibilidadeSensor;       // Temperatura equivalente em mV na entrada analógica -> intervalos * (5/1023).
  
  return temperatura;                                      // Retorna temperatura em GRAUS CELCIUS.
}
