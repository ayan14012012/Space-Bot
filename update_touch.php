<?php
if (isset($_GET['touch'])) {
    $touch = $_GET['touch'];
    $data = [
        "touch" => $touch
    ];
    file_put_contents("touch_data.txt", json_encode($data));
    echo "Touch data updated.";
} else {
    echo "No touch data received.";
}
?>
