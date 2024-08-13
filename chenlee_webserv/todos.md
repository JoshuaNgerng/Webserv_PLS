- [ ] Set up a basic TCP/IP socket connection to listen for incoming requests.
- [ ] Parse incoming HTTP requests to extract the requested resource and any query parameters.
- [ ] serving static files (HTML, CSS, JS, images, etc.).
- [ ] dynamic content generation using a templating engine or CGI scripts.
- [ ] handling HTTP methods (GET, POST, PUT, DELETE, etc.).
- [ ] handling HTTP headers (Content-Type, Content-Length, etc.).
- [ ] handling server-side logging.
- [ ] handling HTTP redirects.
- [ ] handling server-side events (SSE).
- [ ] handling server-side caching.
- [ ] handling server-side rate limiting.
- [ ] handling server-side load balancing.
- [ ] handling server-side error reporting.
- [ ] handling server-side monitoring and metrics.
- [ ] Write unit tests to ensure the server is functioning correctly.

- [ ] handling HTTP cookies.
- [ ] handling HTTP authentication (Basic, Digest, etc.).
- [ ] handling server-side compression (gzip, deflate, etc.).

### server-router
- [ ] parse config create ServerBlocks with Router
- [ ] Connection read data into Request
- [ ] Connection send data from Response

### Reminder!
- take configuration file, like nginx
- cant `execv` another web server
- the server must never block, use select(poll equivalent)
- client can be bounced properly if necessary
- multiplexing

TODOs:
- [ ] make chunked response