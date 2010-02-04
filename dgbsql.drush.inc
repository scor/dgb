<?php

/**
 * @file
 *   dgbsql drush command.
 */

/**
 * Implementation of hook_drush_command().
 *
 * @return
 *   An associative array describing your command(s).
 */
function dgbsql_drush_command() {
  $options['--database'] = 'The DB connection key if using multiple connections in settings.php.';
  if (drush_drupal_major_version() >= 7) {
    $options['--target'] = 'The name of a target within the specified database.';
  }

  $items = array();

  $items['dgbsql-dump'] = array(
    'description' => "Alternative to the core sql-dump command, with support for table name expansion and validation.",
    'callback' => 'drush_dgbsql_dump_dgbexecute',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_CONFIGURATION,
    'examples' => array(
      'drush sql-dump --result-file=../18.sql' => 'Save SQL dump to the directory above Drupal root.',
      'drush sql-dump --skip-tables-key=common' => 'Skip standard tables. @see example.drushrc.com',
    ),
    'options' => array(
      '--result-file' => 'Save to a file. The file should be relative to Drupal root. Recommended.',
      '--dgbsql-log' => 'Display the query used for the dump. Helps to ensure the right tables have been backed up.',
      '--skip-tables-key' => 'A key in the $skip_tables array. @see example.drushrc.php. Optional.',
      '--structure-tables-key' => 'A key in the $structure_tables array. @see example.drushrc.php. Optional.',
      '--tables' => 'An explcit list of tables to dump, in comma delimited format',
      '--ordered-dump' => 'Use this option to output ordered INSERT statements in the sql-dump.Useful when backups are managed in a Version Control System. Optional.',
      '--create-db' => 'Wipe existing tables (i.e. --clean). Postgres only.',
      '--data-only' => 'Omit CREATE TABLE statements. Postgres only.',
      '--ordered-dump' => 'Order by primary key and add line breaks for efficient diff in revision control. Also, faster rsync. Slows down the dump. Mysql only.',
    ) + $options,
  );

  return $items;
}

/**
 * Command callback. Dumps the Drupal database in SQL format.
 */
function drush_dgbsql_dump_dgbexecute() {

  // Set a default list of structure tables which should work for most sites.
  $default_structure_tables = array(
    'common' => array('accesslog', 'cache', 'cache_*', 'history', 'search_dataset', 'search_index', 'search_total', 'sessions', 'watchdog'),
  );
  drush_set_default('structure-tables', $default_structure_tables);

  // Prepare SQL dump command.
  $exec = drush_dgbsql_dump();

  if (drush_get_option('dgbsql-log')) {
    drush_print('Executing ' . $exec);
  }

  // Avoid the php memory of the $output array in drush_shell_exec().
  $return = drush_op('system', $exec);
  return $return;
}

function drush_dgbsql_dump($db_spec = NULL) {
  $ignores = array();

  // Skip large core tables if instructed.  Also used by 'sql-sync' command.
  $skip_tables = _drush_dgbsql_get_table_list('skip-tables');
  // Skip any structure-tables as well.
  $structure_tables = _drush_dgbsql_get_table_list('structure-tables');
  $skip_tables += $structure_tables;
  // Dump only the specified tables.  Takes precedence over skip-tables and
  // structure-tables.
  $tables = _drush_dgbsql_get_table_list('tables');
  $data_only = drush_get_option('data-only');
  // The ordered-dump option is only supported by MySQL for now.
  $ordered_dump = drush_get_option('ordered-dump');

  if (is_null($db_spec)) {
    $db_spec = _drush_sql_get_db_spec();
  }
  $database = $db_spec['database'];

  switch (_drush_sql_get_scheme($db_spec)) {
    case 'mysql':
      $exec = 'mysqldump' . (drush_get_context('DRUSH_VERBOSE') ? ' -v' : '');
      if ($file = drush_get_option('result-file')) {
        $exec .= ' --result-file '. $file;
      }
      $extra = ' --single-transaction --opt -Q' . _drush_sql_get_credentials($db_spec);
      if (isset($data_only)) {
        $extra .= ' --no-create-info';
      }
      if (isset($ordered_dump)) {
       $extra .= ' --skip-extended-insert --order-by-primary';
      }
      $exec .= $extra;

      if (!empty($tables)) {
        $exec .= ' ' . implode(' ', $tables);
      }
      else {
        // Append the ignore-table options.
        foreach ($skip_tables as $table) {
          $ignores[] = "--ignore-table=$database.$table";
        }
        $exec .= ' '. implode(' ', $ignores);

        // Run mysqldump again and append output if we need some structure only
        // tables.
        if (!empty($structure_tables)) {
          $exec .= "; mysqldump --no-data $extra " . implode(' ', $structure_tables);
          if ($file) {
            $exec .= " >> $file";
          }
        }
      }
      break;
    case 'pgsql':
      $create_db = drush_get_option('create-db');
      $exec = 'pg_dump ' . (drush_get_context('DRUSH_VERBOSE') ? ' -v' : '');
      if ($file = drush_get_option('result-file')) {
        $exec .= ' --file '. $file;
      }
      // Unlike psql, pg_dump does not take a '-d' flag before the database
      // name. We'll put the database name in 'by hand' and then clear the name
      // from the record to prevent _drush_sql_get_credentials from inserting
      // a -d term that pg_dump does not understand.
      $extra = ' ' . $db_spec['database'];
      $db_spec['database'] = null;
      $extra .= _drush_sql_get_credentials($db_spec);
      if (isset($data_only)) {
        $extra .= ' --data-only';
      }
      $exec .= $extra;
      $exec .= (!isset($create_db) && !isset($data_only) ? ' --clean' : '');

      if (!empty($tables)) {
        foreach ($tables as $table) {
          $exec .= " --table=$table";
        }
      }
      else {
        foreach ($skip_tables as $table) {
          $ignores[] = "--exclude-table=$table";
        }
        $exec .= ' '. implode(' ', $ignores);
        // Run pg_dump again and append output if we need some structure only
        // tables.
        if (!empty($structure_tables)) {
          $schemaonlies = array();
          foreach ($structure_tables as $table) {
            $schemaonlies[] = "--table=$table";
          }
          $exec .= "; pg_dump --schema-only " . implode(' ', $schemaonlies) . $extra;
          if ($file) {
            $exec .= " >> $file";
          }
        }
      }
      break;
  }

  return $exec;
}

