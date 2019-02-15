# Snakes (LAN Multiplayer console game)
 
**A LAN Multiplayer Snake game in C++ . This is a snake game which is a console based game written in C++ to which any number of players in LAN can join and  play with their corresponding snakes with smooth synchronization.**

Features :-
-------------

+ Game has two related programs , this is the Game Controller (main program) and other one is the client program which can be used to join the multiplayer mode of the game in a LAN network ." ; 

+ **Different Modes :** \"Single\" or \"Multiplayer\" modes." ; 
+ **Parallel Control :** Parallel controlling of all the snakes in multiplayer mode without any delay. This greatly ups user experience."  ; 
+ **Speed Control :** User can change the speed of the snakes while playing." ; 
+ **Scoring System :** It has a scoring system for both single and multiplayer modes." ; 
+ **Looping screen :**  On going through the edge of screen , the snake pops out of the opposite edge." ; 
+ **Beautiful colors:** The snakes will be presented with good looking colors." ; 


-------------



**Coming to Client Side Program : "** :

+ **LAN MULTIPLAYER option:** Play with anyone in the same network and see the snakes in both the screen play in synchronization." ; 
+ Speed Changes in the Game Controller reflects back at the client side too . " ; 
+ Food generated at the server side and client side are also synced." ; 
+ On gameover of any client , the players are notified of that player's final score which is shown at the Game Controller." ; 
+ Client user can either choose to have his snake displayed in his screen in addition to the snake on the game controller or to only control the snake on game controller without anything displayed on his screen. This is useful , if the client only wants to use his keyboard as a controller." ; 


---------------

#### Implementation :
+ Used Socket programming and System calls for networking
