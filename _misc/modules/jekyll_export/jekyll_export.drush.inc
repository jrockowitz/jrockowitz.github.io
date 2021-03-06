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
    27, // The new MSKCC.org
    5, // Resume
  ];
  $custom_filenames = [
    2 => 'experience',
    5 => 'resume',
    16 => 'inettool',
    9 => 'mskcc-inettool',
    10 => 'mymskcc-inetool',
    13 => 'gerstner-sloan-kettering-inettool',
    14 => 'weill-cornell-inetool',
    15 => 'starr-inettool',
    11 => 'cancer-prediction-tools-dot-net',
    27 => 'mskcc-drupal-6',
    28 => 'mskcc-drupal-6-appendix',
    29 => 'mskcc-drupal-6-presentation',
    18 => 'time-magazine',
    19 => 'sony',
    20 => 'ethan-allen',
  ];
  $custom_titles = [
    2 => 'My Experience',
    9 => 'The Old MSKCC.org',
    10 => 'MyMSKCC',
    11 => 'Cancer Prediction Tools',
    13 => 'Gerstner Sloan-Kettering Graduate School of Biomedical Sciences',
    14 => 'Weill Cornell Graduate School of Medical Sciences',
    15 => 'Starr Cancer Consortium',
    16 => 'Inettool: Content Management System',
    18 => 'Time Magazine Online Media Planner',
    19 => 'Sony CD-It',
    20 => 'Ethan Allen Extranet',
    27 => 'The New MSKCC.org: Drupal 6 Case Study',
  ];
  $custom_short_titles = [
    9 => 'Memorial Sloan Kettering (Inettool)',
    10 => 'MyMSKCC',
    11 => 'Cancer Prediction Tools (ASP.NET)',
    13 => 'Gerstner Sloan-Kettering',
    14 => 'Weill Cornell Graduate School',
    15 => 'Starr Cancer Consortium',
    16 => 'Inettool (CMS)',
    18 => 'Time Magazine',
    19 => 'Sony',
    20 => 'Ethan Allen',
    27 => 'Memorial Sloan Kettering (Drupal 6)',
  ];
  $custom_weights = [
    27 => 20, // D6 - MSKCC
    9 => 31, // Inettool - MSKCC
    11 => 21, // Nomograms
    16 => 30, // Inettool
    13 => 32, // Inettool - GSK,
    14 => 33, // Inettool - Weill
    15 => 34, // Inettool - Starr
    10 => 35, // Inettool - MyMskcc
    18 => 40, // Time
    19 => 40, // Sony
    20 => 40, // Ethan Allen
  ];
  $custom_icons = [
    1 => 'fa fa-user', // About
    5 => 'fa fa-file', // Resume
    2 => 'fa fa-quote-left', // Experience
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
    if (isset($custom_icons[$nid])) {
      $front_matter['icon'] = $custom_icons[$nid];
    }
    $front_matter['title'] = (isset($custom_titles[$nid])) ? $custom_titles[$nid] : $node->title;
    if ($is_work) {
      $front_matter['short_title'] = $custom_short_titles[$nid];
    }
    if ($node->body['und'][0]['summary']) {
      $front_matter['description'] = $node->body['und'][0]['summary'];
    }
    $front_matter['layout'] = 'page';

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

    // Set body and description (aka summary).
    $body = _drush_jekyll_export_body($node->body['und'][0]['value'], $front_matter);

    // Set thumbnail placeholder for work, before images are added.
    if ($is_work) {
      $front_matter['thumbnail']['src'] = NULL;
    }

    // Fix inline images
    $body = _drush_jekyll_export_inline_images($body, $front_matter);

    // Fix inline links
    $body = _drush_jekyll_export_inline_links($body);

    // Fix attached images (aka screen shots)
    _drush_jekyll_export_attached_images($node, $front_matter);

    // Generate YAML.
    $yaml = "";
    $yaml .= str_replace('...', '---', yaml_emit($front_matter));
    $yaml .= "\n";
    $yaml .= $body;

    // Put Jekyll file.
    file_put_contents($destination, $yaml);

    // DEBUG: Front Matter
    // drush_print(yaml_emit($front_matter));drush_print();

  }
  return TRUE;
}

