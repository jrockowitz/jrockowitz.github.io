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
        uglify: {
            dist: {
                files: {
                    'js/main.min.js': 'js/main.js'
                }
            }
        },
        exec: {
            build: {
                cmd: 'jekyll build'
            },
            serve: {
                // http://unix.stackexchange.com/questions/102956/how-to-run-a-command-in-the-background-with-a-delay
                cmd: '(sleep 5; open http://localhost:4000) &; jekyll serve --drafts --watch'
            },
            deploy: {
                cmd: 'git add --all; git commit -am"Deploying changes to GitHub";'
            }
        }
    });

    grunt.loadNpmTasks('grunt-contrib-concat');
    grunt.loadNpmTasks('grunt-contrib-uglify');
    grunt.loadNpmTasks('grunt-exec');

    grunt.registerTask('default', [ 'concat', 'uglify', 'exec:build' ]);
    grunt.registerTask('serve', [ 'concat', 'uglify', 'exec:serve' ]);
    grunt.registerTask('deploy', [ 'default', 'exec:deploy' ]);

};
