#!/usr/bin/perl

use strict;
use warnings;
use CGI;

my $cgi = CGI->new;

# Send a custom status header with HTTP/1.1
print $cgi->header('HTTP/1.1 405 Method Not Allowed', -type => 'text/html', -charset => 'ISO-8859-1');

# Your response body
print "<html><body><h1>405 Method Not Allowed</h1><p>Method Not Allowed</p></body></html>";
