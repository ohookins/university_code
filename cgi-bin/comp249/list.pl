#!/usr/bin/perl -w

use CGI qw/:standard/;
use LWP::Simple;
use XML::DOM;

my $topcgi = new CGI;

# passed parameters for current user
my $username = $topcgi->param('user');
my $password = $topcgi->param('pass');
my $fullname = $topcgi->param('name');
my $sitetemp = $topcgi->param('sites');

(@sitelist) = split(/,/, $sitetemp);

my %sources;	# hash table holding selected source names and url's

$src = './sources.dat'; # file containing source url's

open(SOURCE, $src)	# retrieve sources from file
  or die ("Unable to read sources from file\n");
while($line = <SOURCE>){	# parse source file
  chomp $line;
  ($parm1, $parm2) = split("¯", $line);
  my $i = 0;
  foreach(@sitelist){
    if ($parm1 eq $sitelist[$i++]){
      $sources{$parm1} = $parm2;	#add site to list of sources
    }
  }
}
close(SOURCE);

my ($thisurl, $thistitle);

# Set up html page
print $topcgi->header, $topcgi->start_html('Story Listing');
print $topcgi->h1("Story Listing<br>\n");

# set up the searching query box and button
print qq(
  <form action="http://groove.cm.nu/cgi-bin/comp249/search.pl"
  method="post">
  <input type="hidden" name='user' value="$username">
  <input type="hidden" name='pass' value="$password">
  <input type="hidden" name='name' value="$fullname">
  <input type="hidden" name='sites' value="$sitetemp">
  <p align="left">Search String: 
  <input type="text" name="querystring" maxlength="30" size="20" align="left">
  <input type="submit" name="button" value="Search" align="left">
  </p></form>\n);

# set up button for changing preferences
print qq(
  <form action="http://groove.cm.nu/cgi-bin/comp249/prefs.pl"
  method="post">Update Preferences:
  <input type="submit" name="button" value="Preferences" align="left">
  <input type="hidden" name='user' value="$username">
  <input type="hidden" name='pass' value="$password">
  <input type="hidden" name='name' value="$fullname">
  <input type="hidden" name='sites' value="$sitetemp">
 </form>\n);

# and set up a button for loggin out
print qq(
  <form action="http://groove.cm.nu/cgi-bin/comp249/portal.pl"
  method="post">
  <input type="submit" name="button" value="Logout" align="right">
  </form><hr>\n);

# iterate thru list of selected sources and display them
foreach $key (keys %sources){
  $thisurl = $sources{$key};
  $thistitle = $key;
  &displayData;	# display this source's articles
}

print $topcgi->end_html;


# DISPLAYDATA - displays data from file storage only
sub displayData{
    my $file = "./$thistitle.dat";	# data storage location

    open(STORE, $file)	# open the file for reading
      or print "No news today<br>\n";

    while($line = <STORE>){	# start parsing data
      chomp $line;
      ($parm1, $parm2, $parm3, $parm4) = split("¯", $line);

      if($parm1 eq "channel"){
	print $topcgi->h2(qq(<p align="center">
	  <a href="$parm3">$parm2</a></p>\n));
	# NOTE: channel description is not used at this point
      }
      elsif($parm1 eq "image"){
	if(($parm3 eq "") && (($parm2 eq "") || ($parm4 eq ""))){
	  $parm2 = "No image available for this site...";
	  $parm4 = "http://groove.cm.nu/noimage.jpg";
	}
	if($parm3 eq ""){ $parm3 = "http://groove.cm.nu/noimage.jpg";}
	print qq(<a href="$parm4"><p align="center">
	  <img border="0" src="$parm3" alt="$parm2"></a></p>\n);
	# Assume there are no more channels or images to follow...

	print qq(</p><table align="center" border="1">\n);
	# Sets up a table for the items
      }
      elsif($parm1 eq "item"){
	print qq(<tr><td><a href="$parm3">$parm2</a></td></tr>\n);
	# NOTE: dummy 4th parameter is NOT printed
      }
    }
    print "</table><br><hr>\n";
}
