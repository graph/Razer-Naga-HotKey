#!/usr/bin/perl
use strict;

my $linesString;
my @lines;

# we set this so w will give us enough details
$ENV{"COLUMNS"} = 200;
$ENV{"PROCPS_USERLEN"} =16;

$linesString = `w -h -s`;

@lines = split("\n", $linesString);

my $i;
my $user;
$user = 0;
# find the user that is logged in to x
for($i= 0; $i < @lines; $i++){
	my $l;
	$l = @lines[$i];
	if($l =~ /xserver/ || $l=~ /session/){
		if($l =~ /^(\S+)/){
			$user = $1; 
		}
	}
}
if(!$user){
	exit(0);
}

print $user . "\n";
# now we start the deamon as the user

#my $homedir;
#$homedir = `getent passwd $user | cut -d ':' -f 6`;
#chomp $homedir;

my $deamon;
$deamon = "/usr/local/bin/nagad";

# the exec says dont start new process but take over existing one
exec("su $user -l -c 'exec $deamon'");
