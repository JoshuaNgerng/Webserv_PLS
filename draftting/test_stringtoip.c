#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdbool.h>

// Function to check if a string is a valid IPv4 or IPv6 address
bool is_valid_ip(const char *addr) {
    struct sockaddr_in sa;   // for IPv4
    struct sockaddr_in6 sa6; // for IPv6
    // Try to convert to IPv4
    if (inet_pton(AF_INET, addr, &(sa.sin_addr)) == 1) {
        return true;
    }
    // Try to convert to IPv6
    if (inet_pton(AF_INET6, addr, &(sa6.sin6_addr)) == 1) {
        return true;
    }
    return false;
}

// Function to parse the string into address and port
void parse_address(const char *input) {
    char *colon_pos = strchr(input, ':');  // Find the colon separating address and port
    char addr[INET6_ADDRSTRLEN] = {0};     // To store the IP address part
    char *port = NULL;

    if (colon_pos) {
        size_t addr_len = colon_pos - input;
        strncpy(addr, input, addr_len);  // Copy the address part
        port = colon_pos + 1;            // Port is the part after the colon
    } else {
        // No colon found, treat the input as a port
        port = (char *)input;
    }

    // Check if the address part is valid
    if (is_valid_ip(addr) || strcmp(addr, "*") == 0) {
        printf("IP Address: %s\n", addr);
    } else {
        printf("No valid IP address found, default to any IP.\n");
    }

    if (port) {
        printf("Port: %s\n", port);
    }
}

int main() {
    char input1[] = "127.0.0.1:8080";
    char input2[] = "[::1]:443";
    char input3[] = "80";
    char input4[] = "*:8080";

    printf("Input 1: %s\n", input1);
    parse_address(input1);

    printf("\nInput 2: %s\n", input2);
    parse_address(input2);

    printf("\nInput 3: %s\n", input3);
    parse_address(input3);

    printf("\nInput 4: %s\n", input4);
    parse_address(input4);

    return 0;
}
