#ifndef SERVER_H
#define SERVER_H

#include <stdio.h> 
#include <string.h>   //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include<iostream>
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include<bits/stdc++.h>
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 


class socketHandler{

    int opt ;  
    int master_socket , PORT , addrlen , new_socket , client_socket[30] , 
          max_clients , activity, i , valread , sd;  
    int max_sd;  
    struct sockaddr_in address;  
    char message[50] ; 
        
    char buffer[1025];  //data buffer of 1K 
        
    //set of socket descriptors 
    fd_set readfds; 

    void initServerSocket() ; 
       

    public : 

    socketHandler(void) ; 
    void bindServer() ; 
    void handleActivity() ; 
    std::vector<int> handleActivity(std::string) ; 
    void setupClientDescriptors() ; 
    void startServer() ; 

    int checkClientActiviy() ; 
    void closeSocket() ; 
    int handleNewConnection() ; 
    void handleIOActivity(int) ; 

    
};

#endif // !1SE SERVER_H