// Given an option name (skip-tables, structure-tables or tables),
// look up the appropriate list of tables in the given category.
function _drush_dgbsql_get_table_list($option_name) {
  $tables = array();
  foreach(array('', 'source-', 'target-') as $prefix) {
    $key_list = drush_get_option($prefix . $option_name . '-key');
    foreach(explode(',', $key_list) as $key) {
      $all_tables = drush_get_option($option_name, array());
      if (array_key_exists($key, $all_tables)) {
        $tables += $all_tables[$key];
      }
      if ($option_name != 'tables') {
        $all_tables = drush_get_option('tables', array());
        if (array_key_exists($key, $all_tables)) {
          $tables += $all_tables[$key];
        }
      }
    }
    $table_list = drush_get_option($prefix . $option_name . '-list');
    if (isset($table_list)) {
      $tables += explode(',', $table_list);
    }
  }

	// Get the existing table names contained in the current database.
	$db_tables = _drush_dgbsql_get_db_table_list();

	// Table name expansion based on * wildcard.
	$expanded_db_tables = array();
	foreach ($tables as $k => $table) {
		// Only deal with table names containing a wildcard.
		if (strpos($table, '*') !== FALSE) {
			$pattern = '/^' . str_replace('*', '.*', $table) . '$/i';
			// Merge those existing tables which match the pattern with the rest of
			// the expanded table names.
			$expanded_db_tables += preg_grep($pattern, $db_tables);
			// Drop the wildcard table, this is not a valid table name.
			unset($tables[$k]);
		}
	}

	// Ensure all the tables actually exist in the database.
	foreach ($tables as $k => $table) {
		if (!in_array($table, $db_tables)) {
			unset($tables[$k]);
		}
	}

	// Merge expanded and validated table names.
	$tables += $expanded_db_tables;

	// Make sure each table name is unique and sort them.
	$tables = array_unique($tables);
	sort($tables);

  return $tables;
}

/**
 * Extract the name of all existing tables in the current local database.
 *
 * @return array
 *   A list of table names which exist in the current database.
 */
function _drush_dgbsql_get_db_table_list() {
  static $db_tables = array();

  if (!empty($db_tables)) {
    return $db_tables;
  }

  $scheme = _drush_sql_get_scheme();

  $query = 'show tables;';
  if ($scheme == 'pgsql') {
    $query = "select tablename from pg_tables where schemaname='public';";
  }

  // We do not have access to the database other than by running the appropriate
  // command line, sending the results into a file and reading it.
  if ($query_file = drush_save_data_to_temp_file($query)) {
    // Create a temporary file for storing the results.
    $results_file = tempnam(sys_get_temp_dir(), $target_db_url['database'] . '.sql.');

    switch ($scheme) {
      case 'mysql':
        $exec = 'mysql' . (drush_get_context('DRUSH_VERBOSE') ? ' -v' : '');
        $exec .= _drush_sql_get_credentials($db_spec);
        $exec .= ' ' . drush_get_option('extra');
        $exec .= " < $query_file > $results_file";

        break;
      case 'pgsql':
        $exec = 'psql';
        $exec .= _drush_sql_get_credentials($db_spec);
        $exec .= (drush_get_context('DRUSH_VERBOSE') ? '' : ' -q');
        $exec .= ' ' . (drush_get_option('extra') ? drush_get_option('extra') : "--no-align --field-separator=$'\t' --pset footer=off");
        $exec .= " --file $query_file > $results_file";
        break;
    }

    // Execute the show tables; command.
    drush_op('system', $exec);
    // Read all the results back in memory.
    $results = file($results_file);
    // We can scratch the query and results files now.
    drush_op('unlink', $query_file);
    drush_op('unlink', $results_file);

    // The first result is the column name.
    unset($results[0]);
    // Clean up table names.
    foreach ($results as &$v) {
      $v = trim($v);
    }
    $db_tables = $results;
  }

  return $db_tables;
}
