<?php
// Update command files
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (isset($_POST["capsule1"])) {
        file_put_contents("capsule1_command.txt", $_POST["capsule1"]);
    }
    if (isset($_POST["capsule2"])) {
        file_put_contents("capsule2_command.txt", $_POST["capsule2"]);
    }
}

// Read current command statuses
$c1_cmd = trim(file_get_contents("capsule1_command.txt"));
$c2_cmd = trim(file_get_contents("capsule2_command.txt"));
?>

<!DOCTYPE html>
<html>
<head>
    <title>🚀 SpaceBot Mission Control Panel</title>
    <style>
        body {
            background-color: #000814;
            font-family: 'Segoe UI', sans-serif;
            color: white;
            text-align: center;
            padding: 30px;
        }

        h1 {
            color: #00d9ff;
            margin-bottom: 40px;
        }

        .capsule-control {
            background: #001d3d;
            border: 2px solid #00d9ff;
            border-radius: 15px;
            padding: 25px;
            width: 340px;
            margin: 30px auto;
            box-shadow: 0 0 20px #0077b6;
        }

        .capsule-control h2 {
            color: #90e0ef;
        }

        .switch-btn {
            font-size: 18px;
            padding: 12px 28px;
            margin: 10px;
            border: none;
            border-radius: 50px;
            cursor: pointer;
            transition: 0.3s;
            box-shadow: 0 0 10px #000;
        }

        .start {
            background: #00ff88;
            color: #002b36;
            box-shadow: 0 0 15px #00ff88;
        }

        .start:hover {
            background: #00cc66;
        }

        .stop {
            background: #ff3b3b;
            color: #fff;
            box-shadow: 0 0 15px #ff3b3b;
        }

        .stop:hover {
            background: #cc0000;
        }

        .status {
            margin-top: 15px;
            font-size: 18px;
            color: #ffd60a;
        }
    </style>
</head>
<body>

    <h1>🛰 SpaceBot Capsule Mission Control</h1>

    <!-- Capsule 1 -->
    <div class="capsule-control">
        <h2>Capsule 1 Control</h2>
        <form method="POST">
            <button class="switch-btn start" type="submit" name="capsule1" value="START">Start Motor</button>
            <button class="switch-btn stop" type="submit" name="capsule1" value="STOP">Stop Motor</button>
        </form>
        <div class="status">🟢 Current Status: <strong><?php echo $c1_cmd; ?></strong></div>
    </div>

    <!-- Capsule 2 -->
    <div class="capsule-control">
        <h2>Capsule 2 Control</h2>
        <form method="POST">
            <button class="switch-btn start" type="submit" name="capsule2" value="START">Start Motor</button>
            <button class="switch-btn stop" type="submit" name="capsule2" value="STOP">Stop Motor</button>
        </form>
        <div class="status">🟢 Current Status: <strong><?php echo $c2_cmd; ?></strong></div>
    </div>

</body>
</html>
