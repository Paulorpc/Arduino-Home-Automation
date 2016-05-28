
/**********************************************************************************************************************
* 
*  Função para desconectar do webserver.
*
************************************************************************************************************************/
void DesconectarCliente(){

    cliente.flush();                                   //limpa memória e para o cliente
    delay(5);
    cliente.stop();
    Serial.println("Cliente desconectado!\n");

}

/*
void sendGET() //client function to send and receive GET data from external server.
{
  if (client.connect(serverName, 80)) {
    Serial.println("connected");
    client.println("GET /painelcontrole/arduino.txt HTTP/1.0");
    client.println();
  } 
  else {
    Serial.println("connection failed");
    Serial.println();
  }

  while(client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read();
    Serial.print(c);
  }

  Serial.println();
  Serial.println("disconnecting.");
  Serial.println("==================");
  Serial.println();
  client.stop();

}
*/  
  
/**********************************************************************************************************************
* 
*  Função para enviar as informações para o webserver por ''POST ou GET''.
*  @param servidor - nome do servidor conectado
*  @param dados - dados que deverão ser enviados ao webserver
*  @param metodoEnvio - Método de envio dos dados (POST ou GET)
*
************************************************************************************************************************/
void EnviaDados(char servidor[], String dados, String metodoEnvio) {
  
  if (metodoEnvio == "POST" || metodoEnvio == "GET") {                        // Verifica se o metodo de envio passado é POST ou GET
  
    if ( cliente.connect(servidor, porta) ) {
      
      Serial.print("\nConectado a ");
      Serial.println(servidor);
      Serial.print("enviando dados...");
  
      cliente.print(metodoEnvio);
      cliente.println(" /painelcontrole/arduino/grava.php HTTP/1.1");      
      cliente.println("Host: 192.168.0.2");
      cliente.println("Content-Type: application/x-www-form-urlencoded");
      cliente.println("Connection: close");
      cliente.print("Content-Length: ");
      cliente.println( dados.length() );
      cliente.println();
      cliente.println( dados );
      
      Serial.println(" Enviado");
      Serial.println( dados );
     
      DesconectarCliente();                                                      // Desconecta o cliente
      
    } else {
        Serial.println("\nConxao com o WebServer falhou. Dados nao enviados.");
    }
    
  } else { 
      Serial.println("\nO Método de envio deve ser POST ou GET");
    }
}


/**********************************************************************************************************************
* 
*  Função para receber as informações do webserver
*
************************************************************************************************************************/
String RecebeDados() {

  server.begin();
  delay(5);
  
  EthernetClient cliente = server.available();
  String string = "-1";
  
  if ( cliente ) {
    
    Serial.print("recebendo dados... ");
    string = cliente.readString();                   // Função da biblioteca stream para receber string
    Serial.print("recebido.\n");
    DesconectarCliente();
    
  }
  
  return string;                                     //Retorna string
}

