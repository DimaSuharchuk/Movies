<?php
/**
 * @file
 * cs_tax_directors.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function cs_tax_directors_taxonomy_default_vocabularies() {
  return array(
    'directors' => array(
      'name' => 'Directors',
      'machine_name' => 'directors',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
