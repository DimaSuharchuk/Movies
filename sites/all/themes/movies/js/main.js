(function ($) {
  // Initialize nanoScroller.
  Drupal.behaviors.nanoScrollerInit = {
    attach: function () {
      $('.nano').nanoScroller();
    }
  };

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
      $('.filters-collapsed-button', context).on('click', function () {
        $('.block-views-exp-cs-views-search-page').toggle();

        return false;
      });

      // Toggle search button hidden for mobiles. Hide exposed filters too.
      if (window.innerWidth <= 641) {
        if (!$('.filters-collapsed-button').is(':visible')) {
          $('.block-views-exp-cs-views-search-page').hide();
        }
      }
    }
  };
})(jQuery);