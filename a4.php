<!--Alexander Lapena | 0844071 | CIS* 2750 - A4 | 17 November 2016-->
<html>
<head>
  <!--       -->
  <!--Styling-->
  <!--       -->
  <style type="text/css">
    #conv {
    position:fixed;
    top: 50%;
    left: 50%;
    transform: translate(-50%,-70%);
    width: 550px;
    height: 400px;
    margin: 50px auto;
    border: 2px solid;
    padding: 10px;
    background: #D3D3D3;
    border-radius: 8px;
    display:none;
    }

    #dbStore {
    position:fixed;
    top: 50%;
    left: 50%;
    transform: translate(-50%,-70%);
    width: 550px;
    height: 400px;
    margin: 50px auto;
    border: 2px solid;
    padding: 10px;
    background: #D3D3D3;
    border-radius: 8px;
    display:none;
    }
    
    #upload {
    position:fixed;
    top: 50%;
    left: 50%;
    transform: translate(-50%,-70%);
    width: 550px;
    height: 400px;
    margin: 50px auto;
    border: 2px solid;
    padding: 10px;
    background: #D3D3D3;
    border-radius: 8px;
    display:none;
    }
  </style>
    
  <title>HTML Converter</title>
  
  <!--          -->
  <!--Javascript-->
  <!--          -->
  <script type="text/javascript">
    
    function toggle(id) {
      var e = document.getElementById(id);
      if(e.style.display == 'block') {
        e.style.display = 'none';
      } else {
        e.style.display = 'block';
      }
    }
    
    function chooseFile() {
      var e = document.getElementById("fileInput");
      e.style.display = 'block';
      document.getElementById("fileInput").click();
    }
    
  </script>
  
</head> 
  
