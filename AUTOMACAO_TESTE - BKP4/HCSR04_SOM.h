/*******************************************************************************************************************
*
* Calculo da distancia (x = vt) -> Distancia = (velocidade do som)*(tempo)
* Distacia / 2 ().
* Distancia foi medida em metros, sendo assim, devemos dividir por 10^(-6), pois o tempo está em microsegundos (us).
* 
********************************************************************************************************************/
float Distancia(long tempo) {
  
  float velocidade = 343.3;                          // velocidade do som - 343.3 m/s  (média, pois varia com temperatura e condições do ar)
  float distancia = ((velocidade*tempo)/2);          // devemos dividir por 2, pois há o tempo de ida e retorno do som 
  distancia = distancia/1000000;

  return distancia;
  
}


/**********************************************************************************************************************
* 
*  Função para conversão de unidades de medida.
*  @param distancia - distancia a converter
*  @param de - unidade de medida original (m,cm e mm)
*  @param para - unidade de medida para conversão (m, cm e mm)
*
************************************************************************************************************************/
float ConversaoUnidade(float distancia, char de[], char para[]) {
  
  if ( (de != "mm" || de!= "cm" || de != "m") && (para != "m" || para != "cm" || para != "mm") ) 
    return -1;  
  else {
    if (de == "m" && para == "mm") distancia *= 1000;        //conversão para milimetros
    if (de == "m" && para == "cm") distancia *= 100;         //conversão para centimetros
    if (de == "cm" && para == "m") distancia /= 100;         //conversão para centimetros
    if (de == "cm" && para == "mm") distancia *= 10;         //conversão para centimetros
    if (de == "mm" && para == "m") distancia /= 1000;        //conversão para centimetros
    if (de == "mm" && para == "cm") distancia /= 10;         //conversão para centimetros
    
    return distancia;                                        // Retorna valor convertido
  }
}
