#!/usr/bin/perl

use strict;
use warnings;
use CGI;
use File::Basename;

# Initialize CGI object
# Add dollar sign here for scalar variable
my $cgi = CGI->new;
my $body = "";
my $status_code = 200;  

# logging purpose
my $log = "text.txt";
open my $fh, '>', $log or die "Cannot open file $log: $!";
print $fh "testinf req method: ";
print $fh $cgi->request_method;
print $fh "test param file: ";
my $buffer = $cgi->param('file');
print $fh "$buffer";

# Check if the request method is DELETE
if ($cgi->request_method eq 'DELETE') {

    # Get the 'file' parameter from the request
    my $file = $cgi->param('file');

    # If the 'file' parameter is not provided, return an error
    if (!$file) {
        $status_code = 404;  # Use $ for scalar variable
        $body .= "Error: No file specified.\n";  # Use .= to append to string
        exit(1);
    }

    # Define the directory where files can be deleted (e.g., /path/to/files)
    my $file_dir = 'list';  # Change this to your target directory

    # Sanitize the file name to prevent directory traversal
    my $basename = basename($file);  # This ensures we get the file name only, no directory path
    my $file_path = "$file_dir/$basename";

    print STDERR "check '$file_path' '$file' \n";

    # Check if the file exists
    if (-e $file_path && -f $file_path) {

        # Try to delete the file
        if (!(unlink $file_path)) {
            $status_code = 500;  # Use $ for scalar variable
            $body .= "Error: Unable to delete file '$file'.\n";  # Use .= to append to string
        } else {
            $body .= "File '$file' deleted successfully.\n";  # Use .= to append to string
        }

    } else {
        $status_code = 404;  # Use $ for scalar variable
        $body .= "Error: File '$file' does not exist or is not a valid file.\n";  # Use .= to append to string
    }
} else {
    $status_code = 405;  # Use $ for scalar variable
    $body .= "Error: Invalid HTTP method. Only DELETE requests are allowed.\n";  # Use .= to append to string
}

# Print the appropriate HTTP header based on status code
print 'HTTP/1.1 ';
if ($status_code == 200) {
    print "200 OK\r\n";
    print $cgi->header(-type => 'text/html', -charset => 'ISO-8859-1');
} elsif ($status_code == 404) {
    print "404 Not Found\r\n";
    print $cgi->header(-type => 'text/html', -charset => 'ISO-8859-1');
} elsif ($status_code == 405) {
    print "405 Method Not Allowed\r\n";
    print $cgi->header(-type => 'text/html', -charset => 'ISO-8859-1');
} else {
    print "500 Internal Server Error\r\n";
    print $cgi->header(-status => 'HTTP/1.1 500 Internal Server Error', -type => 'text/html', -charset => 'ISO-8859-1');
}

# Print the body of the response
print "$body";
