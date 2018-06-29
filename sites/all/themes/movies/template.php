<?php

/**
 * Implements template_preprocess_html().
 */
function movies_preprocess_html(&$variables) {
}

/**
 * Implements template_preprocess_page.
 */
function movies_preprocess_page(&$variables) {
}

/**
 * Implements template_preprocess_node.
 */
function movies_preprocess_node(&$variables) {
  if (isset($variables['type']) && $variables['type'] == 'film') {
    switch ($variables['view_mode']) {
      case 'teaser':
        // Hide regular teaser title link.
        $variables['page'] = TRUE;

        // Remove default node teaser's "Read more" link.
        unset($variables['content']['links']['node']['#links']['node-readmore']);

        // Set custom link to content as title + year.
        $year = $variables['field_year'][LANGUAGE_NONE][0]['value'];
        $variables['content']['title'] = [
          '#markup' => "$variables[title] ($year)",
          '#prefix' => '<div class="film-title-link">',
          '#suffix' => '</div>',
        ];
        break;

      case 'popup':
        // Hide regular teaser title link.
        $variables['page'] = TRUE;

        $title = $variables['title'];

        // Display title with original title if chosen not english language.
        global $language;
        if ($language->language !== 'en') {
          // Get original movie title.
          $imdb_id = $variables['field_imdb_id'][LANGUAGE_NONE][0]['value'];
          $query = db_select('node', 'n');
          $query->innerJoin('field_data_field_imdb_id', 'id', 'n.nid = id.entity_id');
          $query
            ->fields('n', ['title'])
            ->condition('id.field_imdb_id_value', $imdb_id)
            ->condition('n.language', 'en');
          $en_title = $query->execute()->fetchObject();

          $title = $variables['title'] . ' / ' . $en_title->title;
        }

        $variables['content']['title'] = [
          '#markup' => $title,
          '#prefix' => '<h3 class="popup-film-title">',
          '#suffix' => '</h3>',
          '#weight' => -1,
        ];
        break;
    }
  }
}

/**
 * Implements template_preprocess_field.
 */
function movies_preprocess_field(&$variables) {
  // Translate field label.
  if (isset($variables['label'])) {
    $variables['label'] = t($variables['label']);
  }

  if (isset($variables['element'])) {
    $element = &$variables['element'];
    if (isset($element['#bundle']) && $element['#bundle'] == 'film') {
      // Add classes to every field.
      switch ($element['#view_mode']) {
        case 'full':
          $variables['classes_array'][] = 'large-8';
          $variables['classes_array'][] = 'column';
          break;
      }
    }

    // Correct field "Runtime".
    if ($element['#title'] == 'Runtime') {
      preg_match('/\d+/', $variables['items'][0]['#markup'], $matches);
      $variables['items'][0]['#markup'] = format_plural($matches[0], '1 minute', '@count minutes');
    }
  }
}

/**
 * Implements theme_field__field_type().
 */
function movies_field__taxonomy_term_reference($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<span class="field-label">' . t($variables['label']) . ': </span>';
  }

  // Render the items.
  $output .= ($variables['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($variables['items'] as $delta => $item) {
    // Decode quotes from html entity to symbol.
    $item = html_entity_decode($item['#markup'], ENT_QUOTES);

    $output .= "<li class='taxonomy-term-reference-{$delta}' {$variables['item_attributes'][$delta]}>";
    $output .= t($item);
    $output .= '</li>';
  }
  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . (!in_array('clearfix', $variables['classes_array']) ? ' clearfix' : '') . '">' . $output . '</div>';

  return $output;
}
