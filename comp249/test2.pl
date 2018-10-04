#!/usr/bin/perl -w

# Ongoing test code for Assignment 2

use CGI qw/:standard/;
use XML::DOM;
use LWP::Simple;

&getData;

my $url = "http://www.slashdot.org/slashdot.rdf";

my $cgiobj = new CGI;
print $cgiobj->header,
      $cgiobj->start_html('Portal'),
      $cgiobj->h1('Come on baby, you know you`re the girl of my dreams...');

&initParser;
&parseData;

print $cgiobj->end_html;


sub getData {
  my $data = get($url);
  getprint($url);
} # getData

sub initParser{
  my $parser = new XML::DOM::Parser;

  # Parse the file and create the DOM tree
  my $doc = $parser->parse($data)
    or die ("Unable to parse data\n");

  my $nodes = $doc->getElementsByTagName("*");
} # initParser

sub parseData{
  my ($i, $j, $k);
  my ($elem, $kids, $child, $val, $nodename, $attrs);
  my ($nodeval, $attval);

  for $i (0 .. ($nodes->getLength - 1)){
    $nodeval = "";
    $attval = "";
    $elem = $nodes->item($i);
    if ($elem->getNodeType == ELEMENT_NODE) {
      $nodename = $elem->getTagName;
    }

# then find their children
  if ($elem->hasChildNodes){
    $kids = $elem->getChildNodes($j);
    for $j (0 .. ($kids->getLength - 1)){
      $child = $kids->item($j);
      if ($child->getNodeType == TEXT_NODE){
	$val = $child->getNodeValue;
	# only print this if not an empty string
	if (($val) && ($val =~ /^\w/m)) {
	  $nodevals .= "$val ";
	}
      }
      $attrs = $child->getAttributes;
      if ($attrs){
	for $k (0 .. ($attrs->geLength - 1)) {
	  $val = $attrs->item($k)->getNodeValue;
	  # only print if not an empty string
	  if (($val) && ($val = ~ /^\w/m)) {
	    $attval .= "$val ";
	  }
	}
      }
    }
  }
  if (($nodevals ne "") || ($attval ne "")){
    print "$attval $nodevals\n";
  }
  }
} # parseData
