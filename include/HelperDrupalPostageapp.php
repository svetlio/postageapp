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
   * Concatenate and wrap the e-mail body for HTML mails.
   *
   * @param array $message
   *   A message array, as described in hook_mail_alter() with optional
   *   parameters described in mimemail_prepare_message().
   *
   * @return array
   *   The formatted $message.
   */
  public function format(array $message) {
    if (is_array($message['body'])) {
      $message['body'] = implode("\n\n", $message['body']);
    }

    if (preg_match('/plain/', $message['headers']['Content-Type'])) {
      $message['body'] = check_markup($message['body'], 'full_html');
    }

    return $message;
  }
  
  /**
   * Overrides DefaultMailSystem::mail().
   *
   */
  public function mail(array $message) {
    $variables = array();
    //dpm($message);
    //
    if ($message['module'] == 'postageapp_rules') {
      $message['subject'] = $message['params']['subject'];
      $message['body'] = $message['params']['message'];
    }
    
    $variables['submitted'] = array();
    $form_id = $_POST['form_id'];
    
    // get settings for form/tpl
    $pa_vars = unserialize(variable_get('postageapp_forms', ''));

    // 
    if (!empty($pa_vars[$form_id]['pa_template'])) {
      // get Pa tpl for this form
      $mail_body['template'] = $pa_vars[$form_id]['pa_template'];
    }
    
    // check for rules pa_template
    // rules template override form template if any
    if (!empty($message['params']['pa_template'])) {
      $mail_body['template'] = $message['params']['pa_template'];
    }
    
    // use default template if none set
    if (empty($mail_body['template']) && variable_get('postageapp_template_default', '')) {
      $mail_body['template'] = variable_get('postageapp_template_default', '');
    }
    // use PA to just resend email as is
    else {
      $mail_body['content'] = $message['body'];
    }
    
    // prepare variables for Pa template
    $variables['submitted'] = postageapp_variables_submitted($_POST, $message);

    // webform $_POST['submitted']
    if (strpos($form_id, 'webform_client_form_') === 0) {
      $variables['submitted'] = array_merge($variables['submitted'], $_POST['submitted']);
    }

    // add all keys
    $keys = array_keys($variables['submitted']);
    $variables['submitted']['keys'] = implode(', ', $keys);

    //
    $to = $message['to'];
    $subject = $message['subject'];
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
