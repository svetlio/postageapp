Settings

Added Rules Action "Postageapp send email". It is as "Send email" rules action
with addition 2 field - for Postageapp template for this email, and for variables
which send to Postageapp template with this email. We can use rules action "Add variable"
to create individual variables, and the using token to add them to the email.

For rules without template in their email action will use default template from
admin/config/services/postageapp/forms_vars. If there is empty, emails will just
resend to recepients from postageapp as is (html enabled by default).