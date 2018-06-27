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
})(jQuery);