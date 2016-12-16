<?php
include("config.php");
header("Content-Type: application/json");

$headers = array_merge(apache_request_headers(), $_SERVER);
$authKey = null;

if(isset($headers['HTTP_AUTHORIZATION'])) {
	$authKey = $headers['HTTP_AUTHORIZATION'];
} elseif( isset($headers['Authorization'])) {
	$authKey = $headers['Authorization'];
}

if($authKey == null || !$authKey === $AUTH_KEY) {
	echo json_encode(array(null));
	return -2;
}

if(! isset($_POST['image'])) {
	echo json_encode(array(null));
	return -3;
}

$imageData = $_POST['image'];
$imageName = md5($imageData) . ".png";

file_put_contents($PICDIR . "/" . $imageName, $imageData);

echo json_encode(array("data" => array("link" => str_replace("[[PICID]]", $imageName, $URLFORMAT))));

?>
