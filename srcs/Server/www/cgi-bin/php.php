<?php
// not working
echo "Content-type:text/html\r\n\r\n";
echo "<!DOCTYPE html>";


echo "<body>";
echo "<link rel=\"icon\" href=\"data:,\"> ";
echo "<center>";
echo "<h1>Test Post with php</h1>";
echo "<center>";
if(isset($_POST['name']))
    echo $_POST['name'];
else
    echo "<h1>no name given.</h1>";

echo "    Bonjour, ";
echo $_POST['name'];

echo "</body>";
echo "</html>";
?>