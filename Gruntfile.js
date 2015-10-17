module.exports = function(grunt) {

  grunt.initConfig({
    // Get project configuration from package.json.
    pkg: require('./package.json'),
    concat: {
      js: {
        src: [
          'bower_components/jquery/jquery.js',
          'bower_components/bootstrap-sass-official/assets/javascripts/bootstrap.js',
          'bower_components/jquery.equal-height/jquery.equal-height.js',
          'bower_components/enquire/dist/enquire.js',
          'vendor/jquery.mobile.custom/jquery.mobile.custom.js'
        ],
        dest: 'assets/js/vendor.js'
      },
      css: {
        src: [
          'bower_components/font-awesome/css/font-awesome.css',
        ],
        dest: 'assets/css/vendor.css'
      }

    },
    copy: {
      "bootstrap-fonts": {
        files: [
          {
            expand: true,
            cwd: 'bower_components/bootstrap-sass-official/assets/fonts',
            src: ['**', '.**'],
            dest: 'fonts'
          }
        ]
      },
      "font-awesome-fonts": {
        files: [
          {
            expand: true,
            cwd: 'bower_components/font-awesome/fonts',
            src: ['**', '.**'],
            dest: 'assets/fonts'
          }
        ]
      }
    },
    cssmin: {
      dist: {
        files: {
          'assets/css/vendor.min.css': [
            'assets/css/vendor.css'
          ],
          'css/main.min.css': [
            '_site/css/main.css'
          ]
        }
      }
    },
    uglify: {
      dist: {
        files: {
          'assets/js/vendor.min.js': 'assets/js/vendor.js',
          'js/main.min.js': 'js/main.js'
        }
      }
    },
    responsive_images: {
      thumbnails: {
        options: {
          sizes: [
            {
              rename: false,
              width: 640,
              height: 480,
              gravity: 'North',
              aspectRatio: false,
              sharpen: {
                sigma: 1,
                radius: 2
              }
            }
          ]
        },
        files: [{
          expand: true,
          src: ['**/*.{jpg,gif,png}'],
          cwd: 'images/',
          dest: 'images-thumbnails/'
        }]
      }
    },
    sed: {
      dev: {
        path: '_config.yml',
        pattern: 'environment: production',
        replacement: 'environment: development'
      },
      prod: {
        path: '_config.yml',
        pattern: 'environment: development',
        replacement: 'environment: production'
      }
    },
    exec: {
      build: {
        cmd: 'jekyll build'
      },
      serve: {
        // http://unix.stackexchange.com/questions/102956/how-to-run-a-command-in-the-background-with-a-delay
        cmd: '(sleep 5; open http://localhost:4000) &\njekyll serve --drafts --watch'
      },
      deploy: {
        cmd: 'git add --all; git commit -am"Deploying changes to GitHub"; git push;'
      }
    }
  });

  grunt.loadNpmTasks('grunt-contrib-concat');
  grunt.loadNpmTasks('grunt-contrib-copy');
  grunt.loadNpmTasks('grunt-contrib-cssmin');
  grunt.loadNpmTasks('grunt-contrib-uglify');
  grunt.loadNpmTasks('grunt-exec');
  grunt.loadNpmTasks('grunt-responsive-images');
  grunt.loadNpmTasks('grunt-sed');

  grunt.registerTask('build', [ 'copy', 'concat', 'cssmin', 'uglify', 'responsive_images', 'sed:dev']);
  grunt.registerTask('default', [ 'build', 'exec:build' ]);
  grunt.registerTask('serve', [ 'build', 'exec:serve' ]);
  grunt.registerTask('deploy', [ 'default', 'sed:prod', 'exec:deploy' ]);

};
