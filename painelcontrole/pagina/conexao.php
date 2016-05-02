<?php   


/**************************************************************************************

							CLASSE DE CONEXÃO COM O ARDUINO

/*************************************************************************************/

// Cria a conexão socket TCP (habilitar extensão PHP_SOCKETS)
$con = socket_create(AF_INET, SOCK_STREAM, SOL_TCP); 

class Conexao_Arduino {
	
	// Função de conexão via Socket ao arduino
	private function conecta() {
		
		global $con;
			
		$ip_arduino = "192.168.0.10";
		$porta = "80";
		
		// Se conecta ao IP e Porta do Arduino:
		socket_connect($con, $ip_arduino, $porta);
	
	}
	
	// Função que envia instruções para o Arduino
	function enviaDados( $dados ) {
		
		global $con;
		
		$this->conecta();
		socket_write( $con, $dados, strlen($dados) );	
		socket_close($con);
		
	}
	
	// Função que recebe atualização do status dos dispositivos conecatados ao Arduino
	function recebeDados() { 
		
		global $con;
		
		$this->conecta();
		$status = socket_read( $con, $dados );
		socket_close($con);
		
		return $status;
		
	 }	
}
 
 

/**************************************************************************************

				CLASSE DE CONEXÃO COM O BANCO DE DADOS E SUAS FUNÇÕES

/*************************************************************************************/


class Conexao_db {

	//dados de conexao com o banco
	private $host = "localhost";
	private $usuario = "root";
	private $senha = "";
	private $db = "arduino"; 
	
	
	// Função de conexão com o mysql na tabela arduino (padrao).
	private function conecta_mysql() {
		
		// Faz conexão com o banco e checa a conexão.
		$con = mysql_connect($this->host, $this->usuario, $this->senha);
		if (!$con)
			die("<table width='100%'><td align='center'><div class='error'>Erro ao conectar ao servidor: \"" .mysql_error()."\"</div></td></table>");
	
		// Selecionar o banco de dados e checa a conexão.
		$db_selecionado = mysql_select_db($this->db, $con);
		if (!$db_selecionado)
			die ("<table width='100%'><td align='center'><div class='error'>Erro ao conectar a base de dados: \"" .mysql_error()."\"</div></td></table>");
		
		return $con;
		
	}
	
	
	// Função para fazer o insert na tabela.
	public function insert_mysql( $tabela, $campos, $valores ) {
			
		$linha_sql = "INSERT INTO ".$tabela." (".$campos.") VALUES (".$valores.")";
		//echo "sql: $linha_sql";
		
		$con = $this->conecta_mysql();	
		mysql_query( $linha_sql );
		mysql_close( $con ); 
		
	}
	
	
	// Função para dar um select na tabela.
	public function select_mysql( $campos, $tabela, $where ) {
		
		$linha_sql = "SELECT ".$campos." FROM ".$tabela." WHERE ".$where;
		//echo "sql: $linha_sql<br/>";
		
		$con = $this->conecta_mysql();
		$dado = mysql_query( $linha_sql );
		
			
		if ( !$dado )
			echo "<table width='100%'><td align='center'><div class='error'>Erro ao executar a consulta (".$linha_sql.") no banco de dados: " . mysql_error()."</div></td></table>";
		
		//if ( mysql_num_rows( $dado ) == 0 )
		//	echo "<table width='100%'><td align='center'><div class='error'>Nao há resultado para exibição</div></td></table>";
		
		mysql_close( $con );
		return $dado;
		
	}
	
	
	// Função para fazer update na tabela
	public function update_mysql( $tabela, $campo, $valor, $where ) {
		
		$linha_sql = "UPDATE ".$tabela." SET ".$campo." = ".$valor." WHERE ".$where;
		
		$con = $this->conecta_mysql();
		$dado = mysql_query( $linha_sql );
		mysql_close( $con );
		
		//Obs: melhorar método para maior segurança de parametros enviados.
		
	}
	
	// Função para deletar dados na tabela
	public function delete_mysql( $tabela, $where ) {
		
		$linha_sql = "DELETE FROM ".$tabela." WHERE ".$where;
		
		$con = $this->conecta_mysql();
		mysql_query( $linha_sql );
		mysql_close( $con );
		
		//Obs: melhorar método para maior segurança de parametros enviados.
	
	 }
 
}
?>