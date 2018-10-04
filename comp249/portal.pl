#!/usr/bin/perl

use CGI qw/:standard/;
use LWP::Simple;
use XML::DOM;

my $topcgi = new CGI;
my $state = "";
$state = $topcgi->param('mystate');
if($state eq ""){	# set initial state of script
  $state = "Logout";
}

my $username = "";  #settings for current user
my $password = "";
my $fullname = "";
my @sitelist;

$query = $topcgi->param('querystring');

my %sources;	# hash table holding source names and url's

$src = './sources.dat'; # file containing source url's
open(SOURCE, $src)
  or die ("Unable to read sources from file\n");
while($line = <SOURCE>){	# parse source file
  chomp $line;
  ($parm1, $parm2) = split("¯", $line);
  $sources{$parm1} = $parm2;	# name is the key, url is the val
}

my ($thisurl, $thistitle);

# tests to see if a search is being performed
    if($state eq "Search"){
      print $topcgi->header, $topcgi->start_html('Query Results');
      print $topcgi->h1("Query Results<br>\n");
      print qq(
	<form action="http://groove.cm.nu/cgi-bin/comp249/portal.pl"
	method="post">
	<p align="left">
	<input type="submit" name="mystate" value="Story Listing">
	</p><hr>\n);

      print qq(<table border="1" align="center">\n);

      foreach $key (keys %sources){
	$thisurl = $sources{$key};
	$thistitle = $key;
	&searchData;
      }

      print qq(</table><hr>\n);
      print $topcgi->end_html;
    }
# checks if we are listing all [selected] channels
    elsif($state eq "Story Listing"){
      print $topcgi->header, $topcgi->start_html('Story Listing');
      print $topcgi->h1("Story Listing<br>\n");

      # set up the searching query box and button
      print qq(
	<form action="http://groove.cm.nu/cgi-bin/comp249/portal.pl"
	method="post">
	<p align="left">Search String: 
	<input type="text" name="querystring" maxlength="30" size="20">
	<input type="submit" name="mystate" value="Search">
	</p><hr>\n);

      foreach $key (keys %sources){
	$thisurl = $sources{$key};
	$thistitle = $key;
	&updateData;	# update data sources first
	&displayData;	# then display all of it
      }

      print $topcgi->end_html;
    }
# check for non-logged in status
    elsif($state eq "Logout"){
      print $topcgi->header, $topcgi->start_html('Portal Login');
      print $topcgi->h2("Portal Login<br>\n");

      # set up login text boxes and button
      print qq(
	<form action="http://groove.cm.nu/cgi-bin/comp249/portal.pl"
	method="post">
	<p align="left">Username: 
	<input type="text" name="entryuser" maxlength="25" size="20">
	<br>Password: 
	<input type="password" name="entrypass" maxlength="25" size="20">
	<br><br><input type="submit" name="mystate" value="Login">
	<br><hr>\n);
      print $topcgi->end_html;
    }

