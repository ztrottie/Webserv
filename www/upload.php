<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (!isset($_SERVER['CONTENT_LENGTH'])) {
        echo "Error: Content-Length header is missing.";
        exit;
    }
    $expectedContentLength = (int)$_SERVER['CONTENT_LENGTH'];
    $actualContentLength = strlen(file_get_contents("php://input"));
    if ($expectedContentLength === $actualContentLength) {
        if ($_FILES["file"]["error"] == UPLOAD_ERR_OK) {
            if (is_uploaded_file($_FILES["file"]["tmp_name"])) {
                $uploadedFilePath = $_FILES["file"]["tmp_name"];
                echo "File uploaded successfully: $uploadedFilePath";
            } else {
                echo "Error: File upload not completed.";
            }
        } else {
            echo "Error during file upload: " . $_FILES["file"]["error"];
        }
    } else {
        echo "Error: Content-Length mismatch.";
    }
}
?>
