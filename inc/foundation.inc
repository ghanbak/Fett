<?php

/**
 * Include Foundation CSS/SCSS components
 */
function fett_foundation_add_css(){
  global $theme_key;
  $sonar_enabled = module_exists('sonar');
  $scss = theme_get_setting('fett_foundation_scss');
  $options = array(
    'every_page' => TRUE,
    'weight' => -1000
  );

  drupal_add_css();

  if(@is_array($scss) && $sonar_enabled){
    $fett_path = drupal_get_path('theme', 'fett');
    $path = $fett_path . '/assets/foundation/scss';
    $override = drupal_get_path('theme', $theme_key).'/assets/scss/override';

    // Add normalize no matter what
    drupal_add_css($fett_path . '/assets/scss/theme.scss', $options);

    // Add normalize no matter what
    drupal_add_css($path . '/normalize.scss', $options);

    $path .= '/foundation/components';

    foreach($scss as $s){
      if(empty($s)) continue;
      $use_path = $path;
      // Allow local overriding of Bootstrap files
      if(file_exists($override.'/_'.$s.'.scss')){
        $use_path = $override;
      }
      drupal_add_css($use_path.'/_'.$s.'.scss', $options);
    }
  }
  // We have not selected any SCSS components to include OR Sonar is not
  // enabled. In this case we just use default compiled Foundation.
  else{
    $path = drupal_get_path('theme', 'fett').'/assets/foundation/css';

    // Add normalize no matter what
    drupal_add_css($path . '/normalize.css', $options);

    drupal_add_css($path . '/foundation.min.css', $options);
  }
}

/**
 * Include Foundation SCSS components
 */
function fett_foundation_add_js($js = array()){

  $js = !empty($js) ? $js : theme_get_setting('fett_foundation_js');
  $path = drupal_get_path('theme', 'fett').'/assets/foundation/js/foundation';
  $options = array(
    'every_page' => TRUE,
    'scope' => 'footer'
  );
  if(@is_array($js)){
    drupal_add_js($path.'/foundation.js', $options + array('weight' => -1));
    foreach($js as $s){
      if(!empty($s)){
        drupal_add_js($path.'/'.$s, $options);
      }
    }
  }

}

/**
 * Returns a list of Foundation SCSS components
 */
function fett_foundation_get_scss($options = FALSE) {
  $file = file_get_contents(dirname(dirname(__FILE__)) . '/assets/foundation/scss/foundation.scss');
  preg_match_all("/components\/(.*)?\"/",$file,$matches, PREG_PATTERN_ORDER);
  $names = array();
  if(!empty($matches[1]) && is_array($matches[1])){
    foreach($matches[1] as $match){
      $names[$match] = ucwords(str_replace('-',' ',$match));
    }
  }
  return $names;
}

/**
 * Returns a list of Foundation javascript components
 */
function fett_foundation_get_js($options = FALSE) {
  $base = dirname(dirname(__FILE__)) . '/assets/foundation/js/foundation';
  $files = array_diff(scandir($base), array('.', '..'));
  $names = array();
  if(is_array($files)){
    $key = array_search('foundation.js', $files);
    unset($files[$key]);
    foreach($files as $file){
      $names[$file] = ucwords(preg_replace('/(foundation\.)?(.*)(\.js)/', '$2', $file));
    }
  }

  return $names;
}