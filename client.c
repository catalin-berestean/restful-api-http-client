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
#include "parson.h"

int main(int argc, char *argv[]){
    char *message, *response;
    char *cookie = calloc(100,sizeof(char));
    int i = 0;
    int sockfd = socket(AF_INET,SOCK_STREAM, 0);
    //----TASK 1 - GET initial ----//

    sockfd = open_connection("185.118.200.35",8081, AF_INET,SOCK_STREAM,0);
    message = compute_get_request("185.118.200.35","/task1/start",NULL,0,0,0);
    send_to_server(sockfd,message);
   
    printf("\nTrimit requestul:\n");
    printf("%s\n",message);
    response = receive_from_server(sockfd);
    printf("%s\n",response);    //printez raspunsul de la server
    close_connection(sockfd);   
   	
   	//----TASK 2 - AUTENTIFICARE----//

    char *set_Cookie(int nr, char *data) {      //functie pentru parsarea cookie
        printf("\nTASK %d\n", nr);
        char *str = strtok(data, "\" \n");

        char *cookie2 = calloc(100, sizeof(char)); // 2nd cookie
        int len = strlen(cookie) - 1;
        while (str != NULL) {

            if(strcmp(str, "Set-Cookie:") == 0) {
                str = strtok(NULL, "\" \n");
                strcat(cookie, str);
                cookie[len] = '\0';
                sprintf(cookie, "%s ", cookie);
                strcat(cookie2, cookie);
            }
            str = strtok(NULL, "\" \n");
        }
        return cookie;
    }
    
    char *parse(char *str) {        //functie pentru parsarea raspunsului
        int i = 0;
        while (str[i] != '{'){
            i++;
        }
        return &str[i];
    }

    char *response1 = calloc(100,sizeof(char));
    char *data = calloc(100,sizeof(char));
	
	response1 = parse(response);

    JSON_Value * value = json_parse_string(response1);    //folosite pt extragerea si inlocuirea informatiilor in json
   	JSON_Object * object = json_value_get_object(value);

    char *method = json_object_get_string(object, "method");
    char *type = json_object_get_string(object, "type");
    char *username = json_object_dotget_string(object, "data.username");
    char *password= json_object_dotget_string(object, "data.password");

    cookie = set_Cookie(2,response);        //setez cookie

    sprintf(data, "username=%s&password=%s",username,password); //inlocuiesc username si parola

    sockfd =  open_connection("185.118.200.35",8081, AF_INET,SOCK_STREAM,0);
    //printf("\nTrimit requestul:\n");
    message = compute_post_request("185.118.200.35","/task2/login",data,method,type,cookie,NULL);
    
    printf("\nTrimit requestul:\n");
    printf("%s\n",message);
    send_to_server(sockfd,message);
    printf("%s\n",response);
    response = receive_from_server(sockfd);

    printf("%s\n",response);    //printez raspunsul de la server
    close_connection(sockfd);
    
    //----TASK 3 - TOKEN JWT ----//

    char *token, *parametersID;
    char *url, * parameters = calloc(100,sizeof(char));

    response1 = parse(response);
    value = json_parse_string(response1);
    object = json_value_get_object(value);
    
    sprintf(cookie,"%s","");                //eliberarea cookie

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);

    token = json_object_dotget_string(object, "data.token");
    parametersID = json_object_dotget_string(object, "data.queryParams.id");
    url = json_object_get_string(object,"url");
    method = json_object_get_string(object, "method");    
    

    strcat(parameters,"raspuns1=omul&raspuns2=numele&id=");
    strcat(parameters,parametersID); //completez raspunsurile

    cookie = set_Cookie(3,response);        //setez cookie

    message = compute_get_request("185.118.200.35", url, parameters, method, cookie, token);    

    printf("\nTrimit requestul:\n");
    printf("%s\n",message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);   //printez raspunsul de la server
    
    close_connection(sockfd);



    //----TASK 4 - OBTINEREA CHEII ----//

    response1 = parse(response);
    
    printf("\nLooking for the key\n...");       

    value = json_parse_string(response1);
    object = json_value_get_object(value);

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);

    url = json_object_get_string(object,"url");
    method = json_object_get_string(object, "method");
    type = json_object_get_string(object, "type");

    sprintf(cookie,"%s","");                //eliberez cookie
    cookie = set_Cookie(4,response);

    message = compute_get_request("185.118.200.35", url, parameters, method, cookie, token); 

    printf("\nTrimit requestul:\n");
    printf("%s\n",message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);           //printez raspunsul de la server
    
    close_connection(sockfd);

    return 0;
}