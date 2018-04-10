
/*
Copyright (c) 2018 Natesh M Bhat

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/




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
    struct timeval timeout; 
        
    char buffer[1025];  //data buffer of 1K 
        
    //set of socket descriptors 
    fd_set readfds; 

    void initServerSocket() ; 
       

    public : 

    socketHandler(void) ; 
    void bindServer() ; 
    // void handleActivity() ; 
    std::vector<int> handleActivity() ; 
    void setupClientDescriptors() ; 
    void startServer() ; 
    void stopServer() ; 
    void sendData(int , std::string) ; 
    int checkClientActiviy() ; 
    void closeSocket(int ) ; 
    int handleNewConnection() ; 
    std::string handleIOActivity(int) ; 

    
};

#endif // !1SE SERVER_H

