# Alexander Cabos
# PA4 - pa4_Cabos.pl
# 3/22/23

use strict;
use warnings;
use utf8;
use Encode;
use feature 'say';

# Read the test data from the file
my $filename = 'control-char.txt';
open my $fh, '<:raw', $filename or die "Could not open file '$filename': $!";
my $data;
{
    local $/;
    $data = <$fh>;
}

#Fixes all the non-printable characters
my $fixed_data = $data;
$fixed_data =~ s/\x05\r\n//g;
$fixed_data =~ s/\x06\r\n //g;
$fixed_data =~ s/\r\n kf \x17\x12\'\r\n jf//g;
$fixed_data =~ s/\x03//g;
$fixed_data =~ s/ \x02//g;

#Convert to string to use regex
my $fixed_string = decode('utf-8', $fixed_data);

#Remove extra spaces between words
$fixed_string =~ s/[^\n\S]+/ /g;

#Remove extra letters at the end of a line
$fixed_string =~ s/\s[A-Z]\s\n/\n/g;

#Fix space in the middle of "line" on line 3, I couldn't figure out another way to get this done.
$fixed_string =~ s/i\s+n/in/g;

#Update file with corrected data
open $fh, '>', $filename or die "Could not open file '$filename': $!";
print $fh $fixed_string;
close $fh;
