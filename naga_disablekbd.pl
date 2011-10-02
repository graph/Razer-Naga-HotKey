#!/usr/bin/perl
use strict;

my $linesString = `xinput --list`;

my @lines = split("\n", $linesString);


for(my $i =0; $i < @lines; $i++){
	my $l = $lines[$i];
	if($l =~ /Razer Naga\s+id=(\d+).+keyboard/c){
		# found the keyboard input
		my $id = $1;
		system("xinput set-int-prop $id \"Device Enabled\" 8 0");
	}
}
