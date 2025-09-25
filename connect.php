<?php
$host  = "localhost";
$user = "root";
$password = "";
$database = "UAP";

$con = mysqli_connect($host,$user,$password,$database);

if(mysqli_connect_errno()){
    echo "Gagal melakukan koneksi ke MySQL: " . mysqli_connect_errno();
}