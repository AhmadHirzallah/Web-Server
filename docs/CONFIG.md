# Config Parsing
The config parser translates text rules into structured data that controls how every HTTP request is handled.
## File Hierarchy 
```
GLOBAL
 └── server
		├── listen
		├── error_page
		├── client_max_body_size
		└── location
			├── methods
			├── root
			├── index
			├── autoindex
			├── upload_path
			├── return
			└── cgi
```
## 1. `server` Block
```nginx
server {
	...
}
```
### Purpose

 - Define a virtual server, multiple servers are accepted.
## 1.1 `listen` Directive
```nginx
listen 8080;
listen 127.0.0.1:8081;
```
### Purpose

 - Tells the server which sockets to bind
 - Each `listen` creates a listening socket.
## 1.2 `client_max_body_size` Directive
```nginx
client_max_body_size 10M;
client_max_body_size 500k;
client_max_body_size 1048576;
```
### Purpose
 - Maximum allowed request body size
 - Protects the server from large uploads
## 1.3 `error_page` Directive
```nginx
error_page 404 /errors/404.html;
error_page 500 /errors/500.html;
```
### Purpose
 - Custom error responses
## 2. `location` Block
```nginx
location /upload {
	...
}
```
### Purpose
 - Defines rules for a URL subtree
## 2.1 `methods` Directive
```nginx
methods GET POST DELETE;
```
### Purpose
 - Restricts allowed HTTP methods for this route
## 2.2 `root` Directive
```nginx
root ./www;
```
### Purpose
 - Maps URL to filesystem path
## 2.3 `index` Directive
```nginx
index index.html
```
### Purpose
 - Default file when URL maps to a directory
## 2.4 `autoindex` Directive
```nginx
autoindex on;
autoindex off;
```
### Purpose
 - Enables directory listing if no index file
## 2.5 `upload_path` Directive
```nginx
upload_path ./uploads;
```
### Purpose
 - Allows file uploads (POST)
 - Where uploaded files are saved
## 2.6 `return` Directive
```nginx
return 301 /new-path;
```
### Purpose
 - Redirects client
## 2.7 `cgi` Directive
```nginx
cgi .php /usr/bin/php-cgi;
```
### Purpose
Enables CGI execution for matching files
