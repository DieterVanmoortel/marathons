<?php

function marathon_overview(){
  $out = 'Marathons overview page';
  $out .= marathon_import_csv();
  return $out;
}
function marathon_export_csv(){
  $filename = drupal_get_path('module', 'marathons') . '/data/marathons.csv';
  $data = _retrieve_data($filename);
//  output_csv($data, $filename = 'list.csv');
  return 'This function is temporarily disabled';
}

function marathon_import_csv(){
  $filename = drupal_get_path('module', 'marathons') . '/data/marathons_short.csv';
  if(file_exists($filename)){
    $data = _retrieve_data($filename);
    if(!empty($data)){
      $header = array_shift($data);
      $output = theme('table', array('rows' => $data, 'header' => $header));
      _write_data_to_db($data);
      return $output;
    }
  }
}

function marathons_process_csv(){
  $output = t('Processing imported events');

  $data = marathons_get_data_from_db();
  foreach((array)$data as $name => $object) {
    // new or existing node?
    $new = TRUE; // dev

    if($new) {
      $node = setup_new_event_node($object);
    }
    else{
      $node = node_load($nid);
    }
    // add other data fields
    node_save($node);
  }
  return $output;
}