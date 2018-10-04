#!/usr/bin/perl

use CGI qw/:standard/;
use LWP::Simple;
use XML::DOM;

my $topcgi = new CGI;

# passed parameters from register or prefs to be updated on disk
my $username = $topcgi->param('user');
my $password = $topcgi->param('pass');
my $fullname = $topcgi->param('name');
my @sitetemp = $topcgi->param('sites');

# must convert site selection into storable format
my $sitelist;
my $i = 0;
foreach(@sitetemp){
  if($i != 0){ $sitelist .= ","; }
  $sitelist .= "$sitetemp[$i++]";
}

# make hash tables for reconstucting the user.dat file
my %pwords;
my %fnames;
my %slists;

my $nofile = 0;
my $file = "./user.dat";
open(USERS, "$file")
 or $nofile = 1;

if($nofile == 0){
  while($line = <USERS>){
    chomp $line;
    ($parm1, $parm2, $parm3, $parm4) = split("¯", $line);

    # store passes,names and sites into hash tables ref'd by username
    $pwords{$parm1} = $parm2;
    $fnames{$parm1} = $parm3;
    $slists{$parm1} = $parm4;
  }
}

# add in the new data
$pwords{$username} = $password;
$fnames{$username} = $fullname;
$slists{$username} = $sitelist;
# note that if a user updates their information, the old info
# is destroyed due to overwriting the old key

close(USERS);
$file = ">./user.dat";
open(USERS, $file);	# reopen file for writing

# now to write the data back to the file, why not sort it too?
my ($thispass, $thisname, $thissite);
foreach $key (sort keys %pwords) {
  $thispass = $pwords{$key};
  $thisname = $fnames{$key};
  $thissite = $slists{$key};
  print USERS "$key¯$thispass¯$thisname¯$thissite\n";
}
# all finished writing data, close file and back to listing
close(USERS);

$fullname =~ tr/ /,/;
print $topcgi->redirect(
  "http://groove.cm.nu/cgi-bin/comp249/list.pl?user=$username\&pass=$password\&name=$fullname\&sites=$sitelist");
