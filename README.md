# C++98 HTTP 1.1 webserver

## Workflow
![workflow diagram](https://github.com/nsr888/webserver/blob/main/workflow_diagram.jpg?raw=true)
## Objects relationship diagram
![class diagram](https://github.com/nsr888/webserver/blob/main/objects_diagram_.jpg?raw=true)
## CGI diagram
![cgi diagram](https://github.com/nsr888/webserver/blob/main/cgi_diagram.svg?raw=true)
https://lectureswww.readthedocs.io/5.web.server/cgi.html

## Subject
* You must write a HTTP server in C++ 98
* The C++ standard must be C++ 98. Your project must compile with it.
* It must be conditionnal compliant with the rfc 7230 to 7235 (http 1.1) but you need to implement only the following headers
    * Accept-Charsets
    * Accept-Language
    * Allow
    * Authorization
    * Content-Language
    * Content-Length
    * Content-Location
    * Content-Type
    * Date
    * Host
    * Last-Modified
    * Location
    * Referer
    * Retry-After
    * Server
    * Transfer-Encoding
    * User-Agent
    * WWW-Authenticate
* You can implement all the headers if you want to
* We will consider that nginx is HTTP 1.1 compliant and may be use to compare
headers and answer behaviors
* It must be non blocking and use only 1 select for all the IO between the client and the server (listens includes).
* Select should check read and write at the same time.
* Your server should never block and client should be bounce properly if necessary
* You should never do a read operation or a write operation without going through select
* Checking the value of errno is strictly forbidden after a read or a write operation
* A request to your server should never hang forever
* You server should have default error pages if none are provided
* Your program should not leak and should never crash, (even when out of memory if all the initialisation is done)
* You can’t use fork for something else than CGI (like php or perl or ruby etc...)
* You can include and use everything in "iostream" "string" "vector" "list" "queue"
"stack" "map" "algorithm"
* Your program should have a config file in argument or use a default path

## Resources
### Core
* https://luminousmen.com/post/asynchronous-programming-blocking-and-non-blocking
* http://www.stolyarov.info/books/programming_intro/vol3 (page 227)
### HTTP Protocol
* https://ru.wikipedia.org/wiki/HTTP
* https://efim360.ru/rfc-7230-protokol-peredachi-giperteksta-http-1-1-sintaksis-soobshheniya-i-marshrutizacziya/#3-Message-Format
* https://efim360.ru/rfc-7231-protokol-peredachi-giperteksta-http-1-1-semantika-i-kontent/
### CGI
* https://habr.com/ru/post/121885/
