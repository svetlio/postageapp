<?php

function postageapp_rules_rules_action_info() {
  return array(
    'pa_mail' => array(
      'label' => t('Postageapp Send mail'),
      'group' => t('Postageapp'),
      'parameter' => array(
        'to' => array(
          'type' => 'text',
          'label' => t('To'),
          'description' => t('The e-mail address or addresses where the message will be sent to. The formatting of this string must comply with RFC 2822.'),
        ),
        'subject' => array(
          'type' => 'text',
          'label' => t('Subject'),
          'description' => t("The mail's subject."),
          'translatable' => TRUE,
        ),
        'message' => array(
          'type' => 'text',
          'label' => t('Message'),
          'description' => t("The mail's message body."),
          'translatable' => TRUE,
        ),
        'from' => array(
          'type' => 'text',
          'label' => t('From'),
          'description' => t("The mail's from address. Leave it empty to use the site-wide configured address."),
          'optional' => TRUE,
        ),
        'language' => array(
          'type' => 'token',
          'label' => t('Language'),
          'description' => t('If specified, the language used for getting the mail message and subject.'),
          'options list' => 'entity_metadata_language_list',
          'optional' => TRUE,
          'default value' => LANGUAGE_NONE,
          'default mode' => 'selector',
        ),
        'pa_template' => array(
          'type' => 'text',
          'label' => t('Postageapp Template'),
          'description' => t("Postageapp template to use for this e-mail."),
          'translatable' => TRUE,
        ),
        'pa_variables' => array(
          'type' => 'text',
          'label' => t('Postageapp Template Variables'),
          'description' => t('Postageapp template <b>variables</b> to use on template (above) for this e-mail. key|value pair, each on own row. Add Action element "Add a variable" before this action to define (not existins) variable value to use - see "Replacement patterns for Added variable" below. Use key on PA template to print value.'),
          'translatable' => TRUE,
        ),
      ),
      'base' => 'pa_rules_action_mail',
      'access callback' => 'rules_system_integration_access',
    ),
  );
}

function pa_rules_action_mail($to, $subject, $message, $from = NULL, $langcode, $pa_template, $pa_variables, $settings, RulesState $state, RulesPlugin $element) {
  
  $to = str_replace(array("\r", "\n"), '', $to);
  $from = !empty($from) ? str_replace(array("\r", "\n"), '', $from) : NULL;
  $params = array(
    'subject' => $subject,
    'message' => $message,
    'langcode' => $langcode,
    'pa_template' => $pa_template,
    'pa_variables' => $pa_variables,
  );
  // Set a unique key for this mail.
  $name = isset($element->root()->name) ? $element->root()->name : 'unnamed';
  $key = 'pa_rules_action_mail_' . $name . '_' . $element->elementId();
  $languages = language_list();
  $language = isset($languages[$langcode]) ? $languages[$langcode] : language_default();

  $message = drupal_mail('postageapp_rules', $key, $to, $language, $params, $from);
  if ($message['result']) {
    watchdog('rules', 'Successfully sent email to %recipient', array('%recipient' => $to));
  }
}
