<?php

/**
 * @file
 * Batch for IMDB voting field updating.
 */

/**
 * Set batch for movies voting update.
 *
 * @throws \Exception
 */
function films_update_rating_batch_set() {
  // Get all unique IMDB ids from DB.
  $query = db_select('field_data_field_imdb_id', 'i')
    ->fields('i', ['field_imdb_id_value'])
    ->execute()
    ->fetchCol();
  $imdb_ids = array_unique($query);
  // Get nodes id for en and ru languages.
  $en_node_ids = _get_node_ids_by_imdb_ids($imdb_ids, 'en');
  $ru_node_ids = _get_node_ids_by_imdb_ids($imdb_ids, 'ru');

  $operations = [];
  foreach ($en_node_ids as $imdb_id => $en_node_id) {
    $operations[] = [
      'films_update_rating_batch',
      [
        $imdb_id,
        $en_node_id,
        $ru_node_ids,
      ],
    ];
  }

  $batch = [
    'operations' => $operations,
    'finished' => 'films_update_rating_batch_finished',
  ];
  batch_set($batch);
}

/**
 * Update ratings.
 *
 * @param $imdb_id
 *  IMDB id like tt1234567.
 * @param $en_node_id
 *  Node ID with movie on English.
 * @param $ru_node_ids
 *  Node ID with movie on Russian.
 * @param $context
 *  Batch context.
 */
function films_update_rating_batch($imdb_id, $en_node_id, $ru_node_ids, &$context) {
  // Fetch IMDB rating for single movie.
  $new_rating_value = (float) _fetch_json_from_omdb($imdb_id, FALSE)['imdbRating'];
  // Get ENG node wrapper and get old movie's rating.
  $en_wrapper = entity_metadata_wrapper('node', $en_node_id);
  $old_rating_value = (float) $en_wrapper->field_imdb->value();

  // Compare old rating and new. If different - update for ENG and RU versions.
  if ($new_rating_value !== $old_rating_value) {
    $en_wrapper->field_imdb->set($new_rating_value);
    $en_wrapper->save();
    // Update for russian version.
    $ru_wrapper = entity_metadata_wrapper('node', $ru_node_ids[$imdb_id]);
    $ru_wrapper->field_imdb->set($new_rating_value);
    $ru_wrapper->save();

    // Save some useful info to context when movie has been updated.
    $context['results']['count']++;
    $context['results']['films'][] = [
      'name' => $ru_wrapper->label(),
      'old' => $old_rating_value,
      'new' => $new_rating_value,
    ];
  }
}

/**
 * Show results if some movies has been updated.
 *
 * @param bool $success
 *  Batch success.
 * @param $results
 *  Results from batch context.
 */
function films_update_rating_batch_finished($success, $results) {
  if ($success) {
    if ($results) {
      // Show updated movies count.
      drupal_set_message(format_plural($results['count'], 'Updated 1 film.', 'Updated @count films.'));
      // Show movies name and old/new ratings.
      foreach ($results['films'] as $film) {
        drupal_set_message(t('%name: %old => %new.', [
          '%name' => $film['name'],
          '%old' => $film['old'],
          '%new' => $film['new'],
        ]));
      }
    }
    // Save date and time when last update was performed.
    variable_set('last_films_rating_update', format_date(REQUEST_TIME, 'custom', 'd M Y H:i'));
  }
}
