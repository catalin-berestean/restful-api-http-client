#include <stdio.h>
#include <stdlib.h>     
#include <unistd.h>     
#include <string.h>     
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>      
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"


char *compute_get_request(char *host, char *url, char *url_params,char *method,char *cookie,char *token) {
    
    char *message;
    char *line; 
    message = calloc(BUFLEN, sizeof(char));
    line = calloc(LINELEN, sizeof(char));

    //NUMELE metodei
    if (url_params != NULL){
        sprintf(line, "GET %s?%s HTTP/1.1", url, url_params);
    }
    else{
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    //adaugam la mesaj
    compute_message(message, line);
    
    //HOST
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Cookie: %s", cookie);
    compute_message(message,line);

    sprintf(line, "Authorization: Bearer %s", token);
    compute_message(message, line);

    //se adauga linia de final
    strcat(message, "\r\n");

    compute_message(message, line);

    return message;
}

char *compute_post_request(char *host,char *url, char *form_data, char *command, char *type, char *cookie, char *token){

    char *message;
    char *line; 
    message = calloc(BUFLEN, sizeof(char));
    line = calloc(LINELEN, sizeof(char));

    if(command != NULL && url != NULL){
        sprintf(line, "%s %s HTTP/1.1", command, url);
    }
    compute_message(message, line);

    
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Cookie: %s", cookie);
    compute_message(message, line);

    sprintf(line,"Content-Type:%s",type);
    compute_message(message, line);

    if(form_data != NULL){
        sprintf(line,"Content-Length: %ld", strlen(form_data));
    }
    compute_message(message, line);

    sprintf(line, "Authorization: Bearer %s", token);
    compute_message(message, line);

    strcat(message, "\r\n");
    sprintf(line, "%s", form_data);
    compute_message(message, line);


    return message;
}

