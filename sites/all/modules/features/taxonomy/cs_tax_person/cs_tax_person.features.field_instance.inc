<?php
/**
 * @file
 * cs_tax_person.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function cs_tax_person_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'taxonomy_term-person-field_person_type'.
  $field_instances['taxonomy_term-person-field_person_type'] = array(
    'bundle' => 'person',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_person_type',
    'label' => 'Type',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Type');

  return $field_instances;
}
