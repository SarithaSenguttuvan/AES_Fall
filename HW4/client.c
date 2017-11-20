/**
 * @file 
 *		client.c
 * @brief 
 *		This file from my PC is used to communicate to the BBG application using socket programming
 * @author 
 *		Saritha Senguttuvan
 * @date 
 *		November 19,2017
 *
 */
 
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h> 
#include <stdbool.h>
#include <errno.h>

#define PORT 8000
#define BUFFER_LENGTH 256   
  
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, ret;
    struct sockaddr_in serv_addr;
    
    char buffer[1024] = {0};
    int LED_period = 5000;
    int LED_dutyCycle = 2000;
    bool LED_Toggle = false;
    int read_query = 2;
    char stringFromClient[BUFFER_LENGTH];
    
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)       /* Create a socket on the client side */
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    /* Initialize the server address */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "192.168.7.2", &serv_addr.sin_addr)<=0) /* convert to network byte order */
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) /* Connect to the server socket */
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    sprintf(stringFromClient, "%d %d %d %d", LED_period, LED_dutyCycle, LED_Toggle, read_query);
    send(sock , stringFromClient , strlen(stringFromClient) , 0 ); /* Send to the BBG user application using sockets */
    ret = read( sock , buffer, 1024);
    if(ret == -1)
        printf("ERROR: %d in reading from client\n", errno); 

    printf("%s\n",buffer );
    return 0;
}
