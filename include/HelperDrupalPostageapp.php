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
    // override body content to PA template
    $message['body'] = 'sample_child_template';
    
    $pa_vars = unserialize(variable_get('postageapp_forms', ''));
    
    // use drupal-default template if sett default
    if (variable_get('postageapp_forms_default', '')) {
      $message['body'] = 'drupal-default';
    }
    else {
      if (!empty($pa_vars[$fid]['pa_template'])) {
        // get Pa tpl for this form
        $message['body'] = $pa_vars[$_POST['form_id']]['pa_template'];
      }
    }
    
    // prepare variables for Pa template
    $variables['submitted'] = postageapp_variables_submitted($_POST);
    
    // webform $_POST['submitted']
    if (strpos($_POST['form_id'], 'webform_client_form_') === 0) {
      $variables['submitted'] = array_merge($variables['submitted'], $_POST['submitted']);
    }
    
    //
    $to = $message['to'];
    $subject = $message['subject'];
    $mail_body = $message['body'];
    $header = array(
      'X-Mailer' => 'Drupal 7',
      'From' => $message['headers']['From'],
      'Reply-to' => $message['headers']['Sender']
    );
    
    //
    $q = new PostageApp(variable_get('postageapp_api_key', '')); 
    $return = $q->mail(
      $to, 
      $subject,
      $mail_body,
      $header,
      $variables
    );
    
    if ($return -> response -> status == 'ok') {
      $w_mess = '<br/><b>SUCCESS:</b>, An email was sent and the following response was received: ' . serialize($return -> data -> message);
    } else {
      $w_mess = '<br/><b>Error sending your email:</b> ' . $return -> response -> message;
    }
    
    //
    watchdog ('postageapp', $w_mess);
    
    return TRUE;
  }
}
