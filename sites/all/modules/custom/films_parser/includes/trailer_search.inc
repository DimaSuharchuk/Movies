<?php
/**
 * @file
 * Provides functionality for search trailers on Youtube by imdb id.
 */

function films_parser_youtube_search_ajax_callback($form, $form_state) {
  $imdb_id = $form_state['values']['trailer_input'];

  try {
    // Get movie title and year for using as Youtube search parameters.
    $en_title = _get_movie_title_and_year_string_by_imdb_id($imdb_id);
    $ru_title = _get_movie_title_and_year_string_by_imdb_id($imdb_id, 'ru');

    // Create links on Youtube with search parameters.
    $l1 = l(t('Eng link'), 'https://youtube.com/results', [
      'query' => ['search_query' => "{$en_title} trailer"],
      'external' => TRUE,
      'attributes' => [
        'target' => ['_blank'],
      ],
    ]);
    $l2 = l(t('Rus link'), 'https://youtube.com/results', [
      'query' => ['search_query' => "{$ru_title} трейлер"],
      'external' => TRUE,
      'attributes' => [
        'target' => ['_blank'],
      ],
    ]);

    return "<div id='trailer-search-exceptions'>{$l1}<br>{$l2}</div>";
  } catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }

  return '<div id="trailer-search-exceptions"></div>';
}
