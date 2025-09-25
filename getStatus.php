<?php
include "connect.php";

$sql = mysqli_query($con, "SELECT * from tb_status ORDER BY id DESC");

$data = mysqli_fetch_array($sql);
$status = $data['status'];
$pump_status = $data['pump_status'];
$servo_status = $data['servo_status'];

$response = array(
    'status' => $status,
    'pump_status' => $pump_status,
    'servo_status' => $servo_status
);

echo json_encode($response);