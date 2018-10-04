#!/usr/bin/perl

use CGI qw/:standard/;
use LWP::Simple;
use XML::DOM;

my $topcgi = new CGI;

# login details to be checked, passed from portal.pl
my $user = $topcgi->param('username');
my $pass = $topcgi->param('password');

# other user details to be passed to appropriate script
my $name;
my $sites;

my $nofile = 0;
my $file = './user.dat';
open(USERS, $file)
  or $nofile = 1;

if($nofile == 0){
while($line = <USERS>){		# parse user file
  chomp $line;
  ($parm1, $parm2, $parm3, $parm4) = split("¯", $line);

  if(($parm1 eq $user) && ($parm2 ne $pass)){
    print $topcgi->redirect('http://groove.cm.nu/cgi-bin/comp249/portal.pl');
    #login was incorrect, start from the beginning
  }
  elsif(($parm1 eq $user) && ($parm2 eq $pass)){
    $name = $parm3;
    $name =~ tr/ /,/;

    $sites = $parm4;

    print $topcgi->redirect(
    "http://groove.cm.nu/cgi-bin/comp249/list.pl?user=$user\&pass=$pass\&name=$name\&sites=$sites");
    # if login was correct, save the user details and pass off to list.pl
  }
}
}

print $topcgi->redirect("http://groove.cm.nu/cgi-bin/comp249/register.pl?user=$user\&pass=$pass");

# the username was not found, create a new account

