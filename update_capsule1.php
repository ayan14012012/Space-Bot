<?php
$dataFile = "assets/data/capsule1.json";

$mq8 = isset($_GET['mq8']) ? $_GET['mq8'] : "0";
$mq135 = isset($_GET['mq135']) ? $_GET['mq135'] : "0";

$data = array(
  "mq8" => $mq8,
  "mq135" => $mq135,
  "timestamp" => date("Y-m-d H:i:s")
);

file_put_contents($dataFile, json_encode($data, JSON_PRETTY_PRINT));
echo "✅ Capsule 1 data updated";
?>
