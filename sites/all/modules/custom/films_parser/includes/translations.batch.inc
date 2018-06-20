<?php
/**
 * @file
 * Batch functionality for create translated nodes.
 */

/**
 * Film add translations form submit.
 */
function films_parser_add_translated_films($form, &$form_state) {
  if (!empty($form_state['values']['csv'])) {
    $file = file_load($form_state['values']['csv']);
    $content = file_get_contents($file->uri);
    $csv_strings = explode("\n", $content);

    $translations_from_file = [];
    foreach ($csv_strings as $string) {
      list($imdb_id, $title, $description) = str_getcsv($string);
      $translations_from_file[$imdb_id] = [
        'title' => $title,
        'description' => $description,
      ];
    }

    // Select all eng films to accordance with ids from file.
    $query = db_select('node', 'n');
    $query->innerJoin('field_data_field_imdb_id', 'id', 'n.nid = id.entity_id');
    $query
      ->fields('id', ['field_imdb_id_value'])
      ->fields('n', ['nid'])
      ->condition('id.field_imdb_id_value', array_keys($translations_from_file), 'in')
      ->condition('n.language', 'en');
    $en_node_ids = $query->execute()->fetchAllKeyed(0, 1);
    // Existing eng films ids.
    $isset_en_ids = array_keys($en_node_ids);

    $tr_settings = $form_state['values']['tr_settings'];
    switch ($tr_settings) {
      case 'add_new':
        // Already added rus films.
        $query = db_select('node', 'n');
        $query->innerJoin('field_data_field_imdb_id', 'id', 'n.nid = id.entity_id');
        $query
          ->fields('id', ['field_imdb_id_value'])
          ->fields('n', ['nid'])
          ->condition('id.field_imdb_id_value', array_keys($translations_from_file), 'in')
          ->condition('n.language', 'ru');
        $ru_node_ids = $query->execute()->fetchAllKeyed(0, 1);
        $isset_ru_ids = array_keys($ru_node_ids);

        // Filter from undefined items translations from file.
        foreach ($translations_from_file as $imdb_id => $item) {
          if (!in_array($imdb_id, $isset_en_ids)) {
            drupal_set_message(t('@id not found in DB.', ['@id' => $imdb_id]), 'warning');
            unset($translations_from_file[$imdb_id]);
          }
        }
        // Filter from already translated films.
        $translations_from_file = array_filter($translations_from_file, function ($key) use ($isset_ru_ids) {
          return !in_array($key, $isset_ru_ids);
        }, ARRAY_FILTER_USE_KEY);

        // Batch items.
        if (!empty($translations_from_file)) {
          $operations = [];
          foreach ($translations_from_file as $imdb_id => $item) {
            $operations[] = [
              'films_parser_batch_translate',
              [
                $en_node_ids[$imdb_id],
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
        break;

      case 'replace':
        break;
    }
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
  $new = clone node_load($node_id);
  $new->is_new = TRUE;
  unset($new->nid);
  unset($new->vid);
  $new->language = 'ru';
  $new->title = $item['title'];
  $new->field_description[LANGUAGE_NONE][0]['value'] = $item['description'];
  node_save($new);

  $context['results']['added']++;
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
