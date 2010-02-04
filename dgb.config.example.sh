#!/bin/bash

# rename to dgb.config.sh and these settings will be used by dgb.sh

# Absolute path to the drush executable.
DRUSH="/home/scor/.drush/drush/drush"

# Absolute path to the backup directory.
BACKUP="/var/sites/drupal_sites/multi6"

# Absolute path to the folder containing the dgb script. Assumes it's one level
# up the backup directory by default.
# (leave it as it is unless it has a specific).
DGB="$BACKUP/.."

# Absolute path to the root folder of the Drupal code base
# (leave it as it is unless it has a specific).
DRUPAL_ROOT="$BACKUP/drupal"
