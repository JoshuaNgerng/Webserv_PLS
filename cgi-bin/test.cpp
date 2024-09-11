#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>
#include <cerrno>

// Function to execute the CGI script
void execute_cgi(const std::string &script_path, const std::string &path_info) {
    // Check if the CGI script exists and is executable
    if (access(script_path.c_str(), X_OK) == -1) {
        std::cerr << "Error: Cannot access script '" << script_path << "' - " << strerror(errno) << std::endl;
        return;
    }

    // Set up environment variables for CGI
    setenv("PATH_INFO", path_info.c_str(), 1);

    // Fork a process to run the CGI script
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed: " << strerror(errno) << std::endl;
        return;
    }

    if (pid == 0) {
        // Child process: Execute the CGI script
        char *args[] = {const_cast<char *>(script_path.c_str()), NULL};
        extern char **environ;  // Use extern to reference environment

        // Execute the CGI script and check for errors
        execve(script_path.c_str(), args, environ);

        // If execve fails, report the error and exit
        std::cerr << "Error: execve() failed - " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    } else {
        // Parent process: Wait for CGI script to finish
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            std::cout << "CGI script exited with status: " << WEXITSTATUS(status) << std::endl;
        } else if (WIFSIGNALED(status)) {
            std::cerr << "CGI script was terminated by signal: " << WTERMSIG(status) << std::endl;
        } else {
            std::cerr << "CGI script ended unexpectedly." << std::endl;
        }
    }
}

int main() {
    // Path to the CGI script
    std::string script_path = "./hello.py";  // Use absolute path if needed
    std::string path_info = "/example";      // Simulate PATH_INFO

    // Output message
    std::cout << "Running CGI script: " << script_path << std::endl;

    // Execute the CGI script
    execute_cgi(script_path, path_info);

    return 0;
}
