#!/usr/bin/perl

use CGI;

$q=new CGI;
$array='';
print "Content-type: text/html\n\n";
foreach ($q->param) {
  $array=$array." ".$_;
}
print '<html><body bgcolor="white"><table border=1>';

print
  '<tr><td><center>Day</center></td><td><center>Month</center></td><td>
       <center>Time</center></td></td><td><center>What</center></td><td><center>Did</center></td></tr>';


open TAC, "tac /var/log/messages|";

while (<TAC>) {
  @line=split ' ';  
  $line[4] =~ s/\[\d*\]//;
  $line[4] =~ s/://;
  unless ($array =~ $line[4]){
    @Fld=split ' ';
    if ($line[4] =~ /kernel/) {
      $color="blue";
    }
    elsif ($line[4] =~ /PAM/) {
      $color="purple";
    }
    elsif ($line[4] =~ /login/){
	$color="red";
    }
    elsif ($line[4] =~ /pumpd/) {
      $color="green";
    }
    else {
      $color="black";
    }
    printf "<tr><td><font color=\"$color\">%-4s</font></td><td><font color=\"$color\">%-4s<td><font color=\"$color\"> %-9s</td><td><font color=\"$color\"> %-25s</td>", $Fld[1],$Fld[0], $Fld[2], $Fld[4];
    $Fld[$X] = $Fld[5].' '.$Fld[6].' '.$Fld[7].' '.$Fld[8].' '.$Fld[9].' '.$Fld[10].' '.$Fld[11].' '.$Fld[12].' '.$Fld[13].' '.$Fld[14].' '.$Fld[15].' '.$Fld[16].' '.$Fld[17].' '.$Fld[18].' '.$Fld[19];
    printf "<td><font color=\"$color\">%s</td></tr>", $Fld[$X];
    print "\n";
 
  }
}

print "</table>";

open TAC2, "/var/log/messages";
$array='';
while (<TAC2>) {
  @field=split(" ", $_, 9999);
  #operations on the Fifth (Ok, so it's not the fifth anymore) Element here
  $line=$field[4];
  $line =~ s/\[\d*\]//;
  $line =~s/://;
  unless($array =~ $line){
      $array = $array." ".$line;
    }
}

#------
print $q->startform(-action=>'filter.pl');

@list=split " ", $array;
foreach (@list) {
  print $q->checkbox(-name=>$_, -value=>1, -label=>$_);
}
print $q->submit;
 
print $q->endform;
print "</body></html>";


