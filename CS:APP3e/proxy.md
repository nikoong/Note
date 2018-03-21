#CSAPP3e proxy实验（part1）

###前沿

　　我们要实现一个网络代理。网络代理其实就是从客户端到服务器路途中间的一个中间站。网络代理既要像服务器一样接收客户端发送来的数据，也要像客户端一样向服务器发送消息。这么看来，我们弄懂了网络代理，就能同时搞懂客户端和服务器。我估计这就是为什么网络代理会成为实验的内容了。我们的实验目标分三步走：一、实现一个单线程的网络代理；二、多线程；三、代理加入缓存功能。
　　我在做这个实验的过程中遇到了不少困难。首先因为自己对书本上的知识没有完全吃透，其次就是这次的实验代码里没有给我们框架，要我们从头到尾自己实现。我自己为了做这个实验的第一步，我就整整花了一天时间。写代码的时间只占一点，大部分时间是来回翻书，看writeup实验手册。为了帮助各位同学，顺利完成本次实验，少走点弯路。我：
  
  1. 在**知识准备**部分，罗列出实验part1所需要全部知识，方便大家查缺补漏。

  2. 在**简单框架**部分，给出一个网络代理的简单框架，方便大家迈出第一步。


###知识准备

* 网络代理：

* 套接字函数：

* HTTP请求：请求行 请求报头 空行

* cstring头文件：包含在csapp.h中

###简单框架

- 从头开始很困难，我们先看《深入理解计算机系统第三版》11.6节的 TINY Web服务器代码。
	- main函数
	- clienterror函数
	- doit函数
	- parse_uri函数

- 我们自己需要写的函数，生成请求报头的函数。

```cpp

int main(int argc,char **argv)
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE,
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
    doit(connfd);
    Close(connfd);
    }
}


void doit(int fd) 
{
    int serverfd;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char hostname[MAXLINE], path[MAXLINE], port[MAXLINE], 
         request_header[MAXLINE];
    rio_t rio, server_rio;

    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))
        return;
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);
    if (strcasecmp(method, "GET")) {
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }

    /* Parse URI from GET request */
    parse_uri(uri, hostname, path, port);

    /* Build the request header*/
    build_request_header(request_header, hostname, path, port, &rio);

    /*Connect to server*/
    serverfd = Open_clientfd(hostname, port);
    if(serverfd<0){
        printf("connection failed\n");
        return;
    }
    /*Write the http header to server*/
    Rio_readinitb(&server_rio, serverfd);
    Rio_writen(serverfd, request_header, strlen(request_header));

    /*receive message from end server and send to the client*/
    size_t n;
    while( (n=Rio_readlineb(&server_rio,buf,MAXLINE))!=0 )
    {
        printf("proxy received %zu bytes,then send\n",n);
        Rio_writen(fd, buf, n);
    }
    Close(serverfd);
}

```
