<?php
/**
 * @file
 * Batch functionality for add trailers for nodes.
 */

/**
 * Film add trailer form submit.
 */
function films_parser_add_trailers($form, &$form_state) {
  $file = file_load($form_state['values']['csv_trailer']);
  $content = file_get_contents($file->uri);
  $csv_strings = explode("\n", $content);

  $trailers_from_file = [];
  foreach ($csv_strings as $string) {
    @list($imdb_id, $en_trailer, $ru_trailer, $exception) = str_getcsv($string);
    if ($imdb_id) {
      $imdb_id = trim($imdb_id);
      if (!_imdb_id_is_valid($imdb_id)) {
        drupal_set_message(t('"@id" is invalid.', ['@id' => $imdb_id]), 'warning');
        continue;
      }
      if ($exception) {
        drupal_set_message(t('"@id" has wrong string.', ['@id' => $imdb_id]), 'warning');
        continue;
      }

      $en_trailer = trim($en_trailer);
      $ru_trailer = trim($ru_trailer);

      if (!_trailer_is_valid($en_trailer, TRUE)) {
        drupal_set_message(t('Trailer hash "@t" for ID "@id" has wrong format.', [
          '@t' => $en_trailer,
          '@id' => $imdb_id,
        ]), 'warning');
        $en_trailer = '';
      }
      if (!_trailer_is_valid($ru_trailer, TRUE)) {
        drupal_set_message(t('Trailer hash "@t" for ID "@id" has wrong format.', [
          '@t' => $ru_trailer,
          '@id' => $imdb_id,
        ]), 'warning');
        $ru_trailer = '';
      }

      // Build finished array of trailers from file.
      if ($en_trailer || $ru_trailer) {
        $trailers_from_file[$imdb_id] = [
          'en' => $en_trailer,
          'ru' => $ru_trailer,
        ];
      }
    }
  }

  // Select all eng films to accordance with ids from file.
  $en_node_ids = [];
  try {
    $en_node_ids = _get_node_ids_by_imdb_ids(array_keys($trailers_from_file));
  } catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }

  // Select all rus films to accordance with ids from file.
  $ru_node_ids = [];
  try {
    $ru_node_ids = _get_node_ids_by_imdb_ids(array_keys($trailers_from_file), 'ru');
  } catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }

  // Filter from undefined items translations from file.
  $trailers_from_file = array_intersect_key($trailers_from_file, $en_node_ids);

  // Collect eng and rus nodes for batch.
  $all_nodes = array_flip($en_node_ids) + array_flip($ru_node_ids);

  $mode = $form_state['values']['trailer_settings'];

  // Batch items.
  if (!empty($trailers_from_file)) {
    $operations = [];
    foreach ($all_nodes as $id => $imdb_id) {
      $operations[] = [
        'films_parser_trailer_batch',
        [
          $id,
          $trailers_from_file[$imdb_id],
          $mode,
        ],
      ];
    }

    $batch = [
      'operations' => $operations,
      'finished' => 'films_parser_trailer_batch_finished',
    ];
    batch_set($batch);
  }
  else {
    drupal_set_message(t('Already up-to-date.'));
  }
}

/**
 * Validate for load a file with trailers.
 */
function films_parser_add_trailers_validate($form, &$form_state) {
  if (!$form_state['values']['csv_trailer']) {
    form_set_error('csv_trailer', t('Load csv file.'));
    drupal_add_library('system', 'drupal.collapse');
  }
}

/**
 * Set trailers to nodes.
 *
 * @param $node_id
 *  Node ID.
 * @param $trailers
 *  Array with eng and rus trailer values.
 * @param $mode
 *  Only add new or update old and add new.
 * @param $context
 *  Array with batch processing values.
 */
function films_parser_trailer_batch($node_id, $trailers, $mode, &$context) {
  $wrapper = entity_metadata_wrapper('node', $node_id);
  $lang = $wrapper->language->value();

  $empty_trailer = empty($wrapper->field_trailer->value());

  if (!$empty_trailer) {
    switch ($mode) {
      case 'add_new_trailer':
        // Don't update node for this mode if field already filled.
        return;
        break;

      case 'add_update_trailer':
        $context['results']['updated'][$lang]++;
        break;
    }
  }
  // Not necessary rewrite empty with empty.
  elseif ($empty_trailer && empty($trailers[$lang])) {
    return;
  }
  else {
    $context['results']['added'][$lang]++;
  }

  $wrapper->field_trailer->set($trailers[$lang] ? $trailers[$lang] : NULL);
  $wrapper->save();
}

/**
 * Batch finish function.
 *
 * @param $success
 * @param $results
 * @param $operations
 */
function films_parser_trailer_batch_finished($success, $results, $operations) {
  if ($success) {
    $added_count_en = isset($results['added']['en']) ? $results['added']['en'] : 0;
    $updated_count_en = isset($results['updated']['en']) ? $results['updated']['en'] : 0;
    $added_count_ru = isset($results['added']['ru']) ? $results['added']['ru'] : 0;
    $updated_count_ru = isset($results['updated']['ru']) ? $results['updated']['ru'] : 0;

    $added_en = format_plural(
      $added_count_en,
      '1 english trailer added.',
      '@count english trailers added.'
    );
    $updated_en = format_plural(
      $updated_count_en,
      '1 english trailer updated.',
      '@count english trailers updated.'
    );
    $added_ru = format_plural(
      $added_count_ru,
      '1 russian trailer added.',
      '@count russian trailers added.'
    );
    $updated_ru = format_plural(
      $updated_count_ru,
      '1 russian trailer updated.',
      '@count russian trailers updated.'
    );

    drupal_set_message("$added_en<br>$added_ru<br>$updated_en<br>$updated_ru");
  }
  else {
    $error_operation = reset($operations);
    drupal_set_message(
      t('An error occurred while processing @operation with arguments : @args',
        [
          '@operation' => $error_operation[0],
          '@args' => print_r($error_operation[0], TRUE),
        ]
      ),
      'error'
    );
  }
}

/**
 * Check is trailer hash is valid.
 *
 * @param $trailer
 *  Youtube video id, for example: 3_ZF518dvXc
 * @param bool $or_empty
 *  Returns true if trailer hash is valid or empty.
 *
 * @return false|int
 */
function _trailer_is_valid($trailer, $or_empty = FALSE) {
  $match = preg_match('/^[A-Za-z\d-_]{11}$/', $trailer);
  return $or_empty ? $match || empty($trailer) : $match;
}
