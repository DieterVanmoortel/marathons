<?php

class MarathonsMigration extends Migration {
  public function __construct() {
    parent::__construct(MigrateGroup::getInstance('marathons'));

    //The Description of the import. This desription is shown on the Migrate GUI
    $this->description = t('Import of Marathons.');

    //The Source of the import
    $query = db_select('marathon_raw', 'raw')
             ->fields('raw');
    $this->source = new MigrateSourceSQL($query);
    
    //The destination CCK (bundle)
    $this->destination = new MigrateDestinationNode('marathon');

    //Source and destination relation for rollbacks
    $this->map = new MigrateSQLMap(
      $this->machineName,
      array(
        'name' => array(
          'type' => 'varchar',
          'length' => '128',
          'not null' => TRUE,
          'alias' => 'raw',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    //Field mapping
    $this->addFieldMapping('title', 'name' );
    $this->addFieldMapping('field_date', 'date' );
    $this->addFieldMapping('field_city', 'city' );
    $this->addFieldMapping('field_country','country');
    $this->addFieldMapping('field_distances','distance');
    $this->addFieldMapping('field_days','days');
    $this->addFieldMapping('field_website','website');
  }
  public function prepareRow($row) {
    // convert date to UNIX timestamp
    if(!empty($row->date)) {
      $date = explode('/', $row->date);
      $row->date = mktime(0,0,0,$date[1], $date[0], $date[2]);
    }
    // build other fields
    $data = unserialize($row->data);
    $row->date = REQUEST_TIME;
    foreach((array)$data as $key => $value){
      switch($key){
        case 'days':
          $value = empty($value) ? 1 : $value;
          break;
      }
      $row->$key = $value;
    }
    return TRUE;
  }
}