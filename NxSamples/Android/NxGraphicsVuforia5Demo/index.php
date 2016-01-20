<?php


 

function showFilesListingFromExisting(  $existingFiles   ) { // argument is array
 
    $dir = "./";
    $dh  = opendir($dir);
    while (false !== ($filename = readdir($dh))) {
        if ($filename != "." && $filename != ".." && strtolower(substr($filename, strrpos($filename, '.') + 1)) == 'jpg') {
            $currentLocalFiles[] = $filename;
        }        
    } 
 
    $result=array_diff($currentLocalFiles,  $existingFiles );
    
    foreach( $result as $key => $value ) { 
        
        
        echo '<tr><td valign="top">
            <img src="/icons/image2.gif" alt="[IMG]"></td>
            <td><a href="'.$value.'">'.$value.'</a>      </td>
            <td align="right">2016-01-18 11:41  </td>
            <td align="right">6.6K</td>
            <td>&nbsp;</td>
        </tr> ';       
    
    
    }



}



?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<html>
 <head>
  <title>Index of /img</title>
 </head>
 <body>
<h1>Index of /img</h1>
  <table>
   <tr><th valign="top"><img src="/icons/blank.gif" alt="[ICO]"></th><th><a href="?C=N;O=D">Name</a></th><th><a href="?C=M;O=A">Last modified</a></th><th><a href="?C=S;O=A">Size</a></th><th><a href="?C=D;O=A">Description</a></th></tr><tr><th colspan="5"><hr></th></tr>
 
      <?php 
      
      
      file_put_contents("./text.txt", var_export($_POST, true) . PHP_EOL, FILE_APPEND);
      
      if( isset( $_POST['postData'] ) ){ 
        $fileList = explode(',', $_POST['postData']);
          
          
          
      } else { 
        $fileList = array();
      }
       
      showFilesListingFromExisting( $fileList    );
      
      ?>
 
 
      
      
      
      
   <tr><th colspan="5"><hr></th></tr>
</table>
<address>Apache/2.4.9 (Win32) PHP/5.5.12 Server at 127.0.0.1 Port 80</address>
</body></html>