<?php
    $uploadDir = "webFile/";

    if (mkdir($uploadDir)){
        echo "Directory created!";
    }
    else{
        echo "Error while creating the directory" . $uploadDir;
    }

    
?>

<?php
// $targetDir = "uploads/"; // Directory where you want to store uploaded files
// $targetFile = $targetDir . basename($_FILES["fileToUpload"]["name"]);
// $uploadOk = 1;
// $imageFileType = strtolower(pathinfo($targetFile,PATHINFO_EXTENSION));

// if (file_exists($targetFile)) {
//     echo "Sorry, file already exists.";
//     $uploadOk = 0;
// }

// if ($uploadOk == 0) {
//     echo "Sorry, your file was not uploaded.";
// // if everything is ok, try to upload file
// } else {
//     if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $targetFile)) {
//         echo "The file ". basename( $_FILES["fileToUpload"]["name"]). " has been uploaded.";
//     } else {
//         echo "Sorry, there was an error uploading your file.";
//     }
//}
?>


<!-- <?php
//if ($_SERVER["REQUEST_METHOD"] == "POST") {
//    if (!isset($_SERVER['CONTENT_LENGTH'])) {
//        echo "Error: Content-Length header is missing.";
//        exit;
//    }
//    $expectedContentLength = (int)$_SERVER['CONTENT_LENGTH'];
//    $actualContentLength = strlen(file_get_contents("php://input"));
//    if ($expectedContentLength === $actualContentLength) {
//        if ($_FILES["file"]["error"] == UPLOAD_ERR_OK) {
//            if (is_uploaded_file($_FILES["file"]["tmp_name"])) {
//                $uploadedFilePath = $_FILES["file"]["tmp_name"];
//                echo "File uploaded successfully: $uploadedFilePath";
//            } else {
//                echo "Error: File upload not completed.";
//            }
//        } else {
//            echo "Error during file upload: " . $_FILES["file"]["error"];
//        }
//    } else {
//        echo "Error: Content-Length mismatch.";
//    }
//}
//?> -->
