<?php
/**
 * @file
 * cs_tax_actors.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function cs_tax_actors_taxonomy_default_vocabularies() {
  return array(
    'actors' => array(
      'name' => 'Actors',
      'machine_name' => 'actors',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
