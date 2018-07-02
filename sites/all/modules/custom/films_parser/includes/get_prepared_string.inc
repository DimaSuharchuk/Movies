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
  try {
    // Build some form for view and copy prepared string.
    $elements = [
      '#type' => 'fieldset',
    ];
    $elements['field'] = [
      '#type' => 'textfield',
      '#value' => _get_movie_title_and_year_string_by_imdb_id($form_state['values']['imdb_id']),
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

    return '<div id="prepared-string">' . drupal_render($elements) . '</div>';
  } catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }

  return '<div id="prepared-string"></div>';
}
