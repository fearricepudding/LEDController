#ifndef UDPSERVER_H_INCLUDED
#define UDPSERVER_H_INCLUDED


#define PORT    8080 
#define MAXLINE 2048 

class UdpServer{
public:
	UdpServer();
	~UdpServer();
	void listen();	
private: 
	void send(char *message);
	void recieve();
	struct sockaddr_in servaddr, cliaddr; 
	int sockfd; 
    char buffer[MAXLINE]; 
    
};

#endif
