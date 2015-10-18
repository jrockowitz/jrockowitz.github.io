/**
 * @file
 * Javascript Behaviors
 */

/* http://jshint.com/docs/ */
/* global enquire */

(function ($, enquire) {

  "use strict";

  /**
   * Breakpoints
   *
   * Copied from: bower_components/bootstrap-sass-official/assets/stylesheets/bootstrap/_variables.scss
   */
  var breakPoints = {
    xs: {width: 480},
    sm: {width: 768},
    md: {width: 992},
    lg: {width: 1200}
  };

  /**
   * Contact Form
   */
  $('#contact').submit(function() {
    // Set the form action using javascript helps obscure my email address
    // and prevent bots from easily submitting the form.
    this.action = '//formspree.io/' + ['jrockowitz', 'gmail.com'].join('@');
    this._next.value = '//' + location.host + '/contact/thanks/';
  });

  /**
   * Carousel
   *
   * - Add swipe event handling to carousel.
   *   http://lazcreative.com/blog/adding-swipe-support-to-bootstrap-carousel-3-0/
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
  function equalHeight() {
    $(['.work .thumbnail-container', '#work-mskcc-drupal-6 .thumbnail-container']).each(function () {
      $(this).css('min-height', '').equalheight();
    })
  }
  $(window).load(function () {
    // Initialize equal heights.
    equalHeight();

    // Set orientation change event handler.
    $(window).on('orientationchange', equalHeight);

    // Reset equal height at each break point. Adding +1 to insure heights are reset after css media queries fire.
    for (var breakPoint in breakPoints) {
      enquire.register('screen and (min-width: ' + (breakPoints[breakPoint].width+1) + 'px)', equalHeight);
    }
  });

  /**
   * Anchors
   */
  if (location.hash && $(location.hash).length) {
    // Smooth scroll to anchors (by id) and account for fixed navbar.
    var topHeight = $(location.hash).offset().top - 70; // 60px (Navbar) + 10px (Padding)
    $('body, html').animate({scrollTop: topHeight}, '500', 'swing');
  }


})(jQuery, enquire);


