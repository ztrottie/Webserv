<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $targetDir = "uploads/";

    if (!file_exists($targetDir)) {
        mkdir($targetDir, 0777, true);
    }

    $targetFilePath = $targetDir . basename($_FILES["file"]["name"]);

    if (is_uploaded_file($_FILES["file"]["tmp_name"])) {
        if (move_uploaded_file($_FILES["file"]["tmp_name"], $targetFilePath)) {
            echo "File " . htmlspecialchars(basename($_FILES["file"]["name"])) . " has been uploaded successfully.";
        } else {
            echo "Error uploading file.";
        }
    } else {
        echo "Invalid file.";
    }
} else {
    echo "Invalid request";
}
?>
