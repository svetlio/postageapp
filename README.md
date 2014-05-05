Drupal-7 module for PostageApp
==============================

Allows you to using the PostageApp API to send mail. We can send mails without own mail server as sendmail, ssmtp or other. Instead, module send all outgoing site emails to Postageapp. How the module send messages to postageapp without real mail server - you can see in module code.<br>There we may use templates to get html on the messages before they send to their recepients.<p> <b>Important for settings!</b> You need to make template before using it. See Postageapp docs for this.

Instalation
===========

Install module as usually.

Add

$conf['mail_system']['default-system'] = 'PostageappDrupalMail';

to your settings.php

Visit admin/config/services/postageapp to enter yorp PA API key.
Visit admin/config/services/postageapp/forms_vars to edit/set PA template per form on the site.

Usage
=====
Add

{{ keys }}

on PA template to see all variables for the form. Then send test e-mail.


Questions / Comments?
---------------------

Help about PostageApp: [http://help.postageapp.com](http://help.postageapp.com)

Author
======

Drupal 7 developer [https://github.com/svetlio/](svetlio)

Initial development for Drupal-6 on [https://github.com/postageapp/postageapp-drupal](postageapp) by [https://github.com/stephentwg/](stephentwg)
