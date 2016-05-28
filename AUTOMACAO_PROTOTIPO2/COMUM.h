
/**********************************************************************************************************************
 
*  Função para encontrar um valor médio qualquer.

*  @param valor - valor de entrada para encontrar a média
*  @param qtde - Qtde de repetições para definir média

************************************************************************************************************************/
float Media(float valor, int qtde) {
  
float soma = 0.0;
  
  for (int i=0; i<qtde; i++) {
    soma += valor;
  }  
  return soma/qtde;
}

/**********************************************************************************************************************
 
*  Função para piscar a lampada.

*  @param qtde - Qtde de piscadas
*  @param tempoDelay - Tempo de Delay entre as picadas

************************************************************************************************************************/
void PiscarLampada(int LedPin, int qtde, long tempoDelay) {
  
  for (int i=0; i<qtde; i++) {
    
    digitalWrite(LedPin, HIGH);
    delay(tempoDelay);
    digitalWrite(LedPin, LOW);
    delay(tempoDelay);
    
  }  
}


/**********************************************************************************************************************
 
*  Função para imprimir os ... simulando que está carregando algo.

************************************************************************************************************************/
void Carregando(int tempo) {
  
  int qtde = 10;
  
  for (int i=0; i<qtde; i++) {
    Serial.print(".");
    delay(tempo/qtde);
  }
  Serial.println();
  
}
