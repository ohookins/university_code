#!/usr/bin/perl -w

use CGI qw/:standard/;
use LWP::Simple;
use XML::DOM;

my $cgi = new CGI;
print $cgi->header, $cgi->start_html('Portal Page');

my $url = "http://www.webreference.com/webreference.rdf";
my $data = get($url);

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
    $newitem = 0;
  }
  elsif($newitem == 23){
    print $cgi->h3(qq(<a href="$itmlink">$itmtitle</a><br>\n));
    $newitem = 0;
  }
  elsif($newitem == 33){
    print qq(<a href="$itmlink"><img border="0" src="$itmurl" alt="$itmtitle"></a><br><br>\n);
    $newitem = 0;
  }
}

print $cgi->end_html;
