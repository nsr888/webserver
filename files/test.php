<?php
if (!empty($_POST["key1"])) {
    echo "<html><body>";
    echo "<h2>Key 'key1' is set in POST request to value '".$_POST["key1"]."'</h2>";
    echo "POST array values:<br/><pre>";
    print_r($_POST);
} else {  
    echo "<html><body>";
    echo "<h2>Key 'key1' is NOT set in POST request</h2>";
    echo "POST array values:<br/><pre>";
    print_r($_POST);
}
?>
