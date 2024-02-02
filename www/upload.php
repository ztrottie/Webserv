<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Check if the Content-Length header is provided
    if (!isset($_SERVER['CONTENT_LENGTH'])) {
        echo "Error: Content-Length header is missing.";
        exit;
    }

    // Get the expected content length from the Content-Length header
    $expectedContentLength = (int)$_SERVER['CONTENT_LENGTH'];

    // Get the actual content length of the received data
    $actualContentLength = strlen(file_get_contents("php://input"));

    // Check if the content length matches
    if ($expectedContentLength === $actualContentLength) {
        // Content-Length is correct, proceed with file upload processing

        // Check if file upload was successful
        if ($_FILES["file"]["error"] == UPLOAD_ERR_OK) {
            // Check if the file has been fully uploaded
            if (is_uploaded_file($_FILES["file"]["tmp_name"])) {
                // Process the file as needed
                $uploadedFilePath = $_FILES["file"]["tmp_name"];
                echo "File uploaded successfully: $uploadedFilePath";
            } else {
                echo "Error: File upload not completed.";
            }
        } else {
            echo "Error during file upload: " . $_FILES["file"]["error"];
        }
    } else {
        // Content-Length does not match, handle the error
        echo "Error: Content-Length mismatch.";
    }
}
?>
