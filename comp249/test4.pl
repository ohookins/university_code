#!/usr/bin/perl

use CGI qw/:standard/;
use LWP::Simple;
use XML::DOM;

my $cgi = new CGI;
print $cgi->header, $cgi->start_html('Portal Page');

my ($thisurl, $thistitle);

my @urls = (
	"http://slashdot.org/slashdot.rdf",
	"http://www.newsforge.com/newsforge.rdf",
	"http://freshmeat.net/backend/fm.rdf",
	"http://www.webreference.com/webreference.rdf",
	"http://www.advogato.org/rss/articles.xml",
	"http://www.wired.com/news_drop/netcenter/netcenter.rdf",
	"http://www.cnn.com/cnn.rss"
	);

my @titles = (
	"slashdot",
	"newsforge",
	"freshmeat",
	"webreference",
	"advogato",
	"wired",
	"cnn",
	);

for($index = 0; $index < 7; $index++){
    $thisurl = $urls[$index];
    $thistitle = $titles[$index];
    &updateData;
}

print $cgi->end_html;

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
	($parm1, $parm2, $parm3, $parm4) = split(/|/, $line);

	# strip first two lines - for channel and image data
	if($parm1 eq "channel"){
	  $chtitle = $parm2;	# glean channel data
	  $chlink = $parm3;
	  $chdesc = $parm4;
	  print "$parm1 $parm2 $parm3 $parm4";
	}
	elsif($parm1 eq "image"){
	  $imtitle = $parm2;	#glean image data
	  $imurl = $parm3;
	  $imlink = $parm4;
	}
	elsif($parm1 eq "item"){	# put item data in a hash table
	  %dathash = ("$parm3" => "$parm2");
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
	print qq(<a href="$itmlink">$itmtitle</a><br>\n);
	%dathash = ("$itmlink"=>"$itmtitle");
	$newitem = 0;
      }
      elsif($newitem == 23){
	print $cgi->h3(qq(<a href="$itmlink">$itmtitle</a><br>\n));
	if($initial==1){
          # save channel data for storage
	  $chtitle = $itmtitle;
	  $chlink = $itmlink;
	  $chdesc = $itmdesc;
	}
	$newitem = 0;
      }
      elsif($newitem == 33){
	print qq(<a href="$itmlink"><img border="0" src="$itmurl" alt="$itmtitle"></a><br><br>\n);
	if($initial==1){
	  # save image data for storage
	  $imtitle = $itmtitle;
	  $imurl = $itmurl;
	  $imlink = $itmlink;
	}
	$newitem = 0;
      }
    }
    print "<br><hr>\n";

    # finished parsing live data, start saving it

    print STORE "channel|$chtitle|$chlink|$chdesc\n";
    print STORE "image|$imtitle|$imurl|$imlink\n";
}
