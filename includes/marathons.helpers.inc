<?php
/*
 * Write data from csv to database
 */
function _write_data_to_db($data) {
  backup_and_emtpy_db($data);
  $errors = 0;
  $fields = array(
    'city',
    'distances',
    'altimeters',
    'country',
    'days',
    'website',
  );
  foreach((array)$data as $row) {
    $date = array_shift($row);
    $name = array_shift($row);
    $data = array_combine($fields, $row);
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

function process_data_from_db() {
  $data = marathons_get_data_from_db();
  dpm($data);
  if($data){
    foreach($data as $name => $row){
      $rowdata = unserialize($row->data);
      $out[$row->name]['name'] = $row->name;
      foreach((array)$rowdata as $key => $value) {
        $out[$row->name][$key] = $value;
      }
    }
    return $out;
  }
}

function marathons_get_data_from_db() {
   return db_query('SELECT * FROM {marathon_raw}')->fetchAllAssoc('name');
}

function backup_and_emtpy_db($data) {
  if(!empty($data)){ // write to archives
    $result = db_insert('marathon_archive')
    ->fields(array(
      'created' => REQUEST_TIME,
      'data' => serialize($data),
    ))
    ->execute();
    if ($result == '0') {
      db_truncate('marathon_raw')->execute(); // empty db
    }

  }
}

/**
*********************** CSV IN / OUT ******************************
*/
function marathon_import_csv($filename) {
  if(file_exists($filename)){
    $data = _retrieve_data($filename);
    if(!empty($data)){
      $header = array_shift($data);
      _write_data_to_db($data);
    }
  }
}

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