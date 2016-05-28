  
/**********************************************************************************************************************
 
*  Função para enviar as informações para o webserver por 'POST' ou 'GET'.

*  @param servidor - nome do servidor conectado
*  @param dados - dados que deverão ser enviados ao webserver
*  @param metodoEnvio - Método de envio dos dados (POST ou GET)

************************************************************************************************************************/
void EnviaDados(char servidor[], String dados, String metodoEnvio) {
  
  if (metodoEnvio == "POST" || metodoEnvio == "GET") {                        // Verifica se o metodo de envio passado é POST ou GET
  
    if ( cliente.connect(servidor, porta) ) {
      
      Serial.print("\nConectado a ");
      Serial.println(servidor);
  
      cliente.print(metodoEnvio);
      cliente.println(" /painelcontrole/arduino/grava.php HTTP/1.1");      
      cliente.println("Host: 192.168.0.2");
      cliente.println("Content-Type: application/x-www-form-urlencoded");
      cliente.println("Connection: close");
      cliente.print("Content-Length: ");
      cliente.println( dados.length() );
      cliente.println();
      cliente.println( dados );
      
      Serial.print("Dados enviados: ");
      Serial.println( dados );
      
    } else
        Serial.println("\nConxao com o WebServer falhou. Dados nao enviados.");
    
  } else
      Serial.println("\nO Método de envio deve ser POST ou GET");
  
  cliente.flush();
  cliente.stop();
   
}


/**********************************************************************************************************************
 
*  Função para receber as informações do webserver

************************************************************************************************************************/
String RecebeDados() {

  EthernetClient servidor = server.available();
  String string = "-1";
  
  if ( servidor ) {
    
    string = servidor.readString();                   // Função da biblioteca stream para receber string
    Serial.println( "Dados recebidos." );
    
  }
  servidor.flush();
  servidor.stop();
  
  return string;                                     //Retorna string
}

