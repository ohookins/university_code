#!/usr/bin/perl

use CGI qw/:standard/;
use LWP::Simple;
use XML::DOM;

my $topcgi = new CGI;

# passed parameters to register this user
my $username = $topcgi->param('user');
my $password = $topcgi->param('pass');

$src = './sources.dat'; # file containing source urls
my @sitelist;	# array for checkbox creation

open(SOURCE, $src) # we're going to parse it for checkbox creation
  or die ("Unable to read sources from file\n");
while($line = <SOURCE>){
  chomp $line;
  ($parm1, $parm) = split("�", $line);
  push(@sitelist, "$parm1");
}

print $topcgi->header, $topcgi->start_html('Registration');
print $topcgi->h2("Registration Details<br><br>\n");

print $topcgi->start_form(-method=>'POST',
  -action=> "http://groove.cm.nu/cgi-bin/comp249/update.pl");

print "Username: ",
  $topcgi->textfield(-name=>'user', -default=>"$username",
  -size=>30, -maxlength=>30), "<br>\n";
print "Password: ",
  $topcgi->password_field(-name=>'pass', -value=>"$password",
  -size=>30, -maxlength=>30), "<br>\n";
print "Full Name: ",
  $topcgi->textfield(-name=>'name', -size=>50, -maxlength=>50), "<br>\n";
print "<br>Sites:<br>",
  $topcgi->checkbox_group(-name=>'sites',-values=>\@sitelist,
  -default=>\@sitelist, -linebreak=>'true'), "<br>\n";

print $topcgi->submit(-name=>'button', -value=>'Submit'), "<br><hr>\n";

print $topcgi->endform;

print $topcgi->end_html;
