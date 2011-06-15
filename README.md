Drush command generating Apache VirtualHost configurations.
===========================================================

General Information
-------------------

My initial idea was to create a [drush](http://drupal.org/project/drush) script
that generates a config file, copies this file to /etc/apache/sites-available,
then runs a2ensite and finally restarts apache.

Of course power-user privileges are needed for this and according to
[this issue](http://drupal.org/node/799804) it is not a good idea to run drush
as root.

So what I do now is the following:

    drush vhg test.local > test.conf
    su -
    mv test.conf /etc/apache/sites-available/test.conf
    a2ensite test.conf && apache2ctl graceful

Now I'm looking for a way to automate this so it's only one step.
A bash script will do the job.

Installation
------------

After you download/clone this project place the *vhost_gen* directory to any of
the following:

* A .drush folder in your HOME folder.
* Anywhere in a folder tree below an active module on your site.
* /usr/share/drush/commands (configurable)
* In an arbitrary folder specified with the --include option.

If you choose the last option, to run the command execute:
`drush --include=/path/to/vhost_gen vhg`

See **drush help vhost-generate** for list of available options.
