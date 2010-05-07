Drupal Git Backup (DGB)
=======================

DGB is a Drush command allowing to maintain a full backup a Drupal site, keeping
both Drupal files and database together in the same repository. It supports
multi site setup and will dump each site database in a SQL dump in a versionable
fashion. This command leverages git as a (decentralized) version control system
to store all site's data efficiently. This command is still at an experimental
state so use with caution. Please give any feedback at
http://github.com/scor/dgb

DGB requires Drush 3 <http://drupal.org/project/drush> and works for any version
of Drupal 5, 6 and 7.

Installation
============

Install dgb like any other Drush command. See Drush readme.txt file.

Usage
=====

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

Type "drush" to get the list of commands
Tyep "drush help dgb-backup" for example to get more help on a dgb command.


Author
======

Stéphane "scor" Corlosquet <scorlosquet@gmail.com>
