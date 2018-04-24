(function ($) {
  /**
   * @return {string}
   */
  Drupal.theme.prototype.filmTheme = function () {
    return '<div id="ctools-modal" class="popups-box film-popup">' +
        '     <div class="ctools-modal-content ctools-modal-film-popup">' +
        '       <div class="modal-scroll content-wrapper">' +
        '         <div class="nano">' +
        '           <div id="modal-content" class="modal-content group-body nano-content"></div>' +
        '         </div>' +
        '       </div>' +
        '     </div>' +
        '   </div>';
  };

  Drupal.behaviors.addModalCloseIcon = {
    attach: function () {
      $('#modalBackdrop').html('<i class="material-icons popup-close">close</i>');
    }
  };

  Drupal.behaviors.alignPopup = {
    attach: function () {
      if ($('#modalContent').length) {
        setTimeout(function () {
          modalContentResize();
          $('#modalBackdrop').css({height: '100%', width: '100%'});
        }, 0)
      }
    }
  };

  Drupal.behaviors.closePopup = {
    attach: function () {
      $('#modalBackdrop').bind('click', function () {
        modalContentClose()
      })
    }
  }
})(jQuery);