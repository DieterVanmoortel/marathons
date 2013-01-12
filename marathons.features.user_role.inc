<?php
/**
 * @file
 * feature_marathon.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function marathons_user_default_roles() {
  $roles = array();

  // Exported role: Editor.
  $roles['Editor'] = array(
    'name' => 'Editor',
    'weight' => '2',
  );

  return $roles;
}
