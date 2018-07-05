<?php
/**
 * @file
 * Batch functionality for fetch films from OMDB.
 */

/**
 * Film parser batch form submit.
 */
function films_parser_fetch_films_from_omdb($form, &$form_state) {
  $operations = [];
  $list_ids = [];
  // Get imdb ids from input.
  $input = $form_state['values']['imdb_ids'];

  preg_match_all("@tt\d{7}@", $input, $list_ids);
  $list_ids = reset($list_ids);

  // Select ids of all already created films.
  $db_ids = db_select('field_data_field_imdb_id', 'i')
    ->fields('i', ['field_imdb_id_value'])
    ->execute()
    ->fetchCol();

  // Check for unique values and keep new ids.
  $new_ids = array_diff($list_ids, $db_ids);

  if (!empty($new_ids)) {
    foreach ($new_ids as $i => $id) {
      $operations[] = [
        'films_parser_batch',
        [
          $id,
        ],
      ];
    }

    $batch = [
      'operations' => $operations,
      'finished' => 'films_parser_batch_finished',
    ];

    batch_set($batch);
  }
  else {
    drupal_set_message(t('All films already added.'));
  }
}

/**
 * Batch operations.
 * Fetch JSON from IMDB and create films.
 */
function films_parser_batch($id, &$context) {
  $json = _fetch_json_from_omdb($id);
  _create_film($json);

  if (!isset($context['results']['count'])) {
    $context['results']['count'] = 0;
  }
  $context['results']['count']++;
}

/**
 * Batch finish function.
 *
 * @param $success
 * @param $results
 * @param $operations
 */
function films_parser_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(
      format_plural(
        $results['count'],
        '@count film was created.',
        '@count films was created.'
      )
    );
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
