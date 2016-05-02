<?php 

require_once('conexao.php');

$mysql   = new Conexao_db;
$arduino = new Conexao_Arduino;

/*********************************************************************************************

PARTE 1: CONTROLE DE GRAVAÇÃO DE DADOS NO MY SQL EM RESPOSTA A AÇÃO ENVIADA PELO ARDUINO.

**********************************************************************************************/

//   O isset verifica se existe dado na variavel
if ( isset( $_POST["temperatura"] ) )   {
	 
	$temperatura = $_POST["temperatura"];
	$mysql->insert_mysql( "ar_status", "nome, status", "'TEMP', ".$temperatura );
	
}

if ( isset( $_POST["lampada"] ) ) {
	
	$rs_lamp = $mysql->select_mysql("nome, status", "ar_status", "nome = 'LAMP' ORDER BY id DESC LIMIT 1");
	$rs_lamp = mysql_fetch_assoc($rs_lamp);
	
	if ( $rs_lamp['status'] == 0 ) $ligar = 1; else  $ligar = 0;
	$mysql->insert_mysql( "ar_status", "nome, status", "'LAMP', ".$ligar );
	
}
	
	
if (isset($_POST["#ar#"])) {
	
	foreach($_POST as $var => $value) {
		$$var = $value;
		//echo "$var = $value<br>";
	} 

	if ( $status == 0 ) $status = 1; else $status = 0;
	$valores =  "$dispositivo$local$status";
	//echo "valores: $valores<br>";
	
	$arduino->enviaDados($valores);

}
	

// GRAVA NO BANCO informação vindo do arduino
if (isset($_POST["bd"])) {
	
	$bd = $_POST['bd'];
	$regs = explode('|', $bd);
	
	$tabela = "ar_status2";
	$campos =  "ambiente, dispositivo, local, status";
	
	$valores =  "{$regs[0]}, {$regs[1]}, {$regs[2]}, {$regs[3]}";
	//$valores =  "15, {$bd[0]}, {$bd[1]}, {$bd[2]}";
	echo "valores: $valores<br>";
	
	$mysql->insert_mysql($tabela, $campos, $valores);

}


/* gravando no banco para teste
if (isset($_POST["enviaArduino"])) {
	
	foreach($_POST as $var => $value) {
		$$var = $value;
		//echo "$var = $value<br>";
	}

	$tabela = "ar_status2";
	$campos =  "ambiente, dispositivo, local, status";
	if ( $status == 0 ) $status = 1; else $status = 0;
	//echo "<br>$status<br>";
	
	$valores =  "'{$ambiente}', '{$dispositivo}',$local, $status";
	//echo "valores: $valores<br>";
	
	$mysql->insert_mysql($tabela, $campos, $valores);

}
*/



/**************************************************************************************

PARTE 2: CONTROLE DE ENVIO DE COMANDO DE CONTROLE PARA OS DISPOSITIVOS DO ARDUINO

/*************************************************************************************/


?>