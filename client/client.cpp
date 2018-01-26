//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux 
#include <stdio.h> 
#include <string.h>   //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include<iostream>
#include<ncurses.h>
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
using namespace std ; 
#define TRUE   1 
#define FALSE  0 


class socketHandler{

    struct sockaddr_in address;
    struct sockaddr_in serv_addr;
    int sock , valread;
    char buffer[1024]  ; 

    public : 

    //initialise all client_socket[] to 0 so not checked 
    socketHandler(void)
    {
        sock = 0  , valread = 0 ; 
        memset(&serv_addr, '0', sizeof(serv_addr));
        memset(buffer , 0 ,sizeof(buffer)) ; 
    }

    void sendData(string message)
    {
        send(sock , message.c_str() , message.length() , 0 );
        cout<<"Message sent : " << message ; 
    }

    //creates and assigns the fd value to sock member
    void createClientSocket(void)
    {
        int opt= 1 ;
        sock = socket(AF_INET , SOCK_STREAM , 0) ;
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Socket creation error \n");
        }

        if (setsockopt(sock , SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
    }

    void initServerAddress(string address , int port )
    {
    
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        
        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET,address.c_str(), &serv_addr.sin_addr)<=0) 
        {
            printf("\nInvalid address/ Address not supported \n");
        }
    }

    int connectToServer(string address , int port)
    {
        createClientSocket() ; 
        initServerAddress(address , port) ; 

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("\nConnection Failed \n");
        }
    }

    void readData()
    {

        valread = read( sock , buffer, 1024);
        if(valread>0)
        {
            cout<<"\nMessage received : " << buffer ;     
        }
        else
        {
            cout<<"\nNo message from server yet :( " ; 
        }
    }

    void closeSocket()
    {
        close(sock) ; 
    }
};

int main(int argc , char *argv[])  
{  

    socketHandler sock_obj ; 
    sock_obj.connectToServer("127.0.0.1" , 8888) ; 
    sock_obj.sendData("Hello its me ! ") ; 
    sock_obj.readData() ; 
    sock_obj.closeSocket() ;
        
    return 0;  
}  

