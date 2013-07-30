This is an example of sending an email

void send_email(){
    //Connecting SMTP
    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);
    smtp.setUser("lucas.cmpt212@gmail.com");
    smtp.setPassword("1lucas23");

    //Setting up email
    MimeMessage message;
    message.setSender(new EmailAddress("lucas.cmpt212@gmail.com", "Lucas"));
    message.addRecipient(new EmailAddress("[YOUR EMAIL HERE]@sfu.ca", "You"));
    message.setSubject("Testing SMTP sending email");

    //Set body of email
    MimeText text;
    text.setText("Hi,\nJust testing out sending emails woohoo.\n");
    message.addPart(&text);

    //Connect, login, send email, and quit
    smtp.connect_and_send(message);
}
