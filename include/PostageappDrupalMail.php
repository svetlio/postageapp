<?php

/**
 * Extend drupal mail system to put messages to Postageapp
 *
 */


/**
 * Provides a mail system class useful for debugging mail output.
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
   * Accepts an e-mail message and displays it on screen, and additionally logs
   * it to watchdog().
   */
  public function mail(array $message) {
    $header = "To: {$message['to']} <br />Subject: {$message['subject']}";
    $string = check_plain(print_r($message, TRUE));
    $string = '<pre>' . $string . '</pre>';
    
    //
    $q = new PostageApp(variable_get('postageapp_api_key', '')); 
    $return = $q->mail(
      $message['to'], 
      $message['subject'],
      $message['body'],
      $message['headers']
    );
    
    // Don't actually use debug to display the message since we want to be able
    // to categorize the watchdog type as 'mail' so it can be filterable in
    // UI.
    watchdog('mail', $header . ' ' . $string, NULL, WATCHDOG_INFO);
    
    return TRUE;
  }
}