/**
 * @param $node
 * @param $front_matter
 */
function _drush_jekyll_export_attached_images($node, &$front_matter) {
  if (isset($node->field_screenshots)) {
    foreach ($node->field_screenshots['und'] as $item) {
      $front_matter['images'][] = [
        'src' => _drush_jekyll_export_image($item['uri']),
        'title' => $item['title'],
      ];
    }

    $front_matter['thumbnail']['src'] = str_replace('/images/', '/images-thumbnails/', $front_matter['images'][0]['src']);
  }
}

/******************************************************************************/
// Cleanup functions
/******************************************************************************/

function _drush_jekyll_export_body($body, &$front_matter) {
  // Convert tabs to 2 spaces.
  $body = str_replace("\t", '  ', $body);
  $body = str_replace("    ", '  ', $body);

  // Remove <h1> from resume.
  $body = preg_replace('#<h1[^>]+>[^<]+</h1>#', '', $body);

  // Change bold paragraph to <h4>.
  $body = preg_replace('#<p><strong>(.*?)</strong><br />\s*#s', '<h4>\1</h4>' . "\n" . '<p>', $body);

  if (strpos($body, '<!--break-->') !== FALSE) {
    list($summary, $ignore_body) = explode('<!--break-->', $body);
    $front_matter['description'] = preg_replace('#^<p>(.*?)</p>\s*$#', '\1', $summary);
    $body = str_replace('<!--break-->', '', $body);

  }

  return $body;
}

function _drush_jekyll_export_inline_links($body) {
  $custom_links = [
    'inettool-content-management-system' => '/work/inettool/',
  ];

  if (preg_match_all('/href="([^"]+)"/', $body, $matches)) {
    foreach ($matches[1] as $source_href) {
      if (isset($custom_links[$source_href])) {
        $body = str_replace($source_href, $custom_links[$source_href], $body);
      }
      elseif (strpos($source_href, 'http://thebigbluehouse.com/') === 0) {
        $destination_href = $source_href;
        $destination_href = str_replace('http://thebigbluehouse.com/sites/default/files/images/msk_cs_', '/images/work/drupal_6_mskcc/', $destination_href);
        $destination_href = str_replace('http://thebigbluehouse.com/sites/default/files/images//msk_cs_', '/images/work/drupal_6_mskcc/', $destination_href);
        $destination_href = str_replace('http://thebigbluehouse.com/sites/default/files//images/msk_cs_', '/images/work/drupal_6_mskcc/', $destination_href);
        $destination_href = str_replace('http://thebigbluehouse.com/case-study/mskcc-appendix', '/mskcc-drupal-6-appendix/', $destination_href);
        $destination_href = str_replace('http://thebigbluehouse.com/', '/', $destination_href);
        if (strpos($destination_href, '/images/') === 0) {
          $destination_href = str_replace(basename($destination_href), str_replace('_', '-', basename($destination_href)), $destination_href);
        }
        $body = str_replace($source_href, $destination_href, $body);
      }
      else {
        if (strpos($source_href, 'http') !== 0) {
          throw new Exception('Broken link to ' . $source_href);
        }
      }

      // DEBUG:
      // drush_print("$source_href => $destination_href");
    }
  }
  return $body;
}

function _drush_jekyll_export_inline_images($body, &$front_matter) {
  if (preg_match_all('/src="([^"]+)"/', $body, $matches)) {
    foreach ($matches[1] as $source_src) {
      if (strpos($source_src, 'http://thebigbluehouse.com/sites/default/files') !== 0) {
        continue;
      }
      $destination_src =  str_replace('/images/', '/images-thumbnails/', _drush_jekyll_export_image($source_src));
      $body = str_replace($source_src, $destination_src, $body);

      if (empty($front_matter['thumbnail']['src'])) {
        $front_matter['thumbnail']['src'] = $destination_src;
      }
    }
    // Replace image medium styl.
    $body = str_replace(' class="image-medium"', '', $body);
    // Remove lightbox attribute
    $body = preg_replace('# rel="lightbox[^"]+"#', '', $body);

    return $body;
  }
  return $body;
}

/******************************************************************************/
// Helper functions
/******************************************************************************/

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
