#CSAPP3e proxy实验（part1）

###前言

　　我们要实现一个简单的网络代理，只需要转发HTTP请求事件。网络代理其实就是从客户端到服务器路途中间的一个中间站。网络代理既要像服务器一样接收客户端发送来的数据，也要像客户端一样向服务器发送消息。这么看来，我们弄懂了网络代理，就能同时搞懂客户端和服务器。我估计，这就是为什么网络编程这一章实验的内容，会选为实现网络代理了。我们的实验目标分三步走：一、实现一个单线程的网络代理；二、多线程；三、代理加入缓存功能。
　　我在做这个实验的过程中遇到了不少困难。首先因为自己对书本上的知识没有完全吃透，其次就是这次的实验代码里没有给我们框架（其实给了，见后文），要我们从头到尾自己实现。我自己为了做这个实验的第一步，我就整整花了一天时间。写代码的时间只占一点，大部分时间是来回翻书，看writeup实验手册。为了帮助各位同学，顺利完成本次实验，少走点弯路。我：
  
  1. 在**知识准备**部分，罗列出实验part1所需要全部知识，方便大家查缺补漏。

  2. 在**简单框架**部分，给出一个网络代理的简单框架，方便大家迈出第一步。


###知识准备

* 套接字函数：CSAPP书中为了我们的方便，基于套接字函数实现了两个辅助函数。open_clientfd，输入服务器的ip和端口，返回套接字描述符。open_listenfd输入端口，返回监听描述符

* HTTP请求：一个HTTP请求由请求行、请求报头、空行组成。我们的代理要发送给服务器的请求，必须包含这些请求报头。同时如果客户端还发过来了其他请求报头，也要加在下文里面，一起发给客户端。

        GET /hub/index.html HTTP/1.0
        Host: www.cmu.edu：80
        User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3
        Connection: close
        Proxy-Connection: close
        空行

* cstring头文件：包含在csapp.h中。这里主要集中了C语言处理字符串的函数,比如:strcpy，strchr，strcmp，strcat，strncasecmp。外加一个cstdio头文件里的sprintf函数。

###搭建简单框架

- 从零开始搭建框架很困难，尤其是刚刚接触网络编程的时候。实验手册建议我们先看书上11.6节的TINY Web服务器代码，代码分为以下几个部分：

	- main函数：while循环中得到套接字描述符，然后调用doit函数。

	- doit函数：读入请求行，调用parse_uri解析请求行，然后返回静态内容或者动态内容。

	- parse_uri函数：解析请求行的具体函数。

	- clienterror函数：用来报告错误，我们可以先直接使用。

- 思考一下，我们要实现的代理和web服务器的异同是什么：

	1. 都需要接收客户端发送来的HTTP请求；都需要解析HTTP请求行。代理解析请求行时，需要得到服务器的ip、端口和资源路径（为了连接服务器和构造请求报头），而不需要关注是动态和静态内容。

	2. 我们还需要构造一些固定的请求报头，组成完整的HTTP请求发给服务器。

	3. 要接收服务器发回的信息，然后发给客户端。


- 根据以上的分析。我们的大体框架思路就出来了：
	
	- main函数：同TINY Web
	
	- doit:读入请求行（同TINY Web），调用解析请求行函数parse_uri，调用请求报头构造函数build_request_header，再处理和服务器相关事宜。

	- parse_uri：解析uri，和TINY Web有差异。

	- build_request_header：建立请求报头的函数，需要自己写。

- 最后得到的框架如下，大部分都和TINY Web里的代码一样。我们只要完成parse_uri函数和build_request_header函数就可以了。怎么样，简单吗？

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
　　强烈建议大家看完框架先自己写。我自己的代码提交在[github](https://github.com/nikoong/CSAPP/blob/master/LAB/proxy/proxylab-handout/proxy_part1.c)上，也供大家参考。

####参考
- [CSAPP: How to Write a Web Proxy](http://lifeofzjs.com/blog/2014/02/21/csapp-proxy-lab/)
- [【CSAPP】proxy Lab代理实验](https://blog.csdn.net/a2888409/article/details/47186725)
- [CSAPP: Proxy lab](https://blog.csdn.net/u012336567/article/details/52056089)
- [Socket描述符](http://www.cnblogs.com/davidzhou11225/archive/2012/05/03/2480347.html)
- [理解HTTP消息头【很完整，例子也很丰富】](http://www.cnblogs.com/jacktu/archive/2008/01/16/1041710.html)
