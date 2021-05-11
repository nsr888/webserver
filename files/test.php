<?php
if (!empty($_POST["key1"])) {
    echo "<html><body><pre>\n";
    echo "Key 'key1' is set in POST request to value '".$_POST["key1"]."'\n";
    echo "POST array values:\n";
    print_r($_POST);
} else {  
    echo "<html><body><pre>\n";
    echo "Key 'key1' is NOT set in POST request\n";
    echo "POST array values:\n";
    print_r($_POST);
}
?>
