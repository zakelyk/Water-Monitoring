<?php
// Menyimpan data ke database
include "connect.php";

$temp = $_GET['temperature'];
$water_level = $_GET['water_level'];

mysqli_query($con, "ALTER TABLE tb_sensor AUTO_INCREMENT=1");
$save = mysqli_query($con, "INSERT INTO tb_sensor (temperature, water_level) values ('$temp','$water_level')");

if ($save) {
    echo "Berhasil dikirim";
} else {
    echo "Gagal Terkirim";
}
