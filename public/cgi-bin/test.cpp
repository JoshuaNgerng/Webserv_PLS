/*
Validate Header: true
Header
POST /cgi-bin/upload.py HTTP/1.1
Host: localhost:8000
Connection: keep-alive
Content-Length: 195
sec-ch-ua-platform: "Windows"
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36
sec-ch-ua: "Chromium";v="130", "Google Chrome";v="130", "Not?A_Brand";v="99"
Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryELNVjtAiTi5xYg8p
sec-ch-ua-mobile: ?0
Accept: 
Origin: http://localhost:8000
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: cors
Sec-Fetch-Dest: empty
Referer: http://localhost:8000/upload.html
Accept-Encoding: gzip, deflate, br, zstd
Accept-Language: en-GB,en-US;q=0.9,en;q=0.8

Body
------WebKitFormBoundaryELNVjtAiTi5xYg8p
Content-Disposition: form-data; name="file"; filename="test.txt"
Content-Type: text/plain

testing test

------WebKitFormBoundaryELNVjtAiTi5xYg8p--
*/

# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <sys/socketvar.h>
# include <sys/select.h>
# include <poll.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <netdb.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <string>
# include <iostream>
# include <vector>

static void setup_env(std::vector<const char *>&buffer, char **env) {
    for (int i = 0; env[i]; i ++) {
        buffer.push_back(env[i]);
    }
    buffer.push_back("REQUEST_METHOD=POST");
    buffer.push_back("CONTENT_TYPE=multipart/form-data; boundary=----WebKitFormBoundaryELNVjtAiTi5xYg8p");
    buffer.push_back("CONTENT_LENGTH=195");
    buffer.push_back("REQUEST_URI=/cgi-bin/upload.py");
/*
POST /cgi-bin/upload.py HTTP/1.1
Host: localhost:8000
Connection: keep-alive
Content-Length: 195
sec-ch-ua-platform: "Windows"
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36
sec-ch-ua: "Chromium";v="130", "Google Chrome";v="130", "Not?A_Brand";v="99"
Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryELNVjtAiTi5xYg8p
*/

    buffer.push_back(NULL);
}

int main(int ac, char **av, char **env) {
    int pipfd[4];
    if (pipe(pipfd) < 0 || pipe(pipfd + 2) < 0) {
        return (1);
    }
    {
        std::string body(
            "------WebKitFormBoundaryELNVjtAiTi5xYg8p\r\n"
            "Content-Disposition: form-data; name=\"file\"; filename=\"test.txt\"\r\n"
            "Content-Type: text/plain\r\n\r\n""testing test\n"
            "------WebKitFormBoundaryELNVjtAiTi5xYg8p--\r\n\r\n"
        );
        std::cout << "testing len " << body.length() << '\n';
        ssize_t r = write(pipfd[3], body.c_str(), body.length());
        if (r < 0) {
            return (1);
        }
    }
    std::vector<const char*>env_buffer;
    setup_env(env_buffer, env);
    pid_t   id = fork();
    if (!id) {
        if (dup2(pipfd[2], STDIN_FILENO) < 0 || dup2(pipfd[1], STDOUT_FILENO) < 0 ) {
            return (1);
        }
        for (int i = 0; i < 4; i ++) {
            close(pipfd[i]);
        }
        const char *bin = "/usr/bin/echo";//"/usr/bin/python3";
        // const char *args[] = {"python3", "upload.py", NULL};
        const char *args[] = {"echo", "huh", "why", NULL};
        int status = execve(bin, const_cast<char **>(args), const_cast<char **>(&(env_buffer[0])));
        return (status);
    }
    close(pipfd[1]);
    close(pipfd[2]);
    waitpid(id, NULL, 0);
    char buffer[10001];
    ssize_t r = read(pipfd[0], buffer, 10000);
    if (r < 0) {
        return (1);
    }
    buffer[r] = '\0';
    std::string res(buffer);
    std::cout << res << '\n';
    return (0);
}

// int main(int ac, char **av, char **env) {
//     const char *args[] = {"echo", "testing", "123", NULL};
//     execve("/usr/bin/echo", const_cast<char **>(args), env);
//     return (0);
// }