<body>  
  <div style="width: 100%;">
    <!--                  -->
    <!--Top button section-->
    <!--                  -->
    <div style="text-align: center; position:relative; width: 100%; height:10% border-width: 1px 0 0 0;">     
      <button id="btn" type="submit" onclick="toggle('conv');" style="width: 33%; float: right;"><h1>Convert</h1></button>
      <button id="btn" type="submit" onclick="toggle('dbStore');" style="width: 34%; float: right"><h1>DB Store</h1></button>
      <button id="btn" type="submit" onclick="chooseFile();" style="width: 33%; float: right;"><h1>Upload</h1></button>   
    </div>

    <!--              -->
    <!--CONVERT WINDOW-->
    <!--              -->
    <div id="conv">
      <div class="convWrapper">
        <div class="convContent">
          <fieldset>
          <!--CONVERT-->
          <h3 style="text-align: center;">Convert Files</h3>
          <form action="" method="post">
          <div style="height:65%; width:95%; overflow:auto; border:1px solid black; padding:2%; background:#FFFFFF;">           
            <!--Convert Functionality-->
            <?php
            $path = "./files/";
            
            if($dir = opendir($path)){
              while (($file = readdir($dir)) !== FALSE) {
                // Display each text file as a radio button
                if ((!strpos($file, '.') || strcmp(substr($file, -4),".txt") == 0) && $file != '.' && $file != '..' ) {                
                  echo "<input type=\"radio\" id=\"radio\"  name=\"radio\" style=\"font-size:11px\"; value=\" " . $file . "\" >" . $file. "<br><br>";
                }
              }
              closedir($dir);
            }           
            ?>
          </div>
          <!--Convert Buttons + funcionality-->
          <div style="text-align: center; position:relative; width: 100%; height:10% border-width: 1px 0 0 0;">
            <button type="submit" name="submit" style="width: 45%;">Okay</button>
            <button href="javascript:void(0)" onclick="toggle('conv');" style="width: 45%;">Cancel</button>
            <?php
              if(isset($_POST['submit'])){
                if(isset($_POST['radio'])){
                  $fname = "files/" . $_POST['radio'];
                  $fname = str_replace(' ', '', $fname);
                  // Call C program then refresh
                  exec("./a4 ".$fname." 0");                              
                  header("location:./a4.php");   
                }
              }
            ?>
          </div>
          </form>
          </fieldset>
        </div>
      </div>
    </div>
    <!--      -->
    <!--UPLOAD-->
    <!--      -->
    <div style="height:0x;overflow:hidden;display:none">
      <form action="a4.php" method="post" enctype="multipart/form-data">
        <input type="file" id="fileInput" name="fileInput" onchange="javascript:this.form.submit()"/>
        <input type="button" onclick="Document.getElementById('fileInput').click()" value="submit">
       
      </form>
    </div>
      <!--Uploading to server functionality-->
      <?php
        if(isset($_FILES['fileInput'])){
          $target_dir = "./files/";
          $test = $_FILES['fileInput']['name'];
          move_uploaded_file($_FILES['fileInput']['tmp_name'],$target_dir.$test);          
          header("location:./a4.php");
        }
      ?>
    <!--        -->
    <!--DB Store-->
    <!--        -->
    <div id="dbStore">
      <div class="dbWrapper">
        <div class="dbContent">
          <fieldset>
          <!--DB Popup-->
          <h3 style="text-align: center;">DB Store</h3>
          <form action="" method="post">
          <div style="height:65%; width:95%; overflow:auto; border:1px solid black; padding:2%; background:#FFFFFF;">           
            <!--Convert Functionality-->
            <?php
            $path = "./files/";
            
            if($dir = opendir($path)){
              while (($file = readdir($dir)) !== FALSE) {
                // Display each text file as a radio button
                if ((!strpos($file, '.') || strcmp(substr($file, -4),".txt") == 0) && $file != '.' && $file != '..' ) {                
                  echo "<input type=\"radio\" id=\"dbradio\"  name=\"dbradio\" style=\"font-size:11px\"; value=\" " . $file . "\" >" . $file. "<br><br>";
                }
              }
              closedir($dir);
            }           
            ?>
          </div>
          <!--Store Buttons + Conversion funcionality-->
          <div style="text-align: center; position:relative; width: 100%; height:10% border-width: 1px 0 0 0;">
            <button type="submit" name="insert" style="width: 45%;">Okay</button>
            <button href="javascript:void(0)" onclick="toggle('dbStore');" style="width: 45%;">Cancel</button>
            <?php
              if(isset($_POST['insert'])){
                if(isset($_POST['dbradio'])){
                  $fname = "files/" . $_POST['dbradio'];
                  $fname = str_replace(' ', '', $fname);
                  // Call C program then refresh
                  exec("./a4 ".$fname." 1");    
                  //send to db then remove file and refresh
                  header("location:./a4.php");   
                }
              }
            ?>
          </div>
          </form>
          </fieldset>
        </div>
      </div>
    </div>

    <!--              -->
    <!--Text file List-->
    <!--              -->
    <div style="width:48%; height:80%; float:left;line-height:3em; background-color:#A9A9A9;">
      <fieldset>
        <legend style="font-weight:bold;">Text Files</legend>
        <div style="height:80%; width:95%; overflow:auto; border:1px solid black; padding:2%; background:#FFFFFF;">
          <!--Printing text files in lists-->
          <?php
          $path = "./files/";
          if($dir = opendir($path)){
            while (($file = readdir($dir)) !== FALSE) {
              // Display text files only
              if ((!strpos($file, '.') || strcmp(substr($file, -4),".txt") == 0) && $file != '.' && $file != '..' ) {
                echo "<a href=\"./files/" . $file . "\"><h2>" . $file . "</h2></a>";
              }
            }
            closedir($dir);
          }           
          ?>
        </div>
      </fieldset>
    </div>

    <!--              -->
    <!--HTML file List-->
    <!--              -->
    <div style="width:48%; height:80%; float:right;line-height:3em;background-color:#A9A9A9;">
      <fieldset>
        <legend style="font-weight:bold;">HTML Files</legend>
        <div style="height:80%; width:95%; overflow:auto; border:1px solid black; padding:2%; background:#FFFFFF;">
          <!--printing html files in lists-->
          <?php
          $path = "./files/";
          $fileList = array();
          if($dir = opendir($path)){
            while (($file = readdir($dir)) !== FALSE) {
              if (strcmp(substr($file, -5),".html") == 0) {
                //add to array
                $fileList[] = $file;
              }
            }
            closedir($dir);
          } 
          
          // add database to array
          $dbArray = array();
          $allFiles = array();
          $dbFile = shell_exec("./db -db");
          if(dbFile!= NULL){
            $dbArray = explode(' ', $dbFile);
          }
          $allFiles = array_merge($fileList, $dbArray);
        
          sort($allFiles, SORT_NATURAL | SORT_FLAG_CASE);
          foreach($allFiles as $key => $val) {
            //check from where it came from for hyperlink?
            if (in_array($allFiles[$key], $fileList)) {
              echo "<a href=\"./files/" . $allFiles[$key] . "\"><h2>" . $allFiles[$key] . "</h2></a>";
            }
            elseif (in_array($allFiles[$key], $dbArray)) {
              echo "<a href=\"dbdisp.php?name=".$allFiles[$key]."\"><h2>".$allFiles[$key]."</h2></a>";
            }
          }         
  
          ?>    
        </div>
      </fieldset>
    </div>
    <!--                    -->
    <!--Rights (Bottom Text)-->
    <!--                    -->
    <div style="background-color:#B5DCB3; clear:both">
      <center>
        &copy; 2016 Alexander Lapena | All Rights Reservered
      </center>
    </div>
  </div>

</body>
</html>
