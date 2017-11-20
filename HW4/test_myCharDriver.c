/**
 * @file 
 *		test_myCharDriver.c
 * @brief 
 *		This file implements the testing of char Driver for LED
 * @author 
 *		Saritha Senguttuvan
 * @date 
 *		November 19,2017
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
 
#define BUFFER_LENGTH 256           
#define PORT 8000           /* Port number for the server on BBG */

static char receive[BUFFER_LENGTH]; /* buffer to receive the message */

int main()
{
	int ret, fd;
	int server_fd;
	int new_socket;
	int opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);	
    char buffer[1024] = {0};
    int valread;
    int query_read;
	
	
    int LED_period ;
    int LED_dutyCycle;
    int LED_Toggle;
    char stringToSend[BUFFER_LENGTH] = "Saritha";

    printf("Starting my test code\n");

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)     /* Create a socket */
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) /* Make the socket reusable */
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    /* Initialize the address for the socket */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) /* Bind the socket to the address */
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)       /* Listen to the socket */
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) /* Accept the connection request */
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("IP address is: %s\n", inet_ntoa(address.sin_addr));
	printf("port is: %d\n", (int) ntohs(address.sin_port));
	
    valread = read(new_socket , buffer, 1024);          /* Read from the socket */
    if(valread == -1)
    	printf("Error no: %d in reading the socket", errno);
    printf("Buffer is %s\n",buffer );
       
    
    printf("ENTER TO CONTINUE\n");
    getchar();

	sscanf(buffer, "%d %d %d %d", &LED_period, &LED_dutyCycle, &LED_Toggle, &query_read);  /* Parse the received message */

    fd = open("/dev/LED_CharDriver", O_RDWR);             // Open the device with read/write access
    if (fd < 0)
    {
      printf("unable to open the device. Error no: %d\n", errno);
      
      return errno;
    }

	sprintf(stringToSend, "%d %d %d %d", LED_period, LED_dutyCycle, LED_Toggle, query_read); 
	ret = write(fd, stringToSend, strlen(stringToSend)); 		// Write the string to the kernel module
	if (ret < 0)
	{
		perror("Unable to write to the device.\n");
		return errno;
	}

	printf("Press ENTER to read back from the device...\n");
	getchar();

    ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
	if (ret < 0)
	{
		perror("Unable to read from the device.\n");
		return errno;
	}

    send(new_socket , receive , strlen(receive) , 0 );  /* Send the message received via kernel to the laptop using socket */
    
    
    printf("The received message is %s\n", receive);
    return 0;
} 
