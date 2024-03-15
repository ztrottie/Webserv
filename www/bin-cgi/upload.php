<?php
    if ($_SERVER["REQUEST_METHOD"] == "POST"){
        // Parse the raw POST data into key-value pairs
        $name = $_POST["name"];
        $nickName = $_POST["nickName"];
        $email = $_POST["email"];
        $db = "dataBase.txt";

        $data = "Name: $name nickName: $nickName email: $email";

        file_put_contents($db, $data, FILE_APPEND | LOCK_EX);

    }
?>