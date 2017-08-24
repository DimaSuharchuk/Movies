<?php
/**
 * @file
 * cs_ct_film.features.inc
 */

/**
 * Implements hook_node_info().
 */
function cs_ct_film_node_info() {
  $items = array(
    'film' => array(
      'name' => t('Film'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Film title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
