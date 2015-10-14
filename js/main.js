/**
 * @file
 * Javascript Behaviors
 */

(function ($) {

  "use strict";

  /**
   * Set contact form action to formspree.io and the _next to the thankyou page.
   */
  $('#contact').onsubmit(function() {
    // Set the form action using javascript helps obscure my email address
    // and prevent bots from easily submitting the form.
    this.action = '//formspree.io/' + ['jrockowitz', 'gmail.com'].join('@');
    this._next.value = '//' + location.host + '/contact/thanks/';
  });

})(jQuery);


