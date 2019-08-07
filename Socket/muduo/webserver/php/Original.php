<html>
<body>
<?php
        #预定义的 $_REQUEST 变量包含了 $_GET、$_POST 和 $_COOKIE 的内容。
        #$_REQUEST 变量可用来收集通过 GET 和 POST 方法发送的表单数据。
    $a=$_REQUEST["a"];
    $b=$_REQUEST["b"];
    echo  $a. $b.": Do not handle it for the time being"
    // echo '1';
    // var_dump($_REQUEST);
    // echo $a;
?>
</body>
</html>
