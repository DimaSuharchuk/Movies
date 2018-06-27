<?php
/**
 * @file
 * Provides form for search movies by imdb id and build string with name and
 *   year.
 */

/**
 * Build string with eng movie name and year from IMDB ID.
 *
 * @param $form
 * @param $form_state
 *
 * @return string
 */
function films_parser_get_prepared_string_ajax_callback($form, $form_state) {
  $id = $form_state['values']['imdb_id'];

  // Check for valid imdb id format.
  if (!preg_match('@tt\d{7}@', $id)) {
    drupal_set_message(t('Wrong format of IMDB ID.'), 'error');
  }
  else {
    // Get eng movie node.
    $query = new EntityFieldQuery();
    $query
      ->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'film')
      ->propertyCondition('language', 'en')
      ->fieldCondition('field_imdb_id', 'value', $id);
    $result = $query->execute();

    if (empty($result)) {
      drupal_set_message(t('IMDB ID "@id" not found in DB.', ['@id' => $id]), 'error');
    }
    else {
      $node = node_load(end($result['node'])->nid);
      $year = $node->field_year[LANGUAGE_NONE][0]['value'];

      // Build some form for view and copy prepared string.
      $elements = [
        '#type' => 'fieldset',
      ];
      $elements['field'] = [
        '#type' => 'textfield',
        '#value' => "{$node->title} {$year}",
        '#attributes' => [
          'id' => 'prepared-text-field',
        ],
      ];
      $elements['button'] = [
        '#type' => 'submit',
        '#value' => t('Copy'),
        '#attributes' => [
          'id' => ['copy-prepared-text'],
        ],
      ];
      $elements['copied'] = [
        '#markup' => t('copied'),
        '#prefix' => '<span class="element-hidden copied">',
        '#suffix' => '</span>',
      ];
      drupal_add_js(FILMS_PARSER_MODULE_PATH . '/js/copy_text.js');

      return '<div id="prepared-string">' . drupal_render($elements) . '</div>';
    }
  }

  return '<div id="prepared-string"></div>';
}
