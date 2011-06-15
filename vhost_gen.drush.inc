<?php

/**
 * @file
 *   A drush command generating Apache VirtualHost configuration file.
 *
 * @author
 *   Martin Martinov <martin@mmartinov.com>
 */

/**
 * Implementation of hook_drush_command().
 *
 * @return
 *   An associative array describing our command(s).
 */
function vhost_gen_drush_command() {
  $items = array();

  // The 'vhost-generate' command
  $items['vhost-generate'] = array(
    'description' => "Generate Apache 2 VirtualHost configuration.",
    'arguments' => array(
      'server-name' => 'Value to use for ServerName, e.g. example.com.',
    ),
    'options' => array(
      'server-admin=admin@example.com' => 'Value to use for ServerAdmin (email address).',
      'document-root=path' => 'Path to DocumentRoot. Default is /var/www/[server-name].',
      'log-level=level' => 'Possible values: debug, info, notice, warn, error,
crit, alert, emerg. Default is warn.',
    ),
    'examples' => array(
      'drush vhg example.com' => 'Generate a VirtualHost for example.com.',
      'drush vhg example.com --server-admin=admin@example.com --log-level=error'
        => 'Adds ServerAdmin line to the generated configuration file and
sets the LogLevel to error.',
    ),
    'aliases' => array('vhg'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap at all.
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function vhost_gen_drush_help($section) {
  switch ($section) {
    case 'drush:vhost-generate':
      return dt("A command to generate Apache 2 VirtualHost configurations.
See README.txt for real-life usage example.");
  }
}

/**
 * Drush command callback. This is where the action takes place.
 * 
 * @todo
 *   Validation for file names and paths.
 * @todo
 *   Check if the value for LogLevel param is valid.
 *
 * @param $server_name
 *   A string with value for ServerName line.
 */
function drush_vhost_generate($server_name) {
  if (!isset($server_name)) {
    drush_set_error('DRUSH_VHOST_GEN_ARG_FAIL',
      dt('You have to provide an argument for ServerName.'));
  }
  else {
    $options = array(
      'ServerName' => $server_name,
      'ServerAdmin' => drush_get_option('server-admin', ''),
      'DocumentRoot' => drush_get_option('document-root',
        '/var/www/' . $server_name),
      'LogLevel' => drush_get_option('log-level', 'warn'),
    );
    drush_vhost_gen_generate($options);
  }
}

/**
 * Generate the config file using the template.
 * 
 * @todo
 *   Add support for ServerAlias
 *
 * @param $options
 *   An array with values for different options.
 */
function drush_vhost_gen_generate($options) {
  $vhost_cfg = file_get_contents(dirname(__FILE__) . '/vhost.tpl.txt');

  if (!empty($options['ServerAdmin'])) {
    $options['ServerAdmin'] = "ServerAdmin {$options['ServerAdmin']}" . PHP_EOL;
  }
  $vhost_cfg = str_replace('!ServerAdmin', $options['ServerAdmin'], $vhost_cfg);
  unset($options['ServerAdmin']);
  
  $needles = array();
  $replacements = array();
  foreach ($options as $option_name => $option_value) {
    $needles[] = "!{$option_name}";
    $replacements[] = $option_value;
  }
  $vhost_cfg = str_replace($needles, $replacements, $vhost_cfg);

  drush_print($vhost_cfg);
}
