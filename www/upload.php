<?php
    if ($_SERVER["REQUEST_METHOD"] == "POST"){

        $name = $_POST["name"];
        $nickName = $_POST["nickName"];

        $db = "dataBase.txt";

        $data = "Name: $name nickName: $nickName";

        file_put_contents($db, $data, FILE_APPEND | LOCK_EX);
    }
?>
