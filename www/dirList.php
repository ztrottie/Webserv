<?php
	$dir = $_SERVER["REQUEST_URI"];

	if ($_SERVER["REQUEST_METHOD"] == 'GET'){
		$file = scandir($dir);
		foreach ($file as $file){
			$filePath = $dir . '/' . $file;
			if (is_file($filePath))
				echo $file . "<br>";
		}
	}
?>