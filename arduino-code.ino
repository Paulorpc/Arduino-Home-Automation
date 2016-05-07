
#include <SPI.h>
#include <Ethernet.h>
 
//Configurações de rede do Ethernet Shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };	// endereço mac atribuido ao arduino
byte ip[] = { 192,168,0, 10 }; 							// ip que o arduino assumirá
byte gateway[] = { 192,168,0, 1 }; 		 				// ip do roteador
byte subnet[] = { 255, 255, 255, 0 };

byte webserver[] = { 192,168,0,2 };						// ip do servidor web local
int porta = 80;											// porta para comunicacao (pode ser usada outras como 8080, 8081, etc.)

int seg = 1000;

EthernetServer server(porta);  // Cria o servidor na porta estabelecida
EthernetClient servidor;       // cliente para envio de dados

void setup() {
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

  Serial.begin(9600);  
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  Serial.print("IP do Arduino: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
}

 
void loop() {
  
  String data = "";
  
  // Recebe dados da página
  data = getData();
  
  // Recebeu algum comando da página
  if (data != "" && data != "-1") {
    
    Serial.println(data);
        
    if (data.charAt(2) == '1' || data.charAt(2) == '0') {
      
      if (data.charAt(1) == '5')
        if (data.charAt(2) == '1') digitalWrite(5, LOW); else digitalWrite(5, HIGH);
        
      if (data.charAt(1) == '6')
        if (data.charAt(2) == '1') digitalWrite(6, LOW); else digitalWrite(6, HIGH);
      
      // Grava no banco de dados  
      data = "bd="+data;
      sendData(data);
        
    }
  }
  delay(100);

}

String getData() {
  
  EthernetClient cliente = server.available();                                  
  String string = "-1";
  
  if ( cliente ) {
    
    string = cliente.readString();            // Função da biblioteca stream para receber string
    cliente.stop();                                  // Terminando conexão com webserver
    cliente.flush();                                 // Limpando buffer uma vez que já recebeu o stream
    
  }  
  return string;                                    //Retorna string
}


void sendData(String dados) {
  
  if ( servidor.connect(webserver, porta) ) {
  
//    servidor.println("POST /painelcontrole/arduino/teste1.php HTTP/1.1");
    servidor.println("POST /painelcontrole/arduino/modelando.php HTTP/1.1");    
    servidor.println("Host: 192.168.0.2");
    servidor.println("Content-Type: application/x-www-form-urlencoded");
    servidor.println("Connection: close");
    servidor.print("Content-Length: ");
    servidor.println( dados.length() );
    servidor.println();
    servidor.println( dados );
    
    Serial.println(dados);
    Serial.println("dados enviados");
    
  
  } else 
      Serial.println("Não conectou ao webserver"); 
  
  // Fecha a conexão para poder se conectar novamente.
  servidor.stop();
  
}
