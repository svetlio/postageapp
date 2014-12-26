<?php
// $Id$

/**
 * Admin settings form for the PostageApp API.
 */

function postageapp_admin_form() {
  $form = array();
  
  $form['settings'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Postageapp settings'),
  );
  
  $form['settings']['postageapp_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('PostageApp API Key'),
    '#required' => TRUE,
    '#default_value' => variable_get('postageapp_api_key', ''),
    '#description' => t('The API key for your PostageApp account. Get or generate a valid API key at your PostageApp dashboard which is available at yourusername.postageapp.com')
  );
  
  return system_settings_form($form);
}

/**
 * Postageapp settings form validate.
 */
function postageapp_admin_form_validate($form, &$form_state) {  
  //
  $api_key = $form_state['values']['postageapp_api_key'];
  
  if ($api_key) {
    $postage = new PostageApp($api_key);
    $valid_key = $postage->get_project_info();
    
    if ($valid_key->response->status == 'ok') {
      drupal_set_message(t('Your API key is valid.  Welcome to PostageApp.'), "status");
      drupal_set_message(t('To complete settings add "$conf[\'mail_system\'][\'default-system\'] = \'PostageappDrupalMail\';" to settings.php (without duoble quotes). This settings override default mail system in Drupal to use Postageapp as mail delivery service.'), 'info');
    }
    else {
      form_set_error('postageapp_api_key', t($valid_key->response->message));
    }
  }
}

function postageapp_page_forms_vars($pa_fid = NULL, $op = NULL) {
  $output = '';
  
  $output .= t('Settings page for Postageapp service to use template per form. See <a href="@pa_help">help page</a> for more details.', array('@pa_help' => '/admin/help/postageapp'));
  $form = drupal_get_form('postageapp_forms_vars_form', $pa_fid);
  
  $output .= drupal_render($form);
  
  if ($op == 'delete' && $pa_fid) {
    $vars = unserialize(variable_get('postageapp_forms', ''));
    unset($vars[$pa_fid]);
    
    variable_set('postageapp_forms', serialize($vars));
    
    drupal_set_message('@pa_fid form settings for postageapp deleted.', array('@pa_fid' => $pa_fid));
  }
  
  $output .= pa_forms_settings_list();
  
  return $output;
}

function postageapp_forms_vars_form($form, &$form_state, $pa_fid = NULL) {
  $form = array();
  
  // example data for contact form, also used as default value
  $pa_forms = array(
    'contact_site_form' => array(
      'pa_vars' => 'name, mail, subject, message, copy',
      'pa_template' => 'test-drupal7-dev',
    )
  );
  
  if (variable_get('postageapp_forms', '') == FALSE) {
    variable_set('postageapp_forms', serialize($pa_forms));
  }
  else {
    $pa_forms = unserialize(variable_get('postageapp_forms', ''));
    //dpm($pa_forms);
    if (!empty($pa_forms[$pa_fid])) {
      $pa_forms = array(
        $pa_fid =>$pa_forms[$pa_fid]
      );
    }
  }
  
  $keys = array_keys($pa_forms);
  
  $form['default'] = array(
    '#type' => 'fieldset',
    '#title' => 'default',
  );
  $form['default']['postageapp_template_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Default template'),
    "#description" => t('Postageapp template for emails without assigned template. Empty for none.'),
    '#default_value' => variable_get('postageapp_template_default', ''),
  );
  
  $form['fs'] = array(
    '#type' => 'fieldset',
    '#title' => 'Form settings',
  );
  $form['fs']['pa_fid'] = array(
    '#type' => 'textfield',
    '#title' => t('Form ID'),
    '#default_value' => $keys[0],
    '#description' => t('Leave the same to owerride existing record. Change the "Form ID" to add new record.'),
  );  
  $form['fs']['pa_template'] = array(
    '#type' => 'textfield',
    '#title' => t('Postageapp template'),
    '#description' => 'Enter TEMPLATE SLUG here as you naming template in your PA account.',
    '#default_value' => $pa_forms[$keys[0]]['pa_template'],
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  return $form;
}

function postageapp_forms_vars_form_validate($form, &$form_state) {
  if (empty($form_state['values']['pa_fid'])) {
    form_set_error('pa_fid', 'Field "Form ID"can\'t be empty.');
  }
  
  if (empty($form_state['values']['pa_template'])) {
    form_set_error('pa_template', 'Field "Postageapp template"can\'t be empty.');
  }
}

function postageapp_forms_vars_form_submit($form, &$form_state) {
  
  variable_set('postageapp_template_default', $form_state['values']['postageapp_template_default']);
  
  // collect data for save in variables
  $pa_forms = variable_get('postageapp_forms', '');
  
  // new form settings
  $new_pa_form = array(
    $form_state['values']['pa_fid'] => array(
      'pa_template' => $form_state['values']['pa_template'],
    ),
  );
  
  // add to existings settings
  if ($pa_forms) {
    $pa_forms = unserialize($pa_forms);
    $pa_forms = array_merge($pa_forms, $new_pa_form);
    
    variable_set('postageapp_forms', serialize($pa_forms));
  }
  else {
    variable_set('postageapp_forms', serialize($new_pa_form));
  }
}

//

function pa_forms_settings_list() {
  $output = '<hr>';
  $pa_forms = unserialize(variable_get('postageapp_forms', ''));
  
  foreach ($pa_forms AS $fid => $vars) {
    $list = '';
    $items = '';
    
    $items[] = 'Template: ' . $vars['pa_template'] . '<p>';
    $items[] = l(t('Clone / Edit'), 'admin/config/services/postageapp/forms_vars/' . $fid . '/clone') . ' | ' .
    l(t('Delete'), 'admin/config/services/postageapp/forms_vars/' . $fid . '/delete');
    
    $list['items'] = $items;
    $list['title'] = $fid;
    $list['type'] = 'ul';
    $list['attributes'] = array();
    
    $output .= theme_item_list($list);
  }
  
  
  return $output;
}
