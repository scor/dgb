#!/bin/bash

# rename to dgb.config.sh and these settings will be used by dgb.sh

# Absolute path to the drush executable.
DRUSH="/home/scor/.drush/drush/drush"

# Absolute path to the backup directory.
BACKUP="/var/sites/drupal_sites/multi6"

# Absolute path to the dgb script. Assumes it's one level up the backup
# directory by default. Uncomment and set the following otherwise.
#DGB="$BACKUP/.."

# Absolute path to the root folder of the Drupal code base (leave it as it is
# unless you have a specific path for it).
#DRUPAL_ROOT="$BACKUP/drupal"
