(function ($) {
  Drupal.behaviors.copyPreparedText = {
    attach: function () {
      $('#copy-prepared-text').on('click', function () {
        $('#prepared-text-field').select();
        document.execCommand('copy');
        $('.copied').show().animate({
          opacity: 0
        }, 1000);

        return false;
      });
    }
  };

  // Auto submit for "get name/year" field.
  Drupal.behaviors.getNameYearStringAjaxAutosubmit = {
    attach: function () {
      $('#imdb-id').on('paste', function () {
        $('input[name="prepare_string"], button[name="prepare_string"]').mousedown();
      });
    }
  };

  // Auto submit for "get trailers" field.
  Drupal.behaviors.trailerAjaxAutosubmit = {
    attach: function (context) {
      $('#trailer-input', context).on('paste', function () {
        $('input[name="trailer_button"], button[name="trailer_button"]').mousedown();
      });

      // Auto redirect.
      if ($('#trailer-search-exceptions', context).find('a').length) {
        $('#trailer-search-exceptions').find('a').each(function () {
          window.open($(this).attr('href'), '_blank')
        });
      }
    }
  };
})(jQuery);