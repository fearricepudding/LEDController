
// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <iostream>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "udpServer.h"
#include "LEDController.h"

UdpServer::UdpServer(){}
UdpServer::~UdpServer(){};
  
// Driver code 
void UdpServer::listen(){
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        std::cout << "Socket creation failed" << std::endl;
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) { 
        std::cout << "Error occured binding port" << std::endl;
        exit(EXIT_FAILURE); 
    }  
    
    recieve();
} 

void UdpServer::recieve(){
	 int n; 
  	socklen_t len;
    len = sizeof(cliaddr);  //len is value/resuslt 
    n = recvfrom(sockfd, 
    			(char *)buffer, 
    			MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                &len);
                 
    buffer[n] = '\0'; 
    std::string  msg(buffer);
    
    // Send our message to our controller
    LEDController *ledc = LEDController::getInstance();
    ledc->newMessage(msg);
    
    // Send response
    send((char *)"ok");
    recieve();
}


void UdpServer::send(char  *message){
	int n; 
  	socklen_t len;
	len = sizeof(cliaddr);  //len is value/resuslt 
	sendto(sockfd, (const char *)message, strlen(message),  
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
            len); 
}

