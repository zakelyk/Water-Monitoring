<?php
//Mengambil data dari database
include "connect.php";

$sql = mysqli_query($con, "SELECT * from tb_sensor ORDER BY id DESC LIMIT 1");

$data = mysqli_fetch_array($sql);
$temperature = $data['temperature'];
$water_level = $data['water_level'];

$response = array(
    'temperature' => $temperature,
    'water_level' => $water_level,
);

echo json_encode($response);
