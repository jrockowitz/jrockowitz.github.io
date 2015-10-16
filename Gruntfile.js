module.exports = function(grunt) {

  grunt.initConfig({
    // Get project configuration from package.json.
    pkg: require('./package.json'),
    concat: {
      dist: {
        src: [
          'bower_components/jquery/jquery.min.js',
          'bower_components/bootstrap-sass-official/assets/javascripts/bootstrap.min.js'
        ],
        dest: 'assets/js/vendor.min.js'
      }
    },
    copy: {
      fonts: {
        files: [
          {
            expand: true,
            cwd: 'bower_components/bootstrap-sass-official/assets/fonts',
            src: ['**', '.**'],
            dest: 'fonts',
          }
        ],
      }
    },
    uglify: {
      dist: {
        files: {
          'js/main.min.js': 'js/main.js'
        }
      },
    },
    responsive_images: {
      thumbnails: {
        options: {
          sizes: [
            {
              rename: false,
              width: 300,
              sharpen: {
                sigma: 1,
                radius: 2
              }
            },
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
  grunt.loadNpmTasks('grunt-contrib-uglify');
  grunt.loadNpmTasks('grunt-exec');
  grunt.loadNpmTasks('grunt-responsive-images');

  grunt.registerTask('build', [ 'copy', 'concat', 'uglify', 'responsive_images']);
  grunt.registerTask('default', [ 'build', 'exec:build' ]);
  grunt.registerTask('serve', [ 'build', 'exec:serve' ]);
  grunt.registerTask('deploy', [ 'default', 'exec:deploy' ]);

};
