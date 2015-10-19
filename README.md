---
layout: page
icon: fa fa-cogs
title: About Website (README.md)
permalink: /readme/
---

## Site Information

- URL: <http://jrockowitz.github.io/> or <http://thebigbluehouse.com/>
- GIT: <https://github.com/jrockowitz/jrockowitz.github.io>
- Style-Guide: [http://jrockowitz.github.io/style-guide/](/style-guide/)

--- 

### Software/Libraries

- [Jeykyll](https://jekyllrb.com/)
    - [Jekyll Redirect From Plugin](https://github.com/jekyll/jekyll-redirect-from)
    - [Jekyll Sitemap Generator Plugin](https://github.com/jekyll/jekyll-sitemap)
- [github / pages-gem](https://github.com/github/pages-gem)
- [Grunt](http://gruntjs.com/)
    - [grunt-contrib-concat](https://github.com/gruntjs/grunt-contrib-concat)
    - [grunt-contrib-copy](https://github.com/gruntjs/grunt-contrib-copy)
    - [grunt-contrib-csslint](https://github.com/gruntjs/grunt-contrib-csslint)
    - [grunt-contrib-cssmin](https://github.com/gruntjs/grunt-contrib-cssmin)
    - [grunt-contrib-jshint](https://github.com/gruntjs/grunt-contrib-jshint)
    - [grunt-contrib-uglify](https://github.com/gruntjs/grunt-contrib-uglify)
    - [grunt-exec](https://github.com/jharding/grunt-exec)
    - [grunt-responsive-images](https://github.com/andismith/grunt-responsive-images)
    - [grunt-sed](https://github.com/jharding/grunt-sed)
- [Bower](http://bower.io/)
- [Bootstrap](http://getbootstrap.com/)
- [enquire.js](wicky.nillia.ms/enquire.js/)
- [Font Awesome](https://fortawesome.github.io/Font-Awesome/)
- [jQuery](https://jquery.com/)
- [jQuery Mobile](https://jquerymobile.com/)
  - [Events](https://api.jquerymobile.com/category/events/): Touch & Orientation

--- 

## Setup
  
### 'Hello World' GitHub Pages
  
Setup GitHub Pages respository. 

- [GitHub Pages: Jekyll](http://jekyllrb.com/docs/github-pages/)
  
### Boilerplate Jekyll site
    
Replace 'Hello World' with a boilerplate Jekyll site.

- [Quick-start guide](http://jekyllrb.com/docs/quickstart/)
    - Must remove index.html_ and README.md before executing `jekyll new .` 
      inside `local.jrockowitz.github.io`.
- [Working With Github Pages](https://gist.github.com/jedschneider/2890453)
    - Add _site to .gitignore because Github maintains (and overwrites) this directory.
     
### Commit and push changes to GitHub

~~~
cd local.jrockowitz.github.io
git add --all
git commit -am"Boilerplate Jekyll site"
git push
open http://http://jrockowitz.github.io/
# Refresh browser until GitHub regenerates your website.
~~~

### Initial to Jekyll site

- [Jekyll Configuration](http://jekyllrb.com/docs/configuration/)
  - [Organizing Jekyll Pages](http://damonbauer.me/organizing-jekyll-pages/)


~~~
cd local.jrockowitz.github.io
edit _config.yml
~~~
    
### Use NodeJs Package Manager (npm) to install Grunt and Bower.

- [What is npm?](https://docs.npmjs.com/getting-started/what-is-npm)
- [Specifics of npm's package.json handling](https://docs.npmjs.com/files/package.json)
- [Grunt: The JavaScript Task Runner](http://gruntjs.com)
- [Bower: A package manager for the web](http://bower.io)


~~~
# Setup and execute npm install
cd local.jrockowitz.github.io
edit package.json
npm install

# Add node_modules to .gitignore
cd local.jrockowitz.github.io
echo 'node_modules' >> ~/.gitignore
~~~
   
### Use Bower to download jQuery and Bootstrap SASS.

- [Bower: Getting Started](http://bower.io/#getting-started)
- [jQuery](https://jquery.com)
- [Bootstrap for Sass](https://github.com/twbs/bootstrap-sass)


~~~
cd local.jrockowitz.github.io
edit bower.json
bower install
  
- or -

cd local.jrockowitz.github.io
bower install jquery
bower install bootstrap-sass-official
~~~

### Setup basic Grunt task runner

- [grunt-contrib-concat](https://github.com/gruntjs/grunt-contrib-concat): Concatenate files.
- [grunt-contrib-uglify](https://github.com/gruntjs/grunt-contrib-uglify): Minify files with UglifyJS
- [grunt-exec](https://github.com/jharding/grunt-exec)
    

~~~
cd local.jrockowitz.github.io
edit GruntFile.json
grunt
~~~

### Remove Jekyll's Boilerplate SASS and HTML.

- Clear _sass/base.scss and _sass/layout.scss.

- Replace `%vertical-rhythm` mixin with `margin-bottom: .5em;` in  _syntax-highlighting.scss

### Integrate jQuery, Bootstrap, and main.js into Jekylly Boilerplate website.
    
- [Adding Bootstrap to Jekyll](http://stackoverflow.com/questions/28733425/adding-bootstrap-to-jekyll)

### Add this README.md file to site.

~~~
open http://jrockowitz.github.io/readme
~~~

### Build a style-guide/code snippet page 

- [Bootstrap: CSS](http://getbootstrap.com/css/)  
  Global CSS settings, fundamental HTML elements styled and enhanced with 
  extensible classes, and an advanced grid system.

### Setup Bootstrap Navbar

- [Bootstrap: Navbar](http://getbootstrap.com/components/#navbar)
    - [Bootstrap Case: Adding a Navigation Bar](http://www.w3schools.com/bootstrap/bootstrap_case_navigation.asp)

### Setup Bootstrap Grid

- [Bootstrap: Grid system](http://getbootstrap.com/css/#grid)
    - [Bootstrap Grids](http://www.w3schools.com/bootstrap/bootstrap_grid_basic.asp)

### Setup Navigation using Jekyll's Data File and Collections

- [Jekyll: Data files](http://jekyllrb.com/docs/datafiles/)
    - [How to create data-driven navigation in Jekyll](http://www.tournemille.com/blog/How-to-create-data-driven-navigation-in-Jekyll/)

- [ Jekyll: Collections](http://jekyllrb.com/docs/collections/)
    - [How to change the default order pages in jekyll](http://stackoverflow.com/questions/13266369/how-to-change-the-default-order-pages-in-jekyll)

### Set Contact Form using Formspree

- [You do not need a database for your contact form](http://sebastien.saunier.me/blog/2014/04/15/you-do-not-need-a-database-for-your-contact-form.html)
    - [Formspree](formspree.io)  
      Just send your form to our URL and we'll forward it to your email.

### Migrate Content into Jekyll

- Write custom migration from Drupal 7 to Jekylly using Drush 
  
### Setup Bootstrap Carousel

- [Adding swipe support to Bootstrap Carousel 3.0](http://lazcreative.com/blog/adding-swipe-support-to-bootstrap-carousel-3-0/)

### Generate Thumbnails

- [Grunt Responsive Images](http://www.andismith.com/grunt-responsive-images/)

### Create Image Enlarge Modal

- [Bootstrap open image in modal](http://stackoverflow.com/questions/25023199/bootstrap-open-image-in-modal)

### Responsive Grid for Image Thumbnails

- [Bootstrap Grid System](http://www.tutorialrepublic.com/twitter-bootstrap-tutorial/bootstrap-grid-system.php)

### Adjust Styles using Variables and CSS overrides

- <https://github.com/twbs/bootstrap-sass/blob/master/assets/stylesheets/bootstrap/_variables.scss>

### SEO

- [Sitemaps for GitHub Pages](https://help.github.com/articles/sitemaps-for-github-pages/)
- [Redirects on GitHub Pages](https://help.github.com/articles/redirects-on-github-pages/)
- [How to add Google Analytics Tracking ID to GitHub Pages](stackoverflow.com/questions/17207458/how-to-add-google-analytics-tracking-id-to-github-pages)

--- 

## References

### [GitHub Pages](https://pages.github.com/)

- [Jekyll: GitHub Pages](http://jekyllrb.com/docs/github-pages/)
- [GitHub: Using Jekyll with Pages](https://help.github.com/articles/using-jekyll-with-pages/)

### [Jekyll](http://jekyllrb.com)

- [Documentation](http://jekyllrb.com/docs/home/)
    - [GitHub Pages](http://jekyllrb.com/docs/github-pages/)
- [Tips](http://jekyll.tips/)
- [Pagination](http://jekyllrb.com/docs/pagination/)
- [Photo Gallery](http://jekyll.tips/guide/photo-gallery/)

### [Liquid for Designers](https://github.com/shopify/liquid/wiki/Liquid-for-Designers)

- [Printing / Echoing / Dumping Arrays : liquid](https://github.com/Shopify/liquid/issues/121)

### [Bootstrap](http://getbootstrap.com/)

- Style Guides
    - <http://getbootstrap.com/css/>
    - <http://bootswatch.com/default/>
    - <http://www.bootply.com/render/65566>

- Tutorials
   - [Bootstrap 3 Tutorial](http://www.w3schools.com/bootstrap/default.asp)
   - [Bootstrap Tutorial : Tutorial Republic](http://www.tutorialrepublic.com/twitter-bootstrap-tutorial/)
   
### HTML/Front-end tools

- [HTML Ipsum](http://html-ipsum.com/)

### Placeholders Images

- <http://placehold.it>
    - <http://placehold.it/150x150>
- <http://lorempixel.com/>
    - <http://lorempixel.com/400/200>
- <http://loremflickr.com/>
    - <http://loremflickr.com/320/240>

### Coding Guidelines/Standards

- [Code guidelines: Bootstrap](https://github.com/twbs/bootstrap/blob/master/CONTRIBUTING.md#code-guidelines)
- [Code Guide (HTML and CSS)](http://codeguide.co)

---  

## TODO

- Editorial cleanup
