/**
 * @file
 * Javascript Behaviors
 */

(function ($) {

  "use strict";

  /**
   * Set contact form action to formspree.io and the _next to the thankyou page.
   */
  $('#contact').submit(function() {
    // Set the form action using javascript helps obscure my email address
    // and prevent bots from easily submitting the form.
    this.action = '//formspree.io/' + ['jrockowitz', 'gmail.com'].join('@');
    this._next.value = '//' + location.host + '/contact/thanks/';
  });

  /**
   * Add swipe event handling to carousel.
   *
   * http://lazcreative.com/blog/adding-swipe-support-to-bootstrap-carousel-3-0/
   */
  $("#carousel").swiperight(function() {
    $(this).carousel('prev');
  });
  $("#carousel").swipeleft(function() {
    $(this).carousel('next');
  });

  /**
   * Image Enlarge
   *
   * Inspired by: http://stackoverflow.com/questions/25023199/bootstrap-open-image-in-modal
   */
  $('a[href$=".png"], a[href$=".jpg"]').click(function(event) {
    var $img = $(this).find('img');
    var title = $(this).attr('title') || ($img.length) ? ($img.attr('title') || $img.attr('alt')) : '';
    $('#modal-image img').attr('src', $(this).attr('href'));
    $('#modal-image h4').html(title);
    $('#modal-image').modal('show');
    event.preventDefault();
  });

  /**
   * Thumbnails
   */
  $(window).load(function() {

    $('.work .thumbnail').equalheight();
  });

})(jQuery);


