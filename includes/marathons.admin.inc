<?php

function marathon_overview(){
  $out = 'Marathons overview page';
  $data = process_data_from_db();
  $header = array_keys(current($data));
  $out = t('The following rows are stored in the database:');
  $out .= theme('table', array('rows' => $data, 'header' => $header));
  return $out;
}
function marathon_export_csv(){
//  output_csv($data, $filename = 'list.csv');
  return 'This function is temporarily disabled';
}

function marathon_import_csv_form(&$form, &$form_state) {
  $form = array();
  $form ['#attributes'] = array ('enctype' => "multipart/form-data" );
  $form['upload'] = array(
    '#type' => 'file',
    '#title' => t('Choose a csv file'),
    '#title_display' => 'invisible',
    '#progress_indicator' => 'bar',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Import Marathons'
  );
  return $form;
}

function marathon_import_csv_form_validate(&$form, &$form_state){
  $validators = array('file_validate_extensions' => array('csv')); // allow all extensions
  $file = file_save_upload('upload', $validators, FALSE, FILE_EXISTS_REMOVE);
  $form_state['upload'] = $file->uri;
}

function marathon_import_csv_form_submit(&$form, &$form_state) {
  marathon_import_csv($form_state['upload']);
}

