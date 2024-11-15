#!/usr/bin/perl

use strict;
use warnings;
use CGI;
use File::Basename;

my $cgi = CGI->new;
my $status_code = 200;
my $body = "";

sub print_response {
    my ($status_code, $body) = @_;
    my %status = (
        200 => 'OK',
        400 => 'Bad Request',
        404 => 'Not Found',
        405 => 'Method Not Allowed',
        500 => 'Internal Server Error'
    );
    
    print "HTTP/1.1 $status_code $status{$status_code}\r\n";
    print $cgi->header(-type => 'text/html', -charset => 'ISO-8859-1');
    print $body;
}

if ($cgi->request_method eq 'DELETE') {
    my $file = $cgi->param('file');
    if (!$file) {
        $status_code = 400;  # Bad Request
        $body = "Error: No file specified.\n";
        print_response($status_code, $body);
        exit(0);
    }

    # Sanitize the file parameter
    $file =~ s/\.\.//g;
    $file =~ s/\/+/\//g;

    my $file_dir = 'list';
    my $basename = basename($file);
    my $file_path = "$file_dir/$basename";

    if (-e $file_path && -f $file_path) {
        if (!( -r $file_path && -w $file_path )) {
            $status_code = 403;
            $body .= "Error: No Permission to delete '$file'.\n";
        } elsif (!(unlink $file_path)) {
            $status_code = 500;
            $body .= "Error: Unable to delete file '$file'.\n";
        } else {
            $body .= "File '$file' deleted successfully.\n";
        }
    } else {
        $status_code = 404;
        $body .= "Error: File '$file' does not exist or is not a valid file.\n";
    }

} else {
    $status_code = 405;
    $body .= "Error: Invalid HTTP method. Only DELETE requests are allowed.\n";
}

print_response($status_code, $body);

