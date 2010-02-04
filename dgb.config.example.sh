#!/bin/bash

# Rename this file to dgb.config.sh and configure the paths below to match your
# system. This is required for the main dgb.sh script to work.

# Absolute path to the drush executable.
DRUSH="/home/scor/.drush/drush/drush"

# Absolute path to the backup directory.
BACKUP="/var/sites/drupal_sites/multi6"

# Path to the folder containing the dgb script. Assumes it's one level
# up the backup directory by default.
# (leave it as it is unless it has a different path).
DGB="$BACKUP/.."

# Path to the root folder of the Drupal code base
# (leave it as it is unless it has a different path).
DRUPAL_ROOT="$BACKUP/drupal"
