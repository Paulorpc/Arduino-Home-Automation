<?php

require_once("../login/seguranca.php");	
protegePagina(); 

require_once('conexao.php');
require_once('grava.php');

// Se a página atualizou através de interação do usuário, então faz um refresh rápido para atualizar status dos botões
if (isset($_POST['#ar#']))
	$tempo = "1";
else 
	$tempo = "5";


// Atualiza a página a cada 5 min.
$paginaAtual = $_SERVER['PHP_SELF'];
header("Refresh: $tempo; url=$paginaAtual");

?>


<!DOCTYPE html>
<html lang="pt-BR">
<head>
	<meta charset="UTF-8" />
	<meta http-equiv="X-UA-Compatible" content="IE=10" />
	<title>Automação Residencial | Projeto de Iniciação Científica &#8211; UniSEB COC</title>
	<link rel="profile" href="http://gmpg.org/xfn/11" />


    <link rel="stylesheet" href="../css/style.css" /> 
    <link rel='stylesheet' id='vantage-style-css'  href='../css/style_vantage.css?ver=1.1.7' type='text/css' media='all' /> 


	<meta name="generator" content="WordPress 3.8" />
        <!--[if lt IE 9]>
            <script src="../js/html5.js" type="text/javascript"></script>
        <![endif]-->
        <!--[if (gte IE 6)&(lte IE 8)]>
            <script type="text/javascript" src="../js/selectivizr.js"></script>
        <![endif]-->
	<meta name="viewport" content="width=device-width, initial-scale=1" />	
</head>


<body class="home blog siteorigin-panels siteorigin-panels-home responsive layout-full no-js has-menu-search">

<div id="page-wrapper">

	<header id="masthead" class="site-header" role="banner">

	<nav role="navigation" class="site-navigation main-navigation primary use-sticky-menu">
		<div class="full-container">
							
			<div class="menu-menu-1-container"><ul id="menu-menu-1" class="menu"><li id="menu-item-4" class="menu-item menu-item-type-custom menu-item-object-custom current-menu-item menu-item-home menu-item-4"><a href="../index.php"><span class="icon"></span>Principal</a></li>
<li id="menu-item-58" class="menu-item menu-item-type-custom menu-item-object-custom menu-item-has-children menu-item-58"><a href="#"><span class="icon"></span>Projeto</a>
<ul class="sub-menu">
	<li id="menu-item-32" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-32"><a href="#"></span>Plataforma Web</a></li>
	<li id="menu-item-33" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-33"><a href="#"></span>Arduino</a></li>
	<li id="menu-item-34" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-34"><a href="#"></span>Sistema Elétrico</a></li>
</ul>
</li>
<li id="menu-item-38" class="menu-item menu-item-type-post_type menu-item-object-page menu-item-38" style="float:right; margin-right:-58px"><a href="../login/logout.php" >Sair</a></li>
</ul></div>		</div>
	</nav><!-- .site-navigation .main-navigation -->

</header><!-- #masthead .site-header -->
	
	<div id="main" class="site-main" style="margin-top:-33px">
		<div class="full-container">
			
<div id="primary" class="content-area">
	<div id="content" class="site-content" role="main">
		<div class="entry-content">
			<div class="panel-grid" id="pg-home-0"><div class="panel-row-style panel-row-style-wide-grey"><div class="panel-grid-cell" id="pgc-home-0-0"><div class="panel widget widget_headline-widget panel-first-child panel-last-child" id="panel-0-0-0">		
            <h1>WEB Monitor</h1>
		<div class="decoration"><div class="decoration-inside"></div></div>
		<h3>Controle pela intranet e internet de seu desktop ou mobile.</h3>
		</div></div></div></div>
        
        <?php

// Instanciação de variaveis de comunicação
$arduino = new Conexao_Arduino;


// Atualiza condigurações dos botões		
function estilo($status) {
	if ($status == 0 ) { 
	
		$estilo = "ligar";		//estilo do botão (style.css)
		$texto = "LIGAR";
		
	} else if ( $status == 1 ) {
		
		$estilo = "desligar"; 	//estilo do botão (style.css)
		$texto = "DESLIGAR";
		
	} else {
		$estilo = "carregando2"; 
		$texto = "N/A";
	}
	
return array ($estilo, $texto);

}

// Monta a linha de comando do mysql e recuperar o(s) dado(s) da tabela.
$rs_selectAmbiente = $mysql->select_mysql("distinct(s.ambiente) as id, a.nome as nome", "ar_status2 s, ar_ambiente a", "a.id = s.ambiente ORDER BY a.nome");
	

// MONTAGEM DA TELA DE EXIBIÇAO DOS DADOS AO USUÁRIO
echo "<div align='center'>
		<form action='{$paginaAtual}' method='POST'>
		<table>
		<tr>
			<td><strong>Ambiente:</strong></td>
			<td>
				<select name='selectAmbiente' onChange='this.form.submit()' style='width: 200px; height: 25px;'>"; 

while ($row = mysql_fetch_assoc($rs_selectAmbiente)) {	
	
	//determina o valor do item no combo
	echo "<option value='{$row['id']}'"; 
	
	// da o item como selecionado se houver o valor na sessão
	if( $row['id'] == $_SESSION['selectAmbiente']) echo "selected"; 
	
	// Exibe o texto do item no combo
	echo ">".utf8_encode(ucwords($row['nome']))."</option>";
	
}



echo "			</select>
			</td>
		</tr>
	</table>
	</form>
	</div>";
	

