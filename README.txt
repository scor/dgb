Drupal Git Backup (DGB)
=======================

DGB is a Drush command to easily maintain a full backup a Drupal code base and
all its sites, keeping Drupal files and sites databases together in sync in the
same repository. Its upports multi site setups and will dump each site database
as a version control friendly SQL file. This command leverages git as a powerful
(decentralized) version control system to store all site's data efficiently.
This Drush command is still at an experimental state so use with caution. Please
give any feedback or file bugs at http://github.com/scor/dgb

DGB requires Drush 3 <http://drupal.org/project/drush> and works for any version
of Drupal 5, 6 and 7.

Installation
============

Install dgb like any other Drush command. See Drush readme.txt file. The typical
directory structure dgb was designed for is the following:

-- dgb_instance --> drupal --> index.php
                           --> sites --> example1.com
                                     --> example2.com
                --> databases --> example1.com.sql
                              --> example2.com.sql
                --> drushrc.php (dgb configuration file)
                --> .git (dgb git repository)

1. create the dgb_instance folder somewhere on your server (the name does not
   matter). Make sure it's outside the web public directory.
2. place your drupal code base in the dbg_instance directory.
3. copy example-dgb.drushrc.php in the dgb_instance folder, rename it
   drushrc.php and check its content. Its default settings should works for most
   configurations.
4. create a symbolic link to drushrc.php from inside DRUPAL_ROOT, i.e. run the
   following from inside drupal: ln -s ../drushrc.php drushrc.php
   (the main drushrc.php is kept outside the drupal directory to avoid losing it
   during a core file update, but you can put it direcly in there if you prefer.
   dgb will require a drushrc.php in DRUPAL_ROOT no matter what).
5. create the 'databases' folder (or whatever name you specified in drushrc.php)
6. initialize the git repository in the dgb_instance folder by running git init
7. from the DRUPAL_ROOT directory, type drush dgb-backup to see if dgb was
   installed successfully.
8. setup a cron job to run the backup every day or so, e.g. to run dgb every day
   at 11am:
   00 11 * * * cd /var/sites/dgb_instance/drupal; /usr/local/bin/drush dgb-backup
   (adapt /usr/local/bin/drush to wherever your drush is located on your server)


Commands
========

dgb ships with several commands (in order of relevance):

- dgb-dump: Dumps all the databases of a Drupal installation. Alternative to the
            core sql-dump command, with support for table name expansion and ]
            validation.

- dgb-commit: Commits Drupal installation directory including the files
              directory of all sites, as well as the SQL dumps directory if they
              are located in the same master backup directory (typically one
              level up DRUPAL_ROOT).

- dgb-usage: Displays some information on the current size of the repository and
             the Drupal installation.

- dgb-backup: Wrapper command to dump, commit and display the size of the Drupal
              installation and repository


Author
======

Stéphane "scor" Corlosquet <scorlosquet@gmail.com>

References
==========

This drush command uses a similar approach to the one used in dbscripts
<http://drupal.org/project/dbscripts> except it is fully integrated in drush as
opposed to using custom bash scripts.
