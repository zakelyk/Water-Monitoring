<?php

include "connect.php";

// Set header respons sebagai JSON
header('Content-Type: application/json');

// Ambil nilai dari URL atau atur nilai default menjadi "mati" jika tidak ada
$status = isset($_GET['status']) ? $_GET['status'] : 'mati';
$pump_status = isset($_GET['pump_status']) ? $_GET['pump_status'] : 'mati';
$servo_status = isset($_GET['servo_status']) ? $_GET['servo_status'] : 'mati';

// Gunakan query UPDATE yang benar
$save = mysqli_query($con, "UPDATE tb_status SET status='$status', pump_status='$pump_status', servo_status='$servo_status' WHERE id='1'");

// Berikan respons JSON sesuai hasil operasi
if ($save) {
    echo json_encode(['status' => 'success']);
} else {
    echo json_encode(['status' => 'error']);
}
