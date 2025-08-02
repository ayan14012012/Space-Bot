<?php
// Get values from URL
$pulse = isset($_GET['pulse']) ? $_GET['pulse'] : '0';
$alert = isset($_GET['alert']) ? $_GET['alert'] : 'none';
$touch = isset($_GET['touch']) ? $_GET['touch'] : 'no';

// Combine all into one array
$data = array(
    'pulse' => $pulse,
    'alert' => $alert,
    'touch' => $touch
);

// Convert to JSON and save in data.txt
file_put_contents('data.txt', json_encode($data));
echo "Data Updated: " . json_encode($data);
?>
