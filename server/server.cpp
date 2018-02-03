//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux 
#include <stdio.h> 
#include <string.h>   //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include<iostream>
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

    int opt = TRUE;  
    int master_socket , PORT , addrlen , new_socket , client_socket[30] , 
          max_clients , activity, i , valread , sd;  
    int max_sd;  
    struct sockaddr_in address;  
    char message[50] ; 
        
    char buffer[1025];  //data buffer of 1K 
        
    //set of socket descriptors 
    fd_set readfds; 

    void initServerSocket()
    {
        //create a master socket 
        if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
        {  
            perror("socket failed");  
            exit(EXIT_FAILURE);  
        }  
        
        //set master socket to allow multiple connections , 
        if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
            sizeof(opt)) < 0 )  
        {  
            perror("setsockopt");  
            exit(EXIT_FAILURE);  
        }  

    }
        

    public : 

    //initialise all client_socket[] to 0 so not checked 
    socketHandler(void)
    {
        max_clients = 30 ; 
        valread = 0 ; 
        PORT = 8888 ; 

        //initialise all client_socket[] to 0 so not checked 
        memset(client_socket , 0 , sizeof(client_socket)) ; 
        memset(&address, '0', sizeof(address));
        memset(buffer , 0 ,sizeof(buffer)) ; 
        strcpy(message , "Successfully Connected  \r\n") ; 
    }


    void bindServer()
    {
        initServerSocket() ; 

        //type of socket created 
        address.sin_family = AF_INET;  
        address.sin_addr.s_addr = INADDR_ANY;  
        address.sin_port = htons( PORT );  
            
        //bind the socket to localhost port 8888 
        if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)  
        {  
            perror("bind failed");  
            exit(EXIT_FAILURE);  
        }  
    }


    void setupClientDescriptors()
    {
        //clear the socket set 
        FD_ZERO(&readfds);  
    
        //add master socket to set 
        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;  
            
        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++)  
        {  
            //socket descriptor 
            sd = client_socket[i];  
                
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
 
    }


    void startServer()
    {
        printf("Listener on port %d \n",PORT );  
            
        //try to specify maximum of 3 pending connections for the master socket 
        if (listen(master_socket, 3) < 0)  
        {  
            perror("listen");  
            exit(EXIT_FAILURE);  
        }  
            
        //accept the incoming connection 
        addrlen = sizeof(address);  
        puts("Waiting for connections ...");  

    }


    int checkClientActiviy()
    {
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        setupClientDescriptors() ; 

        puts("\nBefore select : \n") ; 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
        printf("\nSelect returned : %d \n" , activity) ; 
      
        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error\n");  
        }  

        return activity ; 
    }



    void closeSocket()
    {
        close(master_socket) ; 
    }

    void handleNewConnection()
    {
        puts("\nInside FD_ISSET master_socket") ; 
            if ((new_socket = accept(master_socket, 
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
            {  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
            
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                  (address.sin_port));  
          
            //send new connection greeting message 
            if( send(new_socket, message, strlen(message),  MSG_NOSIGNAL) != strlen(message) )  
            {  
                perror("send");  
            }  
                
            puts("Welcome message sent successfully");  
                
            //add new socket to array of sockets 
            for (i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                        
                    break;  
                }  
            }  
    }


    void handleIOActivity(int client_sd)
    {
        //Check if it was for closing , and also read the 
        //incoming message 
        printf("\nFD_ISSET inside the IO operation handler : client_sd : %d\n" , client_sd)  ;
        if ((valread = read( client_sd , buffer, 1024)) <= 0)  
        {  
            //Somebody disconnected , get his details and print 
            getpeername(client_sd , (struct sockaddr*)&address , \
                (socklen_t*)&addrlen);  
            printf("Host disconnected , ip %s , port %d \n" , 
                    inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                
            //Close the socket and mark as 0 in list for reuse 
            close( client_sd );  
            client_socket[i] = 0;  
        }  
            
        //Echo back the message that came in 
        else
        {  
            //set the string terminating NULL byte on the end 
            //of the data read 
            buffer[valread] = '\0';  
            cout<< "\Message from client : " << buffer << " ; Length : " <<valread ;

            send(client_sd , buffer , strlen(buffer) , 0 );  
        }  
    }


    void handleActivity()
    {
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(master_socket, &readfds))  
        {  
           handleNewConnection() ;  
        }  

        else{
            //else its some IO operation on some other socket
            for (i = 0; i < max_clients; i++)  
            {  
                sd = client_socket[i];  
                    
                if (FD_ISSET( sd , &readfds))  
                {  
                    printf("\nCalling IO Activity handler : " ) ; 
                    handleIOActivity(sd) ;                     
                }  
            } 
        }

    }
};




int main(int argc , char *argv[])  
{  
    socketHandler server ; 
    server.bindServer() ; 
    server.setupClientDescriptors() ; 
    server.startServer() ; 

    int activity ;

    while(TRUE)  
    {  
   
        activity = server.checkClientActiviy() ;

        if(activity==1)
        {
            server.handleActivity() ;             
        }
        
        cout<<"\nSleeping for 2 seconds : \n" ; 
        sleep(2) ; 

    }  
        
    return 0;  
}  

