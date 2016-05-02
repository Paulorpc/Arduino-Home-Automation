<?php 

// Inclui o arquivo com o sistema de segurança
require_once("seguranca.php"); 

?>

<!DOCTYPE html>
	<!--[if IE 8]>
		<html xmlns="http://www.w3.org/1999/xhtml" class="ie8" lang="pt-BR">
	<![endif]-->
	<!--[if !(IE 8) ]><!-->
		<html xmlns="http://www.w3.org/1999/xhtml" lang="pt-BR">
	<!--<![endif]-->
	
    <head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<title>Automação Residencial &rsaquo; Login</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />	
    
	<link rel='stylesheet' id='open-sans-css' href='//fonts.googleapis.com/css?family=Open+Sans%3A300italic%2C400italic%2C600italic%2C300%2C400%2C600&#038;subset=latin%2Clatin-ext&#038;ver=3.8' type='text/css' media='all' />
    

<link rel='stylesheet' id='style'   	 	  href='../css/style.css' type='text/css' media='all' /> 
<link rel='stylesheet' id='buttons-css'    	  href='../css/buttons.min.css?ver=3.8' type='text/css' media='all' /> 
<link rel='stylesheet' id='colors-fresh-css'  href='../css/colors.min.css?ver=3.8' type='text/css' media='all' /> 
<link rel='stylesheet' id='wp-admin-css'      href='../css/wp-admin_cpanel.min.css?ver=3.8' type='text/css' media='all' />


<!--[if lte IE 7]>
<link rel='stylesheet' id='ie-css'  href='../css/ie.min.css?ver=3.8' type='text/css' media='all' />
<![endif]-->
    
	</head>
	<body class="login login-action-login wp-core-ui">
    <div id="login">
    <h1><a href="../index.php" title="por Paulo Cezar">Automação Residencial</a></h1>
    
<?php

// Verifica se um formulário foi enviado
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
	
	$usuario = (isset($_POST['usuario'])) ? $_POST['usuario'] : '';
	$senha   = (isset($_POST['senha'])) ? $_POST['senha'] : '';

	// Autenticação do usuário
	if (validaUsuario($usuario, $senha))
		//header("Location: ../pagina/index2.php");
		header("Location: ../pagina/index.php"); //em teste
	else{
	 	echo "<div id='login_error' style='margin-top: -30px; position: relative;  '><strong>ERRO:</strong> Usuário ou senha inválidos</div>";
	}
}

?>

	
<form name="loginform" id="loginform" action="admin.php" method="post">
	<p>
		<label for="user_login">Nome de usuário<br />
		<input type="text" name="usuario" id="usuario" class="input" value="" size="20" /></label>
	</p>
	<p>
		<label for="user_pass">Senha<br />
		<input type="password" name="senha" id="senha" class="input" size="20" /></label>
	</p>
		<p class="forgetmenot">
		  <input type="submit" class="button button-primary button-large" value="Entrar" />

	</p>
</form>
<br/></p>
	
		</div>
   	<div class="clear"></div>
	</body>
	</html>
	