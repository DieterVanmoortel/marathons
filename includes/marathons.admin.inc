<?php

function marathon_overview(){
  $data = process_data_from_db();
  if (empty($data)) {return t('No data in database.');}
  $header = array_keys(current($data));
  $out = t('The following rows are stored in the database:');
  $out .= theme('table', array('rows' => $data, 'header' => $header));
  return $out;
}
function marathon_export_csv(){
//  output_csv($data, $filename = 'list.csv');
  return 'This function is temporarily disabled';
}

function marathon_import_csv_form() {
  $form = array();
  $form['upload'] = array(
    '#type' => 'file',
    '#title' => t('Choose a csv file'),
    '#title_display' => 'invisible',
    '#progress_indicator' => 'bar',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Import csv file'
  );
  return $form;
}

function marathon_import_csv_form_validate(&$form, &$form_state){
  $validators = array('file_validate_extensions' => array('csv')); // allow all extensions
  $file = file_save_upload('upload', $validators, FALSE, FILE_EXISTS_REPLACE);
  $form_state['upload'] = $file->uri;
}

function marathon_import_csv_form_submit(&$form, &$form_state) {
  marathon_import_csv($form_state['upload']);  
  $form_state['redirect'] = 'admin/content/marathons';
}

