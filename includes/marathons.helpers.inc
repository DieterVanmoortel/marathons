<?php
/*
 * Write data from csv to database
 */
function _write_data_to_db($data) {
  backup_and_emtpy_db($data);
  $errors = 0;
  foreach((array)$data as $row) {
    $date = array_shift($row);
    $name = array_shift($row);
    $data = $row;
    if(!empty($name)){
      try{
        db_insert('marathon_raw')
        ->fields(array(
          'name' => $name,
          'date' => $date,
          'data' => serialize($data),
          'created' => REQUEST_TIME,
        ))->execute();
      }
      catch(Exception $e){
        $errors++;
        watchdog_exception('marathon import', $e);
      }
    }
  }
  if($errors <> 0) {
    drupal_set_message(t('#count rows failed to be imported. Check watchdog for details.', array('#count' => $errors)));
  }
}

function marathons_get_data_from_db(){
   return db_query('SELECT * FROM {marathon_raw}')->fetchAllAssoc('name');
}

function backup_and_emtpy_db($data) {
  if(!empty($data)){
  db_insert('marathon_archive')
    ->fields(array(
      'date' => REQUEST_TIME,
      'data' => serialize($data),
    ))
    ->execute();

    db_truncate('marathon_raw')->execute(); // empty db
  }
}


/*
 * Prepare a new event node
 */
function setup_new_event_node($data) {
  $node = new stdClass();
  $node->title = $data->name;
  $node->type = 'event';
  $node->is_new = TRUE;
  node_object_prepare($node);
  return $node;
}

/**
*********************** CSV IN / OUT ******************************
 */
function _retrieve_data($filename){
  $output = array();
  $fh = fopen($filename, 'r');
  while(($data = fgetcsv($fh, 1000, ",")) != FALSE) {
   $output[] = explode(';', $data[0]);
  }
  fclose($fh);
  return $output;
}
function output_csv($data, $filename = 'list.csv') {
  $outputfile = str_replace('.csv', '_'. date('d_M_Y', time()). '.csv', $filename);
  header('Pragma', 'public');
  header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
  header('Content-Type: application/octet-stream');
  header('Content-Disposition: attachment; filename=' . $outputfile . ';');
  $tmp = file_directory_temp();
  $fp = fopen($tmp . '/'. $filename, 'w');
  foreach ((array)$data as $row) {
    fputcsv($fp, $row);
  }
  fclose($fp);
  ob_clean();
  flush();
  readfile($tmp . '/' . $filename);
  exit;
}