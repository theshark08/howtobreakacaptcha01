<?//twitr_br

  $k = $_REQUEST["i"];
  $k = str_replace("_", ".", $k);
  $stamp = microtime(1);
  echo exec("../break_captcha ../sample/$k.jpg");
  echo " :: " . number_format((microtime(1)-$stamp)*1000, 0) . "ms" ;

?>
