#!/bin/bash

# This script backups the databases of all the sites found in the
# Drupal sites folder as well as the Drupal code base and sites files.

# Requirements
#   - git http://git-scm.com/
#   - lastest version of Drush
#       http://ftp.drupal.org/files/projects/drush-All-Versions-HEAD.tar.gz

DRUSH="/Users/scor/.drush/drush/drush"

# Absolute path to the backup directory
BACKUP="/Users/scor/drupal_sites/multi6"
# Absolute path to the root folder of the Drupal code base.
DRUPAL_ROOT="$BACKUP/drupal"

# Creates a SQL dump of all the sites in the database directory.
for site in $( ls "$DRUPAL_ROOT/sites" )
do
  site_path="$DRUPAL_ROOT/sites/$site"

  # Selects directories which contain settings.php (excluding symbolic links)
  if [ -d $site_path -a -f "$site_path/settings.php"  -a ! -L $site_path ]; then
    echo "Dumping database for $site..."
    cd "$site_path"
    $DRUSH sql dump --ordered-dump --structure-tables-key=common > "$BACKUP/database/$site.sql"
    echo "Done"
  fi
done

# Ensures all the files (even the new ones) get added to the git repository
# Warning: this assumes you have created a git repository in your backup
# directory. Simply run `git init` in your backup directory to do so.
cd $BACKUP
git add database/
git add drupal/

# Commits every change
git commit -am "auto-commit changes database and files"

