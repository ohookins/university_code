#!/usr/bin/perl -w

use CGI qw/:standard/;
use LWP::Simple;
use XML::DOM;

my $topcgi = new CGI;

my %sources;	# hash table holding source names and url's

#firstly, update the data for the sources
my $src = './sources.dat'; # file containing source url's
open(SOURCE, $src)
  or die ("Unable to read sources from file\n");
while($line = <SOURCE>){	# parse source file
  chomp $line;
  ($parm1, $parm2) = split("¯", $line);
  $sources{$parm1} = $parm2;	# name is the key, url is the val
}

my ($thisurl, $thistitle);

foreach $key (keys %sources){
  $thisurl = $sources{$key};
  $thistitle = $key;
  &updateData;	# update data sources
}

# Display Login screen
print $topcgi->header, $topcgi->start_html('Portal Login');
print $topcgi->h2("Portal Login<br>\n");

# set up login text boxes and button
print qq(
  <form action="http://groove.cm.nu/cgi-bin/comp249/login.pl"
  method="post"> Username:
  <input type="text" name="username" maxlength="25" size="20">
  <br>Password: 
  <input type="password" name="password" maxlength="25" size="20">
  <br><br><input type="submit" name="button" value="Login">
  <br><hr>\n);
print $topcgi->end_html;

# UPDATEDATA - loads existing data from files, and adds to it
# with updated data from live sources.
sub updateData{
    my $data = get($thisurl);	# retrieve RSS data
    if(is_error($data)){ return -1;}
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
	$itmtitle = $nodevals;
      }
      elsif(($nodename eq "link") && ($newitem == 10)){
	$itmlink = $nodevals;
      }
      elsif(($nodename eq "channel") && ($newitem == 0)){
	$newitem = 20;
	$itmtitle = "";
	$itmlink = "";
	$itmdesc = "";
      }
      elsif(($nodename eq "title") && ($newitem == 20)){
	$itmtitle = $nodevals;
      }
      elsif(($nodename eq "link") && ($newitem == 20)){
	$itmlink = $nodevals;
      }
      elsif(($nodename eq "description") && ($newitem == 20)){
	$itmdesc = $nodevals;
      }
      elsif(($nodename eq "image") && ($newitem == 0)){
	$newitem = 30;
	$itmtitle = "";
	$itmurl = "";
	$itmlink = "";
      }
      elsif(($nodename eq "title") && ($newitem == 30)){
	$itmtitle = $nodevals;
      }
      elsif(($nodename eq "url") && ($newitem == 30)){
	$itmurl = $nodevals;
      }
      elsif(($nodename eq "link") && ($newitem == 30)){
	$itmlink = $nodevals;
      }

      #construct the title/article/image link
      if(($newitem == 10) && ($itmtitle ne "") && ($itmlink ne "")){
	$dathash{$itmlink} = $itmtitle;
	$newitem = 0;
      }
      elsif(($newitem == 20) && ($itmtitle ne "") && ($itmlink ne "") && ($itmdesc ne "")){
	if($initial==1){
          # save channel data for storage
	  $chtitle = $itmtitle;
	  $chlink = $itmlink;
	  $chdesc = $itmdesc;
	}
	$newitem = 0;
      }
      elsif(($newitem == 30) && ($itmtitle ne "") && ($itmlink ne "") && ($itmurl ne "")){
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
