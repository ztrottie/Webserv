<?php
$target_dir = "uploads/"; // Directory where you want to store uploaded files
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]); // Path to the uploaded file
$uploadOk = 1; // Flag to track if the upload was successful

echo $target_file;
// Check if file already exists
if (file_exists($target_file)) {
    echo "Sorry, file already exists.";
    $uploadOk = 0;
}

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

// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
    echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
} else {
    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $targetFile)) {
        echo "The file ". basename( $_FILES["fileToUpload"]["name"]). " has been uploaded.";
    } else {
        echo "Sorry, there was an error uploading your file.";
    }
}
?>
