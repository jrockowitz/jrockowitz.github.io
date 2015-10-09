---
layout: page
title: README
permalink: /readme/
---

# Jacob Rockowitz / The Big Blue House

## Site Information

- URL: <http://jrockowitz.github.io/>
- GIT: <https://github.com/jrockowitz/jrockowitz.github.io>

--- 

## Overview

### Software/Libraries

- Jeykyll
- Grunt
- Bower
- Bootstrap
- jQuery

## Information Architecture

- About
- Resume
- Work

--- 

## TODO

- Experiment with Bootstrap.
  - style-guide.md 

- Add and remove `environment: development` from _config.yml when grunt serve
  is called.

- Look into [RequireJS: A JavaScript file and module loader.](http://requirejs.org)

- Content
  - Merge About with Experience  
  

## Process/Tasks

- Base Template
  - Setup head.html and script.html
  - Build style-guide of Bootstrap elements
  - 

**GET THE CONTENT INTO JEKYLL**

- Convert all Drupal 7 pages to Jekyll
  - Keep HTML AS-IS
  - Meta
  - Content
  - Slides

- Build basic slideshow
  
- Add collection for case studies.

- Set a blog

**THEN EXPERIMENT WITH TEMPLATING**

- Must get site structure correct and then we can experiment with branches.
  
- Use feature branches to experiment with new templates

--- 

## Setup
  
### 'Hello World' GitHub Pages
  
Setup GitHub Pages respository. 

- [GitHub Pages | Jekyll](http://jekyllrb.com/docs/github-pages/)
  
### Boilerplate Jekyll site
    
Replace 'Hello World' with a boilerplate Jekyll site.

- [Quick-start guide](http://jekyllrb.com/docs/quickstart/)
    - Must remove index.html_ and README.md before executing `jekyll new .` 
      inside `local.jrockowitz.github.io`.
- [Working With Github Pages](https://gist.github.com/jedschneider/2890453)
    - Add _site to .gitignore. The generated site should not be uploaded to Github since its gets generated by github.
     
### Commit and push changes to GitHub

    cd local.jrockowitz.github.io
    git add --all
    git commit -am"Boilerplate Jekyll site"
    git push
    open http://http://jrockowitz.github.io/
    # Refresh browser until GitHub regenerates your website.

### Initial to Jekyll site

- [Jekyll Configuration](http://jekyllrb.com/docs/configuration/)


    # Update Jekyll configuration 
    cd local.jrockowitz.github.io
    edit _config.yml
    
    # Add main.js to handle custom JavaScript behavior. 
    cd local.jrockowitz.github.io
    mkdir js
    echo '// main.js' >> js/main.js

    # Stop site from being crawled before it is completed
    # Add <meta name="robots" content="noindex, nofollow"> to includes/head.html
    cd local.jrockowitz.github.io
    edit includes/head.html
    
### Use NodeJs Package Manager (npm) to install Grunt and Bower.

- [What is npm?](https://docs.npmjs.com/getting-started/what-is-npm)
- [Specifics of npm's package.json handling](https://docs.npmjs.com/files/package.json)
- [Grunt: The JavaScript Task Runner](http://gruntjs.com)
- [Bower: A package manager for the web](http://bower.io)


    # Setup and execute npm install
    cd local.jrockowitz.github.io
    edit package.json
    npm install
    
    # Add node_modules to .gitignore
    cd local.jrockowitz.github.io
    echo 'node_modules' >> ~/.gitignore
   
### Use Bower to download jQuery and Bootstrap SASS.

- [Bower: Getting Started](http://bower.io/#getting-started)
- [jQuery](https://jquery.com)
- [Bootstrap for Sass](https://github.com/twbs/bootstrap-sass)


    cd local.jrockowitz.github.io
    edit bower.json
    bower install
      
    - or -
    
    cd local.jrockowitz.github.io
    bower install jquery
    bower install bootstrap-sass-official

### Setup basic Grunt task runner

- [grunt-contrib-concat](https://github.com/gruntjs/grunt-contrib-concat): Concatenate files.
- [grunt-contrib-uglify](https://github.com/gruntjs/grunt-contrib-uglify): Minify files with UglifyJS
- [grunt-exec](https://github.com/jharding/grunt-exec)


    
    cd local.jrockowitz.github.io
    edit GruntFile.json
    grunt

### Integrate jQuery, Bootstrap, and main.js into Jekylly Boilerplate website.
    
- [Adding Bootstrap to Jekyll](http://stackoverflow.com/questions/28733425/adding-bootstrap-to-jekyll)


    
    # CSS: Add Bootstrap SASS to Jekyll SASS
    cd local.jrockowitz.github.io
    cat >> css/main.scss <<'EOF'
    
    // Import Bootstrap
    @import "../bower_components/bootstrap-sass-official/assets/stylesheets/bootstrap";
    EOF
    
    # JS: Add jQuery and Bootstrap
    cd local.jrockowitz.github.io
    cat >> _includes/footer.html <<'EOF'
    
    {% if site.environment == 'development' %}
      <script src="{{ site.baseurl }}/bower_components/jquery/jquery.min.js"></script>
      <script src="{{ site.baseurl }}/bower_components/bootstrap-sass-official/assets/javascripts/bootstrap.min.js"></script>
      <script src="{{ site.baseurl }}/js/main.js"></script>
    {% else %}
      <script src="{{ site.baseurl }}/assets/js/vendor.min.js"></script>
      <script src="{{ site.baseurl }}/js/main.min.js"></script>
    {% endif %}
    EOF

### Add this README.md file to site.

    open http://jrockowitz.github.io/readme


### Build a style-guide/code snippet page 

- [CSS](http://getbootstrap.com/css/): Global CSS settings, fundamental HTML 
  elements styled and enhanced with extensible classes, and an advanced grid system.

- http://getbootstrap.com/css/#type

### More Information about using Jekyll on GitHub Pages
    
- [Repository metadata on GitHub Pages](https://help.github.com/articles/repository-metadata-on-github-pages/)

  exit;


--- 

## Notes

- Bower components are included in the repository so that GitHub pages, using 
  Jekyll can compile the SASS and build the website. 

- Drafts could be used for a private blog or even /notes

--- 

## References

### [GitHub Pages](https://pages.github.com/)

- [GitHub Pages | Jekyll](http://jekyllrb.com/docs/github-pages/)
- [Using Jekyll with Pages | GitHub](https://help.github.com/articles/using-jekyll-with-pages/)


### [Jekyll](http://jekyllrb.com)

- [Documentation](http://jekyllrb.com/docs/home/)
    - [GitHub Pages](http://jekyllrb.com/docs/github-pages/)
- [Tips](http://jekyll.tips/)
- [Liquid for Designers](https://github.com/shopify/liquid/wiki/Liquid-for-Designers)
- [Pagination](http://jekyllrb.com/docs/pagination/)
- [Photo Gallery](http://jekyll.tips/guide/photo-gallery/)

### [Bootstrap])http://getbootstrap.com/)

- Style Guides
    - <http://getbootstrap.com/css/>
    - <http://bootswatch.com/default/>
    - <http://www.bootply.com/render/65566>

### HTML/Front-end tools

- [HTML Ipsum](http://html-ipsum.com/)

### Placeholders Images

- <http://placehold.it>
    - <http://placehold.it/150x150>
- <http://lorempixel.com/>
    - <http://lorempixel.com/400/200>
- <http://loremflickr.com/>
    - <http://loremflickr.com/320/240>
