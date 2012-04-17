Drupal Git Backup (DGB)
=======================

DGB is a Drush command to easily maintain a full backup a Drupal code base and
all its sites, keeping Drupal files and sites databases together in sync in the
same repository. It supports multi site setups and will dump each site database
as a version control friendly SQL file. This command leverages git as a powerful
(decentralized) version control system to store all websites' data efficiently.
This Drush command is still at an experimental state so use with caution. Please
give any feedback or file bugs at http://github.com/scor/dgb

DGB requires Drush <http://drupal.org/project/drush> and works for any version
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
                --> .git (dgb git repository)

1. create the dgb_instance folder somewhere on your server (the name does not
   matter). Make sure it's outside the web public directory.
2. place your drupal code base in the dbg_instance directory, e.g. in a drupal
   directory.
3. create the 'databases' folder (that is the default dump director that dgb
   expect, you can change that in drushrc.php, see below).
6. initialize the git repository in the dgb_instance folder by running git init.
   You might want to create a .gitignore file to ignore certains files, for
   example .svn if you version your files with svn.
7. from the DRUPAL_ROOT directory, type drush dgb-backup to see if dgb was
   installed successfully. This will do the initial backup of your dgb_instance,
   this initial backup might take a while depending on the number of sites you
   have and their size.
8. setup a cron job to run the backup every day or so, e.g. to run dgb every day
   at 11am:
   00 11 * * * cd /var/sites/dgb_instance/drupal; /usr/local/bin/drush dgb-backup
   (adapt /usr/local/bin/drush to wherever your drush is located on your server)

9. The following steps are optional. They allow to customize the names of the
   directories you want to use etc.
   copy example-dgb.drushrc.php in each site settings directory (.e.g
   sites/default/), rename it to drushrc.php and check its content.
   Its default settings should works for most configurations.


Commands
========

dgb ships with several commands:

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
