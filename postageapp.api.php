<?php

/**
 * Documentation about hook_postageapp_vars
 *
 * @param $vars
 *  array of data to send to postageapp template
 *  array(
 *    'variable_1' => 'value_1,
 *    'variable_2' => 'value_2'
 *  )
 *  
 *  On Postageavv template is used by key, e.g.
 *  {{ variable_1 }}, {{ variable_2 }}
 *
 *  @param $post
 *  array of data sent by form (e.g. $_POST)
 */
function hook_postageapp_vars_alter(&$vars, $post, $message) {
  // add new var or change existing
  $vars['my_var_to_send_to_postagapp_template'] = 'value';
}