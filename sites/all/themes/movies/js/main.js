(function ($) {
  // Add extra classes to field "Year range".
  Drupal.behaviors.exposedSearch = {
    attach: function (context) {
      $('.views-widget-filter-field_year_value', context)
          .find('.form-type-textfield')
          .addClass('large-6 medium-6 columns')
    }
  };

  // Button for hide exposed filters for mobile.
  Drupal.behaviors.collapsedButton = {
    attach: function (context) {
      $('.filters-collapsed-button', context).bind('click', function (context) {
        $('.block-views-exp-cs-views-search-page').fadeToggle();
      })
    }
  };

  // Get random movie.
  Drupal.behaviors.getRandomMovie = {
    attach: function (context) {
      $('.get-random-button', context).bind('click', function (context) {
        $('.view-content').load(Drupal.settings.basePath + 'ajax/node/random');
      });
    }
  };
})(jQuery);