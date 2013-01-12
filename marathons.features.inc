<?php
/**
 * @file
 * marathons.features.inc
 */

/**
 * Implements hook_node_info().
 */
function marathons_node_info() {
  $items = array(
    'marathon' => array(
      'name' => t('Marathon'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
