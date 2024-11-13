#!/usr/bin/perl

use strict;
use warnings;
use CGI;
use File::Path 'remove_tree';
use File::Basename;

# Initialize CGI object
my $cgi = CGI->new;

# Output the content-type header
print $cgi->header('text/plain');

# Check if the request method is DELETE
if ($cgi->request_method eq 'DELETE') {

    # Get the 'file' parameter from the request
    my $file = $cgi->param('file');

    # If the 'file' parameter is not provided, return an error
    if (!$file) {
        print "Error: No file specified.\n";
        exit(1);
    }

    # Define the directory where files can be deleted (e.g., /path/to/files)
    my $file_dir = 'list';  # Change this to your target directory

    # Sanitize the file name to prevent directory traversal
    my $basename = basename($file);  # This ensures we get the file name only, no directory path
    my $file_path = "$file_dir/$basename";

    # Check if the file exists
    if (-e $file_path && -f $file_path) {

        # Try to delete the file
        if (unlink $file_path) {
            print "File '$file' deleted successfully.\n";
        } else {
            print "Error: Unable to delete file '$file'.\n";
        }

    } else {
        print "Error: File '$file' does not exist or is not a valid file.\n";
    }
} else {
    print "Error: Invalid HTTP method. Only DELETE requests are allowed.\n";
}
