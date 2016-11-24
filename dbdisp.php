<!--Alexander Lapena | 0844071 | CIS* 2750 - A4 | 17 November 2016-->
<html>
<head>
  <title>HTML File</title>
</head>
  
<body>
  <?php
    $arg = $_GET['name'];
    $arg = chop($arg, ".html");
    $test = shell_exec("./db -disp ".$arg);
    echo $test;
  ?>
</body>

</html>
