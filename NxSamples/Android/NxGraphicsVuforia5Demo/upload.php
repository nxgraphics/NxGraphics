<?php

$uploaddir = './img/';
$uploadfile = $uploaddir . basename($_FILES['uploadedfile']['name']);
if (move_uploaded_file($_FILES['uploadedfile']['tmp_name'], $uploadfile)) {
    echo "upload success\n";
} else {
    echo "upload failed\n";
}
 
?>