// select da temperatura
$rs_temp = $mysql->select_mysql("s.status as status", "ar_status2 s, ar_dispositivos d", "s.dispositivo = 2 AND (d.id = s.dispositivo) AND s.ambiente = '{$_SESSION['selectAmbiente']}' ORDER BY s.id DESC LIMIT 1");

// verifica se a linhas retornadas e se houver monta resultado para exibição
if ( mysql_num_rows( $rs_temp ) > 0 ) {
	
	$rs_temp = mysql_fetch_assoc($rs_temp);
	$temperatura = "{$rs_temp['status']} °C";
	
} else 
	$temperatura = "N/A";

// exibe a temperatura
echo "<h4> Temperatura: {$temperatura}</h4>";


// Casso a sessão esteja limpa, pega o id do primeiro item do combo e carrega na sessão.
if ($_SESSION['selectAmbiente'] == NULL) {
	
	$rs_botoes = $mysql->select_mysql("distinct(s.ambiente) as ambiente, a.nome as nome", "ar_status2 s, ar_ambiente a", "s.ambiente=a.id ORDER BY a.nome ASC limit 1");
	$rs_botoes = mysql_fetch_assoc($rs_botoes);
	$_SESSION['selectAmbiente'] = $rs_botoes['ambiente'];
	//echo $_SESSION['selectAmbiente'];
	
}


/* 
SELECTS (c/ subselects) PARA MONTAGEM DA TABELA DOS BOTÕES

------------------------------------------------------------------------------------------------------

SELECT temp.id AS id, a.nome AS ambiente, d.nome AS dispositivo, l.nome AS local_nome, temp.local AS local_id, temp.status AS 
STATUS FROM (

SELECT a . * 
FROM ar_status2 AS a
INNER JOIN (

	SELECT dispositivo, local, MAX( id ) AS id
	FROM ar_status2
	WHERE ambiente = 3
	AND LOCAL <> 0
	GROUP BY dispositivo, local
	) AS b 
ON ( a.local = b.local AND a.id = b.id ) 
WHERE a.local <>0

) temp,  `ar_ambiente` a,  `ar_dispositivos` d,  `ar_local` l

WHERE ( temp.ambiente = a.id AND temp.dispositivo = d.id AND temp.local = l.id );

------------------------------------------------------------------------------------------------------

SQL1 = SELECT PARA AGRUPAR O ULTIMO REGISTRO (ID) DE CADA LOCAL (GROUP BY)
SQL2 = SELECT PARA PEGAR TODOS OS CAMPOS DA TABELA 'AR_STATUS2' APENAS COM AS LINHAS RESULTANTES DO RESULTADO DA TABELA SQL1
SELECT FINAL -> MONTADO POR {SELECT, FROM E WHERE} = TABELA FINAL FAZENDO O JOIN ENTRE AS OUTAS TABELAS PARA PEGAR OS NOMES DOS CAMPOS.

*/	

$sql1 = "SELECT dispositivo, local, MAX( id ) AS id FROM ar_status2	WHERE ambiente = {$_SESSION['selectAmbiente']} AND local <> 0 GROUP BY dispositivo, local";
$sql2 = "SELECT a.* FROM ar_status2 AS a INNER JOIN ( $sql1 ) AS b ON ( a.local = b.local AND a.id = b.id ) WHERE a.local <> 0";

$select = "temp.id AS id, temp.ambiente AS ambiente_id,a.nome AS ambiente_nome, temp.dispositivo AS dispositivo_id,d.nome AS dispositivo, temp.local AS local_id,l.nome AS local_nome, temp.status AS status";
$from   = "( $sql2 ) temp,  `ar_ambiente` a,  `ar_dispositivos` d,  `ar_local` l";
$where  = "( temp.ambiente = a.id AND temp.dispositivo = d.id AND temp.local = l.id )";

// Chamada de função select para Query acima ($select = sql1 + sql2)
$rs_botoes = $mysql->select_mysql($select, $from, $where);

// montagem dos botões da tela
while ($row = mysql_fetch_assoc($rs_botoes)) {
    
	extract($row);
	//echo "$ambiente - $dispositivo - $local - $status<BR/>";
	
	$estilo = estilo($status);	// vetor com estilo do botão e o texto do status
	$descricao = ucwords(utf8_encode("$dispositivo $local_nome"));
	
	echo "<div align='center'>
		  <form action='{$paginaAtual}' method='POST' >

		  <input type='hidden' name='ambiente' value='{$ambiente_id}' >
		  <input type='hidden' name='dispositivo' value='{$dispositivo_id}' >
		  <input type='hidden' name='local' value='{$local_id}' >
		  <input type='hidden' name='status' value='{$status}' >
	 	  <button type='submit' name='#ar#' class='{$estilo[0]}' title='{$descricao}'>$estilo[1]<div class='descricao' >$descricao</div></button><br/>

     	  </form>
		  </div>";
} 


?>
        
       <div class="panel-grid" id="pg-home-2"></div>		
		</div>
	</div><!-- #content .site-content -->
</div><!-- #primary .content-area -->

					</div><!-- .full-container -->
	</div><!-- #main .site-main -->

	
	
	<footer id="colophon" class="site-footer" role="contentinfo">

	<div id="footer-widgets" class="full-container">
			</div><!-- #footer-widgets -->

			<div id="site-info">
			Sistema Embarcado de Automação Residencial <br/> 2013 - Paulo Cezar - paulorpc@gmail.com</div><!-- .site-info -->
	
</footer><!-- #colophon .site-footer -->
	
</div><!-- #page-wrapper -->


<a href="#" id="scroll-to-top"></a>
</body>
</html>