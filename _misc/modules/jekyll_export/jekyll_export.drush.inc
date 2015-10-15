<?php
/**
 * @file
 * Jekyll export drush command.
 */

define('JEKYLL_EXPORT_DESTINATION_DIRECTORY', '/var/www/rockowij/sites/local.jrockowitz.github.io');

/**
 * Implements hook_drush_command().
 */
function jekyll_export_drush_command() {
  $items = [];
  $items['jekyll-export'] = [
    'description' => 'Exports Drupal 7 nodes to Jekyll files.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  ];
  return $items;
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_jekyll_export() {
  $skipped_nodes = [
    22, // dprint_r()
    23, // dprint_rx
  ];
  $custom_filenames = [
    2 => 'about-experience',
    5 => 'resume',
    16 => 'inettool',
    9 => 'inettool-mskcc',
    10 => 'inetool-mymskcc',
    13 => 'inettool-gerstner-sloan-kettering-graduate-school-biomedical-sciences',
    14 => 'inetool-weill-cornell-graduate-school-medical-sciences',
    15 => 'inettool-starr-cancer-consortium',
    27 => 'mskcc-drupal-6',
    28 => 'mskcc-drupal-6-appendix',
    29 => 'mskcc-drupal-6-presentation',
  ];
  $custom_weights = [
    27 => 20, // D6 - MSKCC
    11 => 21, // Nomograms
    16 => 30, // Inettool
    9 => 31, // Inettool - MSKCC
    13 => 32, // Inettool - GSK,
    14 => 33, // Inettool - Weill
    15 => 34, // Inettool - Starr
    10 => 35, // Inettool - MyMskcc
    18 => 40, // Time
    19 => 40, // Sony
    20 => 40, // Ethan Allen
  ];
  $custom_links = [
    'inettool-content-management-system' => '/work/inettool/',
  ];

  $nodes = node_load_multiple(array(), array('status' => 1));
  foreach ($nodes as $nid => $node) {
    // Skip nodes by nid or if type is blog.
    if (in_array($nid, $skipped_nodes) || $node->type == 'blog') {
      continue;
    }

    // Determine if node is 'work'
    $is_work = (in_array($node->type, ['case_study', 'website']));

    // Set front matter
    $front_matter = [];
    $front_matter['layout'] = 'page';
    $front_matter['title'] = $node->title;
    if ($node->body['und'][0]['summary']) {
      $front_matter['description'] = $node->body['und'][0]['summary'];
    }

    // Set permalink.
    if (isset($custom_filenames[$nid])) {
      $permalink = $custom_filenames[$nid];
    }
    elseif ($path = path_load('node/' . $nid)) {
      $permalink = $destination_filename = str_replace('/', '-', $path['alias']);
    }
    else {
      $permalink = _drush_jekyll_export_slugify($node->title);
    }
    $front_matter['permalink'] = ($is_work ? '/work/' : '/') . "$permalink/";

    // Set destination (using permalink).
    $destination_directory = JEKYLL_EXPORT_DESTINATION_DIRECTORY . '/' . ($is_work ? '_work' : '_pages');
    $destination_filename = (($is_work) ? sprintf("%02d", $custom_weights[$nid]) . '-' : '') . $permalink;
    $destination = "$destination_directory/$destination_filename.html";

    // Display message
    $t_args = [
      '@title' => $node->title,
      '@type' => $node->type,
      '@nid' => $node->nid,
      '@destination' => str_replace(JEKYLL_EXPORT_DESTINATION_DIRECTORY . '/', '', $destination),
    ];

    drush_print(dt('Exporting @title [@type-@nid] to @destination', $t_args));

    // Set body.
    $body = $node->body['und'][0]['value'];
    $body = _drush_jekyll_export_cleanup_body($body);


    if (strpos($body, '<!--break-->') !== FALSE) {
      list($summary, $body) = explode('<!--break-->', $body);
      $body = trim($body) ?: $summary;
      $front_matter['description'] = preg_replace('#^<p>(.*?)</p>\s*$#', '\1', $summary);
    }

    // Fix inline images
    if (preg_match_all('/src="([^"]+)"/', $body, $matches)) {
      foreach ($matches[1] as $source_src) {
        if (strpos($source_src, 'http://thebigbluehouse.com/sites/default/files') !== 0) {
          continue;
        }
        $body = str_replace($source_src, _drush_jekyll_export_image($source_src), $body);
      }
    }

    // Fix inline links
    if (preg_match_all('/href="([^"]+)"/', $body, $matches)) {
      foreach ($matches[1] as $source_href) {
        if (isset($custom_links[$source_href])) {
          $body = str_replace($source_href, $custom_links[$source_href], $body);
        }
        elseif (strpos($source_href, 'http://thebigbluehouse.com/') === 0) {
          $destination_href = $source_href;
          $destination_href = str_replace('http://thebigbluehouse.com/sites/default/files/images/msk_cs_', '/images/work/drupal_6_mskcc/', $destination_href);
          $destination_href = str_replace('http://thebigbluehouse.com/sites/default/files/images//msk_cs_', '/images/work/drupal_6_mskcc/', $destination_href);
          $destination_href = str_replace('http://thebigbluehouse.com/case-study/mskcc-appendix', '/mskcc-appendix/', $destination_href);
          $destination_href = str_replace('http://thebigbluehouse.com/', '/', $destination_href);

          $body = str_replace($source_href, $destination_href, $body);
        }
        else {
          if (strpos($source_href, 'http') !== 0) {
            throw new Exception('Broken link to '. $source_href);
          }
        }

        // DEBUG:
        // drush_print("$source_href => $destination_href");
      }

    }

    // Fix attached images (aka screen shots)
    if (isset($node->field_screenshots)) {
      foreach ($node->field_screenshots['und'] as $item) {
        $front_matter['images'][] = [
          'src' => _drush_jekyll_export_image($item['uri']),
          'title' => $item['title'],
        ];
      }
    }

    // Generate YAML.
    $yaml = "";
    $yaml .= str_replace('...', '---', yaml_emit($front_matter));
    $yaml .= "\n";
    $yaml .= $body;

    // Put Jekyll file.
    file_put_contents($destination, $yaml);

    // DEBUG: Front Matter
    // drush_print(yaml_emit($front_matter));drush_print();

    drush_print(' ');

  }
  return TRUE;
}

function _drush_jekyll_export_cleanup_body($body) {
// Convert tabs to 2 spaces.
  $body = str_replace("\t", '  ', $body);
  $body = str_replace("    ", '  ', $body);

  // Remove <h1> from resume.
  $body = preg_replace('#<h1[^>]+>[^<]+</h1>#', '', $body);
  // Change bold paragraph to <h4>.
  $body = preg_replace('#<p><strong>(.*?)</strong><br />\s*#s', '<h4>\1</h4>' . "\n" . '<p>', $body);
  return $body;
}

function _drush_jekyll_export_image($uri) {
  $rename_images = [
    'msk_cs' => 'drupal_6_mskcc',
    'nomograms' => 'nomograms_asp_net',

    'inettool' => 'inettool',
    'gsk' => 'inettool_gsk',
    'mskcc' => 'inettool_mskcc',
    'mymskcc' => 'inettool_mymskcc',
    'star' => 'inettool_star',
    'starr' => 'inettool_starr',
    'weill' => 'inettool_weill',

    'sony_cdit' => 'sony_cd_it',
    'time' => 'time',
    'ethan_allen' => 'ethan_allen',
  ];

  // Normalize the source file path and get the source file name.
  $source_file_path = $uri;
  $source_file_path = str_replace('public://', 'sites/default/files/',  $source_file_path);
  $source_file_path = str_replace('http://thebigbluehouse.com/sites/default/files/styles/medium/public/', 'sites/default/files/',  $source_file_path);
  $source_file_name = basename($source_file_path);

  $destination_file_name = $source_file_name;
  foreach ($rename_images as $image_source => $image_destination) {
    if (strpos($destination_file_name, $image_source . '_') === 0) {
      $destination_file_name = str_replace('_', '-', str_replace($image_source . '_', '', $destination_file_name));
      $destination_file_name = $image_destination . '/' . $destination_file_name;
    }
    elseif (strpos($destination_file_name, $image_source . '.') === 0) {
      $destination_file_name = $image_destination . '/' . str_replace('_', '-', $image_destination) . '.png';
    }
  }
  $src = '/images/work/' . $destination_file_name;
  $destination_file_path = JEKYLL_EXPORT_DESTINATION_DIRECTORY . $src;

  // Create destination directory and copy the image file.
  $destination_directory = dirname($destination_file_path);
  if (!file_exists($destination_directory)) {
    mkdir($destination_directory, 0777, true);
  }
  file_unmanaged_copy($source_file_path, $destination_file_path, FILE_EXISTS_REPLACE);

  // Display message.
  $t_args = [
    '@source' => $source_file_path,
    '@destination' => str_replace(JEKYLL_EXPORT_DESTINATION_DIRECTORY . '/', '', $destination_file_path),
  ];
  // drush_print( t('   @source => @destination', $t_args));

  return $src;

}

function _drush_jekyll_export_slugify($text) {
  // Remove accents. http://stackoverflow.com/questions/3230012/replacing-accents-with-their-counterparts
  $text = strtr($text, ['Š' => 'S', 'š' => 's', 'Đ' => 'Dj', 'đ' => 'dj', 'Ž' => 'Z', 'ž' => 'z', 'Č' => 'C', 'č' => 'c', 'Ć' => 'C', 'ć' => 'c', 'À' => 'A', 'Á' => 'A', 'Â' => 'A', 'Ã' => 'A', 'Ä' => 'A', 'Å' => 'A', 'Æ' => 'A', 'Ç' => 'C', 'È' => 'E', 'É' => 'E', 'Ê' => 'E', 'Ë' => 'E', 'Ì' => 'I', 'Í' => 'I', 'Î' => 'I', 'Ï' => 'I', 'Ñ' => 'N', 'Ò' => 'O', 'Ó' => 'O', 'Ô' => 'O', 'Õ' => 'O', 'Ö' => 'O', 'Ø' => 'O', 'Ù' => 'U', 'Ú' => 'U', 'Û' => 'U', 'Ü' => 'U', 'Ý' => 'Y', 'Þ' => 'B', 'ß' => 'Ss', 'à' => 'a', 'á' => 'a', 'â' => 'a', 'ã' => 'a', 'ä' => 'a', 'å' => 'a', 'æ' => 'a', 'ç' => 'c', 'è' => 'e', 'é' => 'e', 'ê' => 'e', 'ë' => 'e', 'ì' => 'i', 'í' => 'i', 'î' => 'i', 'ï' => 'i', 'ð' => 'o', 'ñ' => 'n', 'ò' => 'o', 'ó' => 'o', 'ô' => 'o', 'õ' => 'o', 'ö' => 'o', 'ø' => 'o', 'ù' => 'u', 'ú' => 'u', 'û' => 'u', 'ý' => 'y', 'þ' => 'b', 'ÿ' => 'y', 'Ŕ' => 'R', 'ŕ' => 'r']);

  // Lowercase.
  $text = strtolower($text);

  // Change P.S. to PS.
  $text = str_replace(' p.s. ', ' ps ', $text);

  // Remove apostrophes.
  $text = str_replace("'s ", ' ', $text);

  // Remove middle initials.
  $text = preg_replace('/ [a-z]\.? /', ' ', $text);

  // Remove ignored words.
  $text = preg_replace('/\b(?:a|an|as|at|before|but|by|for|from|is|in|into|like|of|off|on|onto|or|our|per|since|than|the|this|that|to|up|via|with)\b/i', ' ', $text);

  // Replace non letter or digits by -
  $text = preg_replace('~[^\\pL\d]+~u', '-', $text);

  // Trim.
  $text = trim($text, '-');

  // Remove unwanted characters.
  $text = preg_replace('~[^-\w]+~', '', $text);

  // Remove certain words from all paths.
  $text = preg_replace('/-(annual-report|memorial-sloan-kettering-cancer-center|cancers|cancer|center|training|program|story)(-|$)/', '\2', $text);

  // Remove certain prefixes (w/o -and-) from all paths.
  $text = preg_replace('/^(center-news|center|department|recommended|browse|find|search)-(?!and-)/', '', $text);

  // Abbreviate Memorial Sloan Kettering Cancer Center.
  $text = str_replace(
    ['memorial-sloan-kettering-cancer-center', 'msk'],
    ['mskcc', 'msk'],
    $text
  );

  return $text;
}
