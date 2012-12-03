/*
 * File: service.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "service.h"
#include "util.h"

void handle_client(int socket)
{
    int reqsize =0;
    int bufsize=100;
    char buf[bufsize];
    typedef struct reqStruct
    {
        http_method method;
        int headerSize;
        int contentLength;
        char* host;
        char* pragma;
        char* acceptedEncoding;
        char* body;

    } reqStruct;
    /* TODO Loop receiving requests and sending appropriate responses,
     *      until one of the conditions to close the connection is
     *      met.
     overall framework:
        while (connection is open)

        receive request:
        while (request header is not complete)
    receive more data from socket into temp buffer
    if buffer is not big enough for old data + new data, realloc it to fit more data
    copy temp buffer to the end of buffer
    if request method is not GET
    read content length
    let read = received data minus size of the header
    if buffer is not big enough for header size plus content length, realloc it to fit more data
    while read < content length
        receive more data into temp buffer
        copy temp buffer to end of buffer

        parse command
        build response
        send response
     */

    while(connection is open)
    {
        reqStruct* r = (reqStruct*) malloc(sizeof (reqStruct));
        r->body=NULL;
        char tempbuf[100];
        //receive request
        //initial recv for first data, keep track of size of request
        reqsize=recv(socket, buf, 99, 0);
        //make sure receive whole request
        while(http_header_complete(buf,bufsize)==-1)
        {
            int nextChunk=recv(socket, tempbuf, 99, 0);
            if (bufsize<reqsize+nextChunk)
            {
                buf=realloc(buf, bufsize*2);
                bufsize*=2;
            }
            for (int i =0; i<nextChunk; i++)
            {
                buf[reqsize+i] = tempbuf[i];
            }
            reqsize+=nextChunk;
        }
        r->headerSize=http_header_complete(buf,reqsize);
        r->method=http_parse_method(buf);

        //make sure to get the body if not a GET request
        //if it isn't need to make sure body is fully received
        int read =0;
        if (method!=METHOD_GET)
        {
            r->contentLength=atoi(http_parse_header_field(buf,bufsize,"Content-Length"));
            bufsize++;
            read = reqsize-r->headerSize;
            //loop until receive all of the contents of body
            while(read < r->contentLength)
            {
                int nextChunk= recv(socket, tempbuf, 99, 0);
                (if (bufsize<reqsize+nextChunk))
                {
                    buf=realloc(buf, bufsize*2);
                    bufsize*=2;
                }
                for(int i =0; i<nextChunk; i++)
                {
                    buf[reqsize+i]=tempbuf[i];
                }
                reqsize+=nextChunk;
            }
            r->body=http_parse_body(buf,reqsize);
        }

        //parse
        r->host=http_parse_header_field(buf,reqsize,"Host");
        r->pragma=http_parse_header_field(buf,reqsize,"Pragma");
        r->acceptedEncoding=http_parse_header_field(buf,reqsize,"Accepted-Encoding");

        //build repsonse
        //send r epsonse
    }

    return;
}
