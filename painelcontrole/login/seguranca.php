<?php 

require_once("../pagina/conexao.php");

$_SG['conectaServidor'] = true;    			// Abre uma conexão com o servidor MySQL
$_SG['abreSessao']      = true;         	// Inicia a sessão com um session_start()
$_SG['caseSensitive']   = true;     		// Usar case-sensitive (apenas para senha)
$_SG['paginaLogin'] 	= 'admin.php'; 		// Página de login


// Verifica se precisa iniciar a sessão
if ($_SG['abreSessao'] == true) session_start();

// cria variaveis de sessão caso não foram criadas
if (!isset($_SESSION['selectAmbiente'])) $_SESSION['selectAmbiente'] = NULL;

// atribui valores as variaveis de sessão caso foram enviadas por POST
if (isset($_POST['selectAmbiente'])) $_SESSION['selectAmbiente'] = $_POST['selectAmbiente']; 


/**
* Função que valida um usuário e senha
*
* @param string $usuario - O usuário a ser validado
* @param string $senha - A senha a ser validada
*
* @return bool - Se o usuário foi validado ou não (true/false)
*/
function validaUsuario($usuario, $senha) {

	global $_SG;
	
	$cs = ($_SG['caseSensitive']) ? 'BINARY' : '';
	
	// Usa a função addslashes para adicionar escape
	$usuario = addslashes($usuario);
	$senha   = addslashes($senha);
	
	$mysql = new Conexao_db();
	$where = " usuario = '{$usuario}' AND {$cs} senha = '{$senha}' LIMIT 1";
	
	// Monta uma consulta SQL (query) para procurar um usuário
	$query = $mysql->select_mysql("id, usuario, senha", "usuarios", $where);
	$resultado = mysql_fetch_assoc($query);
	
	// Se não encontrou registro retorna falso
	if (empty($resultado)) {
		return false;  
	} else {
		// Foi encontrado o usuário, grava ne sessão e valida login.
		$_SESSION['usuarioID']      = $resultado['id']; 		 // Salva na sessao o ID do usuário
		$_SESSION['usuarioLogin']   = $resultado['usuario'];     // Salva na sessao o Login do usuário
		
		return true;
	}
}


function protegePagina() {
	
	// Se não há registro de log na sessão expulsa usuário
	if (!isset($_SESSION['usuarioID']) || !isset($_SESSION['usuarioLogin']))
		expulsaVisitante();
		
}

function expulsaVisitante() {
	
	// Remove as variáveis da sessão (caso elas existam) e redireciona para index
	unset($_SESSION['usuarioID'], $_SESSION['usuarioLogin']);	
	header("Location: ../index.php");
	
}

?>