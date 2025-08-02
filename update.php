<?php
header('Content-Type: application/json');

$file = 'data.json';

// Load existing data if available
$data = file_exists($file) ? json_decode(file_get_contents($file), true) : [];

// 🧪 Capsule 1 Gas Sensors
if (isset($_GET['mq8'])) {
    $data['mq8'] = $_GET['mq8'];
}
if (isset($_GET['mq135'])) {
    $data['mq135'] = $_GET['mq135'];
}

// 🌡️ Capsule 2 Temp/Humidity Sensors
if (isset($_GET['temp'])) {
    $data['temperature'] = floatval($_GET['temp']);
}
if (isset($_GET['hum'])) {
    $data['humidity'] = floatval($_GET['hum']);
}

// 🕒 Add/update timestamp
$data['timestamp'] = date("Y-m-d H:i:s");

// Save updated data
file_put_contents($file, json_encode($data));

// Optional response
echo json_encode(["status" => "success", "updated" => $data]);
?>
