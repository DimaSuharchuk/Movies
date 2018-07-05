<?php
/**
 * @file
 * Batch functionality for create translated taxonomy directors and actors.
 */

/**
 * Callback for "Translate persons" submit.
 * Create batch with persons name.
 *
 * @param $form
 * @param $form_state
 */
function films_parser_translate_persons($form, &$form_state) {
  $persons = array_slice($form_state['values']['persons'], 0, 100);

  $operations = [];

  foreach ($persons as $person) {
    $operations[] = ['translate_person_batch', [$person]];
  }

  $batch = [
    'operations' => $operations,
    'finished' => 'translate_person_batch_finished',
  ];

  batch_set($batch);
}

/**
 * Batch for translate persons.
 * Create translation and save to DB.
 *
 * @param $person
 *  Person eng name.
 * @param $context
 *  Context for count.
 */
function translate_person_batch($person, &$context) {
  $name = _get_person_rus($person);

  // Save to DB persons translations.
  if ($name) {
    $report = [
      'skips' => 0,
      'updates' => 0,
      'deletes' => 0,
      'additions' => 0,
    ];
    _locale_import_one_string_db($report, 'ru', '', $person, $name, 'default', '', LOCALE_IMPORT_KEEP);

    $context['results']['names'][$person] = $name;
    $context['results']['count']++;
  }
}

/**
 * Person translating finish.
 *
 * @param $success
 * @param $results
 * @param $operations
 */
function translate_person_batch_finished($success, $results, $operations) {
  if ($success) {
    $added_count = isset($results['count']) ? $results['count'] : 0;

    drupal_set_message(
      format_plural(
        $added_count,
        '@count person translated.',
        '@count persons translated.'
      )
    );

    $prepared_for_copy = [];
    foreach ($results['names'] as $en => $ru) {
      $prepared_for_copy[] = 'msgid "' . $en . '"<br>msgstr "' . $ru . '"';
    }
    drupal_set_message(implode('<br>', $prepared_for_copy));

    // Clear locale cache.
    cache_clear_all('locale:', 'cache', TRUE);
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
