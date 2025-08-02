<?php
$filename = 'commands.json';
$commands = json_decode(file_get_contents($filename), true);

foreach ($_POST as $key => $value) {
  $commands[$key] = filter_var($value, FILTER_VALIDATE_BOOLEAN);
}

file_put_contents($filename, json_encode($commands, JSON_PRETTY_PRINT));
echo "✅ Command updated";
?>
