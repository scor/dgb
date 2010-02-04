#!/bin/bash

# This script backups the databases of all the sites found in the
# Drupal sites folder as well as the Drupal code base and sites files.

# Requirements
#   - git <http://git-scm.com/>
#   - lastest version of Drush <http://drupal.org/project/drush>

# Path to the script, we cannot rely on pwd if the script is called with cron.
SELF_PATH=$(cd -P -- "$(dirname -- "$0")" && pwd -P)

# Override default variables with custom configuration file, if it exists.
if [ -f "$SELF_PATH/dgb.config.sh" ]; then
  . "$SELF_PATH/dgb.config.sh"
else
  echo "The dgb configuration file is missing. Please rename dgb.config.example.sh to dgb.config.sh and configure it."
  exit
fi

status() {
  if [ -f "$SELF_PATH/dgb.config.sh" ]; then
    echo "Config file:           $SELF_PATH/dgb.config.sh"
  else
    echo "Config file:           none"
  fi
  echo "Drush location:        $DRUSH"
  echo "dgbsql drush location: $DGB"
  echo "Backup path:           $BACKUP"
  echo "Drupal code base:      $DRUPAL_ROOT"

}

dump_databases() {
  # Creates a SQL dump of all the sites in the database directory.
  for site in $( ls "$DRUPAL_ROOT/sites" )
  do
    site_path="$DRUPAL_ROOT/sites/$site"

    # Selects directories which contain settings.php (excluding symbolic links)
    if [ -d $site_path -a -f "$site_path/settings.php"  -a ! -L $site_path ]; then
      echo "Dumping database for site $site..."
      cd "$site_path"
      $DRUSH dgbsql-dump --include="$DGB" --ordered-dump --structure-tables-key=common --dgbsql-log --result-file="$BACKUP/database/$site.sql"
      echo "Done"
    fi
  done
}

commit() {
  # Ensures all the files (even the new ones) get added to the git repository
  # Warning: this assumes you have created a git repository in your backup
  # directory. Simply run `git init` in your backup directory to do so.
  cd $BACKUP
  git add database/
  git add drupal/

  # Commits every change
  git commit -am "auto-commit changes database and files"
}

case "${1:-''}" in
  'status'|'info')
    status
    ;;

  'dump')
    dump_databases
    ;;

  'commit')
    commit
    ;;

  'help')
    echo "Usage: status|dump|commit"
    ;;

  *)
    # By default, if no option is given as input, the whole site and databases
    # are committed.
    dump_databases
    commit
    ;;
esac

