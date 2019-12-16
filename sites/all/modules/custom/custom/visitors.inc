<?php

/**
 * @file
 * Visitors alters and additional functionality.
 */

/**
 * Visitors page form.
 *
 * {@inheritDoc}
 *
 * @throws \Exception
 */
function custom_people_visitors_form($form, &$form_state) {
  $now = new DateTime();

  $form['date'] = [
    '#title' => t('Select month to view statistics'),
    '#type' => 'date',
    '#date_format' => 'Y-m',
    '#default_value' => [
      'year' => $now->format('Y'),
      'month' => $now->format('m'),
      'day' => $now->format('d'),
    ],
    '#after_build' => ['_date_field_alter'],
  ];

  $form['actions'] = ['#type' => 'actions'];
  $form['actions']['submit'] = [
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#ajax' => [
      'callback' => 'custom_people_visitors_form_ajax_callback',
      'wrapper' => 'custom-content',
    ],
  ];

  $form['content'] = [
    '#type' => 'container',
    '#attributes' => [
      'id' => 'custom-content',
    ],
    '#weight' => 100,
  ];

  $form['#attached']['css'][] = drupal_get_path('module', 'custom') . '/css/custom.css';

  return $form;
}

/**
 * Alter date field.
 *
 * {@inheritDoc}
 */
function _date_field_alter($element, $form_state) {
  $now = new DateTime();
  $currentYear = $now->format('Y');
  $years = [$currentYear - 1, $currentYear];
  $element['year']['#options'] = array_combine($years, $years);

  return $element;
}

/**
 * Show tables of visitors via ajax.
 *
 * {@inheritDoc}
 *
 * @throws \Exception
 */
function custom_people_visitors_form_ajax_callback($form, &$form_state) {
  // Remove records of my PC (Triolan).
  db_delete('visitors')
    ->condition('visitors_ip', '2996188432')
    ->execute();

  $dateString = sprintf('%d-%d-%d', $form_state['input']['date']['year'], $form_state['input']['date']['month'], 1);
  $startTime = new DateTime($dateString);
  $endTime = (clone $startTime)->modify('last day of')->setTime(23, 59, 59);

  $commands = [];
  $commands[] = ajax_command_html('#custom-content', custom_people_visitors($startTime, $endTime));
  return ['#type' => 'ajax', '#commands' => $commands];
}

/**
 * Function builds content for page.
 *
 * @param \DateTime $startTime
 *   Date "from" for selecting from DB.
 * @param \DateTime $endTime
 *   Date "to" for selecting from DB.
 *
 * @return string
 *   Rendered content to print on page.
 * @throws \Exception
 */
function custom_people_visitors(DateTime $startTime, DateTime $endTime) {
  $pageContent = '';

  $results = db_select('visitors', 'v')
    ->fields('v', ['visitors_ip', 'visitors_date_time', 'visitors_user_agent'])
    ->condition('visitors_date_time', [
      $startTime->getTimestamp(),
      $endTime->getTimestamp(),
    ], 'BETWEEN')
    ->condition('visitors_user_agent', '%bot%', 'NOT LIKE')
    ->orderBy('visitors_date_time', 'DESC')
    ->execute()
    ->fetchAll();

  $resultsGroupedByDay = [];
  foreach ($results as $result) {
    $day = date('d M Y', $result->visitors_date_time);
    $resultsGroupedByDay[$day][] = $result;
  }

  $header = [
    'Date',
    'IP',
    'Flag',
    'Country',
    'Region',
    'City',
    'User agent',
  ];
  $monthRecordsCount = 0;
  $monthUniqueUsersCount = 0;
  foreach ($resultsGroupedByDay as $day => $array) {
    $rows = [];
    $unique = [];
    foreach ($array as $item) {
      $ip = long2ip($item->visitors_ip);
      $address = get_country_by_ip($ip);

      $rows[] = [
        date('d M Y H:i:s', $item->visitors_date_time),
        $ip,
        theme('image', ['path' => $address['country_flag'], 'width' => 50]),
        $address['country_name'],
        $address['region_name'],
        $address['city'],
        $item->visitors_user_agent,
      ];

      $unique[$item->visitors_ip] = '';
    }

    $recordsCount = count($array);
    $monthRecordsCount += $recordsCount;
    $uniqueUsersCount = count($unique);
    $monthUniqueUsersCount += $uniqueUsersCount;

    $footer = [
      [
        [
          'data' => t('Records count: %rc', [
            '%rc' => $recordsCount,
          ]),
          'colspan' => 7,
        ],
      ],
      [
        [
          'data' => t('Unique: %un', [
            '%un' => $uniqueUsersCount,
          ]),
          'colspan' => 7,
        ],
      ],
    ];

    $pageContent .= theme('table', [
      'caption' => $day,
      'header' => $header,
      'rows' => $rows,
      'sticky' => FALSE,
      'attributes' => [
        'style' => 'width: 100%',
      ],
      'footer' => $footer,
    ]);
  }

  $headerMessage = [
    '#type' => 'container',
    '#attributes' => [
      'class' => [
        'header-message',
      ],
    ],
  ];
  $headerMessage[] = [
    '#markup' => t('Monthly records count: %rc. Monthly unique users: %u', [
      '%rc' => $monthRecordsCount,
      '%u' => $monthUniqueUsersCount,
    ]),
  ];
  $pageContent = drupal_render($headerMessage) . $pageContent;

  return $pageContent;
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * {@inheritDoc}
 */
function custom_form_films_parser_admin_settings_alter(&$form, &$form_state) {
  $form['ipstack_api_key'] = [
    '#type' => 'textfield',
    '#title' => t('IpStack API key'),
    '#default_value' => variable_get('ipstack_api_key'),
  ];
}

/**
 * Get address info by IP.
 *
 * @param $ip
 *   IP address for search.
 * @param bool $short
 *   Short info about address, like country, region and city.
 *
 * @return array|bool|mixed
 */
function get_country_by_ip($ip, $short = TRUE) {
  if ($ipStackApiKey = variable_get('ipstack_api_key')) {
    $cid = "ipstack_{$ip}_data";

    if ($cached = cache_get($cid)) {
      $data = $cached->data;
    }
    else {
      $response = drupal_http_request(
        url(
          "http://api.ipstack.com/{$ip}",
          ['query' => ['access_key' => $ipStackApiKey]]
        )
      );

      $data = json_decode($response->data);

      cache_set($cid, $data);
    }

    if ($short) {
      return [
        'country_name' => $data->country_name ?? '',
        'region_name' => $data->region_name ?? '',
        'city' => $data->city ?? '',
        'country_flag' => $data->location->country_flag ?? '',
      ];
    }
    return $data;
  }

  return FALSE;
}