# check for login - username and password
    elsif($state eq "Login"){
      my $thisuser = $topcgi->param('entryuser');
      my $thispass = $topcgi->param('entrypass');

      my $initfile = 0;	# shows no existing user file
      open(USER, './user.dat')	# user info file for reading
	or $initfile = 1;

      if($initfile = 0){	# if the file already exists
	while($line = <USER>){	#parse user file
	  chomp $line;
	  ($parm1, $parm2, $parm3, $parm4) = split("¯", $line);

	  if(($parm1 eq $thisuser) && ($parm2 ne $thispass)){
	    $topcgi->param(-name=>'mystate',-value=>'Logout');
	    $myself = $topcgi->self_url;
	    print $topcgi->redirect($myself);
	  }
	  elsif(($parm1 eq $thisuser) && ($parm eq $thispass)){
	    $username = $parm1;
	    $password = $parm2;
	    $fullname = $parm3;
	    @sitelist = split(/,/, $parm4);
	    $topcgi->param(-name=>'mystate',-value=>'Story Listing');
	    $myself = $topcgi->self_url;
	    print $topcgi->redirect($myself);
	  }
	}
	# by this point, user was not found in the database
	close(USER);
	$username = $thisuser;
	$password = $thispass;
	$topcgi->param(-name=>'mystate',-value=>'Update Settings');
	$myself = $topcgi->self_url;
	print $topcgi->redirect($myself);
	}
      else{
	$username = $thisuser;
	$password = $thispass;
	$topcgi->param(-name=>'mystate',-value=>'Update Settings');
	$myself = $topcgi->self_url;
	print $topcgi->redirect($myself);
      }
    }
    elsif($state eq "Update Settings"){
      my $thisuser = $topcgi->param('entryuser');
      my $thispass = $topcgi->param('entrypass');

#############################################################
      print $topcgi->header, $topcgi->start_html('Update Settings');
      print $topcgi->h2("Update Settings<br><br>\n");

      #set up detail boxes
      print qq(
	<form action="http://groove.cm.nu/cgi-bin/comp249/portal.pl"
	method="post">
	<p align="left">Username: 
	<input type="text" name="userset" maxlength="25"
	size="20" value=


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
	  <a href="$parm3">$parm2</a><br>\n));
	# NOTE: channel description is not used at this point
      }
      elsif($parm1 eq "image"){
	print qq(<a href="$parm4">
	  <img border="0" src="$parm3" alt="$parm2"></a><br><br>\n);
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

# UPDATEDATA - loads existing data from files, and adds to it
# with updated data from live sources.
sub updateData{
    my $data = get($thisurl);	# retrieve RSS data
    my $file = "./$thistitle.dat";	# the data storage
    my %dathash; # keeps data prior to file storage

    my ($chtitle, $chlink, $chdesc); #channel values for storage
    my ($imtitle, $imurl, $imlink); #image values for storage

    my $initial = 0;	# checking if the file exists already
    open(STORE, $file)
      or $initial = 1;

    if($initial == 1){
      open(STORE, ">$file");	#doesn't exist, write to it!
    }
    else{		#file exists; parse it!
      while($line = <STORE>){	# parse line at a time
	chomp $line;
	($parm1, $parm2, $parm3, $parm4) = split("¯", $line);

	# strip first two lines - for channel and image data
	if($parm1 eq "channel"){
	  $chtitle = $parm2;	# glean channel data
	  $chlink = $parm3;
	  $chdesc = $parm4;
	}
	elsif($parm1 eq "image"){
	  $imtitle = $parm2;	#glean image data
	  $imurl = $parm3;
	  $imlink = $parm4;
	}
	elsif($parm1 eq "item"){	# put item data in a hash table
	  $dathash{$parm3} = $parm2;
	# Links are stored as keys - they will be unique
	# ...also Parm4 for items is a dummy field
	}
      }
      close(STORE);	# re-open file to write
      open(STORE, ">$file");
    }
    #finished parsing/opening storage file!

    #begin parsing potentially new data
    my $parser = new XML::DOM::Parser;
    my $doc = $parser->parse($data)
      or die ("Unable to parse data\n");

    my $nodes = $doc->getElementsByTagName("*");

    my ($i, $j);
    my ($nodename, $elem, $kids, $child);
    my ($nodevals, $val);
    my ($itmtitle, $itmlink, $itmdesc, $itmurl);
    my $newitem = 0;

    for $i (0 .. ($nodes->getLength - 1)){
      $nodevals = "";

      $elem = $nodes->item($i);
      if ($elem->getNodeType == ELEMENT_NODE) {
	$nodename = $elem->getTagName;
      }

      # then find their children
      if ($elem->hasChildNodes){
	$kids = $elem->getChildNodes($i);
	for $j (0 .. ($kids->getLength - 1)){
	  $child = $kids->item($j);
	  if ($child->getNodeType == TEXT_NODE){
	    $val = $child->getNodeValue;
	    # only print this if not an empty string
	    if (($val) && ($val =~ /^\w/m)) {
	      $nodevals .= "$val ";
	    }
	  }
	}
      }

      #capture the link data
      if(($nodename eq "item") && ($newitem == 0)){
	$newitem = 10;
	$itmtitle = "";
	$itmlink = "";
      }
      elsif(($nodename eq "title") && ($newitem == 10)){
	$newitem = 11;
	$itmtitle = $nodevals;
      }
      elsif(($nodename eq "link") && ($newitem == 11)){
	$newitem = 12;
	$itmlink = $nodevals;
      }
      elsif(($nodename eq "channel") && ($newitem == 0)){
	$newitem = 20;
	$itmtitle = "";
	$itmlink = "";
	$itmdesc = "";
      }
      elsif(($nodename eq "title") && ($newitem == 20)){
	$newitem = 21;
	$itmtitle = $nodevals;
      }
      elsif(($nodename eq "link") && ($newitem == 21)){
	$newitem = 22;
	$itmlink = $nodevals;
      }
      elsif(($nodename eq "description") && ($newitem == 22)){
	$newitem = 23;
	$itmdesc = $nodevals;
      }
      elsif(($nodename eq "image") && ($newitem == 0)){
	$newitem = 30;
	$itmtitle = "";
	$itmurl = "";
	$itmlink = "";
      }
      elsif(($nodename eq "title") && ($newitem == 30)){
	$newitem = 31;
	$itmtitle = $nodevals;
      }
      elsif(($nodename eq "url") && ($newitem == 31)){
	$newitem = 32;
	$itmurl = $nodevals;
      }
      elsif(($nodename eq "link") && ($newitem == 32)){
	$newitem = 33;
	$itmlink = $nodevals;
      }

      #construct the title/article/image link
      if($newitem == 12){
	$dathash{$itmlink} = $itmtitle;
	$newitem = 0;
      }
      elsif($newitem == 23){
	if($initial==1){
          # save channel data for storage
	  $chtitle = $itmtitle;
	  $chlink = $itmlink;
	  $chdesc = $itmdesc;
	}
	$newitem = 0;
      }
      elsif($newitem == 33){
	if($initial==1){
	  # save image data for storage
	  $imtitle = $itmtitle;
	  $imurl = $itmurl;
	  $imlink = $itmlink;
	}
	$newitem = 0;
      }
    }
    # finished parsing live data, start saving it

    # firstly store the channel and image data - easy stuff...
    print STORE "channel¯$chtitle¯$chlink¯$chdesc\n";
    print STORE "image¯$imtitle¯$imurl¯$imlink\n";

    # next, retrieve the item keys, sort and store them
    foreach $key (sort keys %dathash){
      $value = $dathash{$key};
      print STORE "item¯$value¯$key¯dummy\n";
    }
    # the links are now in ascending alpha-numeric order

    close(STORE); # close the storage file
}
