<?php
$dataFile = "assets/data/capsule2.json";

$temp = isset($_GET['temp']) ? $_GET['temp'] : "0";
$humidity = isset($_GET['humidity']) ? $_GET['humidity'] : "0";

$data = array(
  "temp" => $temp,
  "humidity" => $humidity,
  "timestamp" => date("Y-m-d H:i:s")
);

file_put_contents($dataFile, json_encode($data, JSON_PRETTY_PRINT));
echo "✅ Capsule 2 data updated";
?>
