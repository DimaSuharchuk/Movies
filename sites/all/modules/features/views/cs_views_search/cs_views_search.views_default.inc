<?php
/**
 * @file
 * cs_views_search.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function cs_views_search_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'cs_views_search';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Search';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Rendered Node */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = '';
  $handler->display->display_options['fields']['rendered_entity']['element_class'] = 'keystroke';
  $handler->display->display_options['fields']['rendered_entity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'teaser';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  /* Sort criterion: Content: IMDB voting (field_imdb) */
  $handler->display->display_options['sorts']['field_imdb_value']['id'] = 'field_imdb_value';
  $handler->display->display_options['sorts']['field_imdb_value']['table'] = 'field_data_field_imdb';
  $handler->display->display_options['sorts']['field_imdb_value']['field'] = 'field_imdb_value';
  $handler->display->display_options['sorts']['field_imdb_value']['order'] = 'DESC';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['expose']['label'] = 'Title';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'film' => 'film',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '12';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['row_class'] = 'row film-teaser-wrapper';
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Rendered Node */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = '';
  $handler->display->display_options['fields']['rendered_entity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'teaser';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'film' => 'film',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'word';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Genres (field_genre) */
  $handler->display->display_options['filters']['field_genre_tid']['id'] = 'field_genre_tid';
  $handler->display->display_options['filters']['field_genre_tid']['table'] = 'field_data_field_genre';
  $handler->display->display_options['filters']['field_genre_tid']['field'] = 'field_genre_tid';
  $handler->display->display_options['filters']['field_genre_tid']['operator'] = 'and';
  $handler->display->display_options['filters']['field_genre_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_genre_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_genre_tid']['expose']['operator_id'] = 'field_genre_tid_op';
  $handler->display->display_options['filters']['field_genre_tid']['expose']['label'] = 'Genres';
  $handler->display->display_options['filters']['field_genre_tid']['expose']['operator'] = 'field_genre_tid_op';
  $handler->display->display_options['filters']['field_genre_tid']['expose']['identifier'] = 'field_genre_tid';
  $handler->display->display_options['filters']['field_genre_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_genre_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_genre_tid']['reduce_duplicates'] = TRUE;
  $handler->display->display_options['filters']['field_genre_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_genre_tid']['vocabulary'] = 'genres';
  /* Filter criterion: Content: Director (field_director) */
  $handler->display->display_options['filters']['field_director_tid']['id'] = 'field_director_tid';
  $handler->display->display_options['filters']['field_director_tid']['table'] = 'field_data_field_director';
  $handler->display->display_options['filters']['field_director_tid']['field'] = 'field_director_tid';
  $handler->display->display_options['filters']['field_director_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_director_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_director_tid']['expose']['operator_id'] = 'field_director_tid_op';
  $handler->display->display_options['filters']['field_director_tid']['expose']['label'] = 'Director';
  $handler->display->display_options['filters']['field_director_tid']['expose']['operator'] = 'field_director_tid_op';
  $handler->display->display_options['filters']['field_director_tid']['expose']['identifier'] = 'field_director_tid';
  $handler->display->display_options['filters']['field_director_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_director_tid']['reduce_duplicates'] = TRUE;
  $handler->display->display_options['filters']['field_director_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_director_tid']['vocabulary'] = 'person';
  /* Filter criterion: Content: Actors (field_actors) */
  $handler->display->display_options['filters']['field_actors_tid']['id'] = 'field_actors_tid';
  $handler->display->display_options['filters']['field_actors_tid']['table'] = 'field_data_field_actors';
  $handler->display->display_options['filters']['field_actors_tid']['field'] = 'field_actors_tid';
  $handler->display->display_options['filters']['field_actors_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_actors_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_actors_tid']['expose']['operator_id'] = 'field_actors_tid_op';
  $handler->display->display_options['filters']['field_actors_tid']['expose']['label'] = 'Actors';
  $handler->display->display_options['filters']['field_actors_tid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_actors_tid']['expose']['operator'] = 'field_actors_tid_op';
  $handler->display->display_options['filters']['field_actors_tid']['expose']['identifier'] = 'field_actors_tid';
  $handler->display->display_options['filters']['field_actors_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_actors_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_actors_tid']['reduce_duplicates'] = TRUE;
  $handler->display->display_options['filters']['field_actors_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_actors_tid']['vocabulary'] = 'person';
  /* Filter criterion: Content: Year (field_year) */
  $handler->display->display_options['filters']['field_year_value_1']['id'] = 'field_year_value_1';
  $handler->display->display_options['filters']['field_year_value_1']['table'] = 'field_data_field_year';
  $handler->display->display_options['filters']['field_year_value_1']['field'] = 'field_year_value';
  $handler->display->display_options['filters']['field_year_value_1']['group'] = 2;
  $handler->display->display_options['filters']['field_year_value_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_year_value_1']['expose']['operator_id'] = 'field_year_value_1_op';
  $handler->display->display_options['filters']['field_year_value_1']['expose']['label'] = 'Single year';
  $handler->display->display_options['filters']['field_year_value_1']['expose']['operator'] = 'field_year_value_1_op';
  $handler->display->display_options['filters']['field_year_value_1']['expose']['identifier'] = 'field_year_value_1';
  $handler->display->display_options['filters']['field_year_value_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Year (field_year) */
  $handler->display->display_options['filters']['field_year_value']['id'] = 'field_year_value';
  $handler->display->display_options['filters']['field_year_value']['table'] = 'field_data_field_year';
  $handler->display->display_options['filters']['field_year_value']['field'] = 'field_year_value';
  $handler->display->display_options['filters']['field_year_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_year_value']['group'] = 2;
  $handler->display->display_options['filters']['field_year_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_year_value']['expose']['operator_id'] = 'field_year_value_op';
  $handler->display->display_options['filters']['field_year_value']['expose']['label'] = 'Year range';
  $handler->display->display_options['filters']['field_year_value']['expose']['operator'] = 'field_year_value_op';
  $handler->display->display_options['filters']['field_year_value']['expose']['identifier'] = 'field_year_value';
  $handler->display->display_options['filters']['field_year_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_year_value']['group_info']['label'] = 'Year';
  $handler->display->display_options['filters']['field_year_value']['group_info']['identifier'] = 'field_year_value';
  $handler->display->display_options['filters']['field_year_value']['group_info']['widget'] = 'radios';
  $handler->display->display_options['filters']['field_year_value']['group_info']['default_group'] = '1';
  $handler->display->display_options['filters']['field_year_value']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Single year',
      'operator' => '=',
      'value' => array(
        'value' => '0',
        'min' => '',
        'max' => '',
      ),
    ),
    2 => array(
      'title' => 'Year range',
      'operator' => 'between',
      'value' => array(
        'value' => '',
        'min' => '0',
        'max' => '0',
      ),
    ),
  );
  $handler->display->display_options['path'] = 'search';
  $export['cs_views_search'] = $view;

  return $export;
}
