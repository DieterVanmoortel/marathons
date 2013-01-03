<?php

function marathon_overview(){
  $out = 'Marathons overview page';
  $out .= marathon_import_csv();
  return $out;
}
function marathon_export_csv(){
//  output_csv($data, $filename = 'list.csv');
  return 'This function is temporarily disabled';
}
