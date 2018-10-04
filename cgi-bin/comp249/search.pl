#!/usr/bin/perl -w

use CGI qw/:standard/;
use LWP::Simple;
use XML::DOM;

my $topcgi = new CGI;

my $query = $topcgi->param('querystring');

# user details passing between scripts
my $username = $topcgi->param('user');
my $password = $topcgi->param('pass');
my $fullname = $topcgi->param('name');
my $sitetemp = $topcgi->param('sites');
(@sitelist) = split(/,/, $sitetemp);

my %sources;	# hash table holding source names and url's

$src = './sources.dat'; # file containing source url's
open(SOURCE, $src)
  or die ("Unable to read sources from file\n");
while($line = <SOURCE>){	# parse source file
  chomp $line;
  ($parm1, $parm2) = split("¯", $line);
  my $i = 0;
  foreach(@sitelist){
    if ($parm1 eq $sitelist[$i++]){
      $sources{$parm1} = $parm2;	# add site to list of sources
    }
  }
}
close(SOURCE);

my ($thisurl, $thistitle);

print $topcgi->header, $topcgi->start_html('Query Results');
print $topcgi->h1("Query Results<br>\n");
# button to link back to story page, sending hidden user data
print qq(
  <form action="http://groove.cm.nu/cgi-bin/comp249/list.pl"
  method="post">
  <p align="left">
  <input type="hidden" name='user' value="$username">
  <input type="hidden" name='pass' value="$password">
  <input type="hidden" name='name' value="$fullname">
  <input type="hidden" name='sites' value="$sitetemp">
  <input type="submit" name="button" value="Story Listing">
  </p></form><hr>\n);

print $topcgi->h3(qq(Query String: \"$query\" <br>\n));
print qq(<table border="1" align="center">\n);

foreach $key (keys %sources){
  $thisurl = $sources{$key};
  $thistitle = $key;
  &searchData;
}

print qq(</table><hr>\n);
print $topcgi->end_html;

# SEARCHDATA - searches the stored data, assuming it has been
# updated, for some search string, and prints the results
sub searchData{
    my $file = "./$thistitle.dat";	# file to be read from
    open(STORE, $file)  # open for reading only
     or print "No text to search<br>\n";

    while($line = <STORE>){	# parse the data
      chomp $line;
      ($parm1, $parm2, $parm3, $parm4) = split("¯", $line);

      if($parm1 eq "item"){
	# We throw away channel and image data... just use items
	$test1 = ($parm2 =~ m/$query/i); # test the title
	$test2 = ($parm3 =~ m/$query/i); # test the link
	if(($test1 == 1) || ($test2 == 1)){
	  # we want the item... print in a table
	  print qq(
	    <tr><td>$thistitle</td><td>
	    <a href="$parm3">$parm2</a></td></tr>\n);
	}
      }
    }
    close(STORE);
}
