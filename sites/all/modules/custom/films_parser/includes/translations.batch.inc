<?php
/**
 * @file
 * Batch functionality for create translated nodes.
 */

/**
 * Film add translations form submit.
 */
function films_parser_add_translated_films($form, &$form_state) {
  $file = file_load($form_state['values']['csv']);
  $content = file_get_contents($file->uri);
  $csv_strings = explode("\n", $content);

  $translations_from_file = [];
  foreach ($csv_strings as $string) {
    @list($imdb_id, $title, $description, $exception) = str_getcsv($string);
    if ($imdb_id) {
      if (!$description || $exception) {
        drupal_set_message(t('"@id" has wrong string.', ['@id' => $imdb_id]), 'warning');
        continue;
      }

      // Prepare string.
      $patterns = [
        '/([.,»:?!—)])([^ .])/ui',
        '/…/',
      ];
      $replacements = [
        '\1 \2',
        '...',
      ];

      // Build finished array of movies translations from file.
      $translations_from_file[trim($imdb_id)] = [
        'title' => trim($title),
        'description' => trim(preg_replace($patterns, $replacements, $description)),
      ];
    }
  }

  // Select all eng films to accordance with ids from file.
  try {
    $en_node_ids = _get_node_ids_by_imdb_ids(array_keys($translations_from_file));
  } catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
  // Existing eng films ids.
  $isset_en_imdb_ids = array_keys($en_node_ids);

  // Already added rus films.
  try {
    $ru_node_ids = _get_node_ids_by_imdb_ids(array_keys($translations_from_file), 'ru', FALSE);
  } catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
  // Existing rus films ids.
  $isset_ru_imdb_ids = array_keys($ru_node_ids);

  // Filter from undefined items translations from file.
  foreach ($translations_from_file as $imdb_id => $item) {
    if (!in_array($imdb_id, $isset_en_imdb_ids)) {
      drupal_set_message(t('IMDB ID "@id" not found in DB.', ['@id' => $imdb_id]), 'warning');
      unset($translations_from_file[$imdb_id]);
    }
  }

  // Prepared node ids is array [imdb_id => nid].
  $prepared_ids = [];
  $tr_settings = $form_state['values']['tr_settings'];
  switch ($tr_settings) {
    case 'add_new':
      // Filter from already translated films.
      $translations_from_file = array_filter($translations_from_file, function ($key) use ($isset_ru_imdb_ids) {
        return !in_array($key, $isset_ru_imdb_ids);
      }, ARRAY_FILTER_USE_KEY);
      $prepared_ids = $en_node_ids;
      break;

    case 'add_update':
      // If exists rus film keep ru nid, else use eng nid.
      $prepared_ids = $ru_node_ids + $en_node_ids;
      break;
  }

  // Batch items.
  if (!empty($translations_from_file)) {
    $operations = [];
    foreach ($translations_from_file as $imdb_id => $item) {
      $operations[] = [
        'films_parser_batch_translate',
        [
          $prepared_ids[$imdb_id],
          $item,
        ],
      ];
    }

    $batch = [
      'operations' => $operations,
      'finished' => 'films_parser_translate_batch_finished',
    ];
    batch_set($batch);
  }
  else {
    drupal_set_message(t('Already up-to-date.'));
  }
}

/**
 * Validate for load a file with translations.
 */
function films_parser_add_translated_films_validate($form, &$form_state) {
  if (!$form_state['values']['csv']) {
    form_set_error('csv', t('Load csv file.'));
  }
}

/**
 * Clone eng films and save title and description with rus values.
 *
 * @param $node_id
 *  Eng film node id.
 * @param $item
 *  Array with title and description values.
 * @param $context
 *  Array with batch processing values.
 *
 * @throws \Exception
 */
function films_parser_batch_translate($node_id, $item, &$context) {
  $node = node_load($node_id);

  $new_node = new stdClass();
  switch ($node->language) {
    case 'en':
      // Duplicate eng film, reset properties.
      $new_node = clone $node;
      $new_node->is_new = TRUE;
      unset($new_node->nid);
      unset($new_node->vid);
      $new_node->language = 'ru';

      $context['results']['added']++;
      break;

    case 'ru':
      // If it's a rus node update it.
      $new_node = $node;

      $context['results']['updated']++;
      break;
  }

  $new_node->title = $item['title'];
  $new_node->field_description[LANGUAGE_NONE][0]['value'] = $item['description'];

  node_save($new_node);
}

/**
 * Batch finish function.
 *
 * @param $success
 * @param $results
 * @param $operations
 */
function films_parser_translate_batch_finished($success, $results, $operations) {
  if ($success) {
    $added_count = isset($results['added']) ? $results['added'] : 0;
    $updated_count = isset($results['updated']) ? $results['updated'] : 0;

    $added = format_plural(
      $added_count,
      '1 translation was created.',
      '@count translations was created.'
    );
    $updated = format_plural(
      $updated_count,
      '1 translation was updated.',
      '@count translations was updated.'
    );
    drupal_set_message("$added $updated");
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
