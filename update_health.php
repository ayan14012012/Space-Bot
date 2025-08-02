<?php
$data = json_decode(file_get_contents("php://input"), true);
$file = "assets/data/health_data.json";

$existing = json_decode(file_get_contents($file), true);

$existing["readings"][] = [
  "pulse" => $data["pulse"],
  "bpm" => $data["bpm"],
  "timestamp" => time()
];

if (count($existing["readings"]) > 100) {
    $existing["readings"] = array_slice($existing["readings"], -100);
}

file_put_contents($file, json_encode($existing, JSON_PRETTY_PRINT));
echo "✅ Data updated.";
?>
