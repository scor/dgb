<?php

// Custom settings for dgb drush command. Rename it to drushrc.php and place it
// in the Drupal installation directory DRUPAL_ROOT. Alternatively you can place
// it one level up and make a symbolic link from DRUPAL_ROOT, it will be backed
// as part of whole site. If you already have drushrc.php file, you can just add
// the settings below to it.

/**
 * List of tables whose *data* is skipped by the 'sql-dump' and 'sql-sync'
 * commands when the "--structure-tables-key=common" option is provided.
 * You may add specific tables to the existing array or add a new element.
 *
 * This list is very broad and includes tables from contrib modules which you
 * may not use, but they will be ignored by dgb.
 */
  $options['structure-tables'] = array(
    'common' => array(
      'accesslog',
      'apachesolr_index_entities*',
      'apachesolr_search_node',
      'cache',
      'cache_*',
      'captcha_sessions',
      'ctools_css_cache',
      'ctools_object_cache',
      'flood',
      'history',
      'migrate_*',
      'realname',
      'search_api_item',
      'search_dataset',
      'search_index',
      'search_node_links',
      'search_total',
      'sessions',
      'sparql_store_*',
      'views_content_cache',
      'watchdog',
      'xmlsitemap',
    ),
  );

/*
 * Command-specific options for dgb.
 *
 * To define options that are only applicable to certain commands,
 * make an entry in the 'command-specific' structures as shown below.
 * The name of the command may be either the command's full name
 * or any of the command's aliases.
 *
 * Options defined here will be overridden by options of the same
 * name on the command line.  Unary flags such as "--verbose" are overridden
 * via special "--no-xxx" options (e.g. "--no-verbose").
 *
 * Limitation: If 'verbose' is set in a command-specific option,
 * it must be cleared by '--no-verbose', not '--no-v', and visa-versa.
 */
$dgb_options = array(
  'verbose' => FALSE,
  // Necessary to have a better versionability of the sql dumps.
  'ordered-dump' => TRUE,
  // By default 'common' is a list of tables whose data will not be backed up.
  // See below to change or add other keys can be added to
  // $options['structure-tables'].
  'structure-tables-key' => 'common',
  // Useful to see what tables are effectively being backed up.
  'display-dump-command' => TRUE,
  // Path to the directory where the database dumps will be stored. Can be
  // relative to DRUPAL_ROOT or absolute. Make sure it is within the git
  // repository and outside the public web space of your server.
  // Default is to assume the database dumps directory is one level up the
  // drupal installation directory.
  'dgb-dumps' => '../databases',
  // Location of the git repository. Typically one level up DRUPAL_ROOT.
  'dgb-repository' => '..',
);

// Sometimes ignoring tables is not enough to ignore data that
// should not be backed up. This allows more fine grained control via regular
// expressions over what's ignored in the dump.
// This is a list of regular expressions for making changes to the dump.
// Useful for example to remove specific variables from the variable table.
//$dgb_options['post-processing-changes'] = array(
//  "/^INSERT INTO `variable` VALUES \('css_js_query_string','.*$\n/m" => '',
//  "/^INSERT INTO `variable` VALUES \('features_codecache','.*$\n/m" => '',
//);

$command_specific['dgb-dump'] = $dgb_options;
$command_specific['dgb-commit'] = $dgb_options;
$command_specific['dgb-usage'] = $dgb_options;
$command_specific['dgb-backup'] = $dgb_options;

// Default logging level for php notices.  Defaults to "notice"; set to "warning"
// if doing drush development.  Also make sure that error_reporting is set to E_ALL
// in your php configuration file.  See 'drush status' for the path to your php.ini file.
#$options['php-notices'] = 'warning';
