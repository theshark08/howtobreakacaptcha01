
<?

  $data = file_get_contents("../data.json");
  $arr = array();
//  exec("cd ../src ; ./build  2>&1",$arr);
  $msg = "";
//  foreach($arr as $v) $msg .= addslashes($v)."\\n";
//  if($msg != ""){ echo "<script>alert('$msg');</script>"; exit(); }
?>
<script src=../../jquery.min.js></script>
<script>
  data = <?echo $data;?>;
</script>
<body>
<div><a href=../ >&lt;&lt;&lt;&lt;</a></div>
<h1> DECAPTCHER TEST </h1>
<div id=stats></div><br>
<a href="javascript: stop=1;">[ STOP ]</a>
<div id=main></div>
</body>
<script>

var ct = 0;
var correct = 0;
var total_time = 0;
var stop = 0;
var err = 0;

var Fin = function(id){
 return function(data){
  input = data.match(/[0-9]{6}/);
  time = 1 * data.match(/([0-9]+)ms/)[1];
  total_time += time;
  valid = input == $("#"+id+"v").text();
  err += input == null;
  $("#"+id).append(
    "<tr><th colspan=2>"+data+"</th></tr>"
  ).css("background", "#" + (valid ? "dfd" : "fdd"));
  correct += valid;

  $("#stats").html("ok: "+correct+" total: "+ct+"<br>accuracy: "+Math.floor(correct*100/ct)+"%<br>avg time: "+Math.floor(total_time/ct)+"ms<br>error reporting: "+Math.floor(100*err/(ct-correct))+"%");
  if( !stop ) f();
 };
};
var Start = function(i){
  return function(data){
    $.get("e.php", { i: i }, Fin(i));
  };
};
var f = function(){
 for(var i in data){
   col = "ddd";
   $("#main").append(
    "<table id='"+i+"' style='float:left;background:#"+col+
    "; padding: 5px;margin: 5px;'><tr><th colspan=2 style='font-size: 8px;'>"+i+
    "</th></tr><tr><th><img src=../sample/"+i+".jpg /></th><th id="+i+
    "v>"+data[i].text+"</th></tr></table>"
   );
   $("#"+i+" img").load(Start(i));
   ct++;
   delete data[i];
   break;
 }
};

f();

</script>
