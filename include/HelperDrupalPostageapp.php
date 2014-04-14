<?php

/**
 * Extend drupal mail system to put messages to Postageapp
 *
 */

/**
 * Provides a mail system class.
 *
 * Usage in settings.php:
 * @code
 * $conf['mail_system']['default-system'] = 'PostageappDrupalMail';
 * @endcode
 */
class PostageappDrupalMail extends DefaultMailSystem {

  /**
   * Overrides DefaultMailSystem::mail().
   *
   */
  public function mail(array $message) {
    $variables = array();
    $variables['submitted'] = array();
    
    // override default Drupal header Content-type for mail.
    $message['headers']['Content-Type'] = 'Content-Type: text/html; charset=UTF-8';
    
    // use drupal-default template if sett default
    if (variable_get('postageapp_forms_default', '')) {
      $variables['postageapp_template'] = 'drupal-default';
    }
    else {
      $fid = $_POST['form_id'];
      $pa_vars = unserialize(variable_get('postageapp_forms', ''));
      
      // get Pa tpl for this form
      $variables['postageapp_template'] = $pa_vars[$fid]['pa_template'];
      
      // webform $_POST['submitted']
      // prepare variables for Pa template
      if (strpos($_POST['form_id'], 'webform_client_form_') === 0) {
        $variables['submitted'] = $_POST['submitted'];
      }
      else {
        $variables['submitted'] = $_POST;
      }
    }
    
    $q = new PostageApp(variable_get('postageapp_api_key', '')); 
    $return = $q->mail(
      $message['to'], 
      $message['subject'],
      $message['body'],
      $message['headers'],
      $variables
    );
    
    return TRUE;
  }
}
