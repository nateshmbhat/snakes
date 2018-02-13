#include<bits/stdc++.h>
#include<iostream>
#include<unistd.h>
#include<string>
#include<ncurses.h>
#include<cstdlib>
#include "./server/server.h"
#include<fstream>
#include<regex>

#define RED 7
#define GREEN 2
#define YELLOW 3
#define BLUE 1
#define BLACK 0
#define CYAN 4
#define WHITE 8
#define MAGENTA 5

#define coloron(a) attron(COLOR_PAIR(a))
#define coloroff(a) attroff(COLOR_PAIR(a))

using namespace std ; 

class snake ; 
class Game ;


//global variables for the program 
int max_x  = 0 , max_y = 0  ;  //Make max_x and max_y as global since the values are used by many methods 
ofstream logfile;

std::ostringstream ss;

typedef struct food
{
    int x,y ;
    char foodChar ;
}food ; 


class Game 
{
    friend snake ; 
    private:
    int noOfPlayers ;
    string gamemode ; 
    socketHandler server ; 
    vector<int> clients ; //contains all the LAN clien't sds
    food foodObj ;
    int center_x , center_y; 
    unsigned long int speed ; 

    public:

    vector <snake> allSnakes ;
    Game(int players = 1 )
    {
        noOfPlayers= players ; 
        speed = 70000; 
        foodObj.x  = foodObj.y = 0 ;
    }

    friend void signalHandler(int) ; 
    
    void generateFood() ; 
    int getSnakeIndexFromDescriptor(int) ; 
    void showInitialChoices() ; 
    void syncSnakeWithClient(snake &) ; 
    int getSnakeIndexFromID(int) ; 
    void reset_max_screen() ; 
    void printAnimated(string ,  int =40000) ; 
    void draw_all_snakes() ; 
    void KeyPressHandler() ; 
    void printFood(string ) ;
    void MainEventLoop() ; 
    void ask_no_players(string) ;
    void moveAllSnakes() ; 
    void LAN_sendFoodCoordinates(int , int ) ; 
    void setFoodPos(int , int) ;
    void printScores() ; 
    void handleNewConnection() ; 
    void handleIOActivity() ; 
    int checkClientActivity() ; //returns activity number
    void initServerForMultiplayer() ; 
    void setClientsList() ; 
    void handleActivity() ; 
    int getFoodX() 
    {return foodObj.x ; }
    int getFoodY()
    {return foodObj.y; }
    int getCenterX(){return center_x ; }
    int getCenterY(){return center_y ; }


    void setGameMode(string mode){gamemode = mode ; }
    void setClientsList(vector<int> clientslist){clients = clientslist ; }
    string  getGameMode(){return gamemode ; }

    food getFoodPos() ;
    int getNoOfPlayers(){return noOfPlayers ; }
    void setNoOfPlayers(int n){noOfPlayers  = n ; }
    void initConsoleScreen(string ) ; 
    unsigned long int setSpeed(unsigned long int s){speed = s>200000?200000:s<10000?10000:s ; return speed ; } //Speed Increments by 3000
    long int getSpeed(void){return speed; }
};
Game GameObj(1) ;


class snake_part
{
    public :
    int x , y ; 

    snake_part(int  xx, int yy )
    {
        x = xx ; y = yy ; 
    }
};



class snake 
{
    friend Game ; 
    private :
    vector <snake_part> parts ;
    string snakeDirection  ;
    int bodycolor ; 
    int socket_descriptor ; 
    int score ;
    string player_sight ; //what the player sees and plays 
    char keyUp , keyDown , keyRight , keyLeft ; 
    string player_name ; 
    int id ; 

    public :
    snake(char up , char down , char right , char left , int snakeid  , int sd = -1 , string name ="Name" , string sees="s")
    {
        keyUp = up , keyDown = down , keyRight = right , keyLeft = left ; 
        score = 0 ; 
        snakeDirection = "right" ; 
        id = snakeid ; 
        socket_descriptor = sd ; 
        player_name = name+std::to_string(-1*id) ;
        player_sight = sees ; 
        bodycolor =  rand()%5+1; 
        logfile<<"\n\nbody color : " <<bodycolor <<endl<<endl; 
    }

    friend void signalHandler(int) ; 

    int getScore(void){return score ; }
    int getID(){return id ; }
    int setScore(int s){score = s ; return score ; }
    void setPlayerName(string name){player_name = name ;}
    void gameOverHandler() ; 
    void setPlayerSight(string sight){player_sight = sight; }
    void setBodyColor(int color){bodycolor = color ; } 
    string getPlayerSight(){return player_sight ; }
    int getSocketDescriptor(){return socket_descriptor ; }
    void draw_snake() ; 
    void add_part(int , int , string) ; 
    void init_snake_on_screen() ; 
    void move_snake(string) ; 
    void check_snake_overlap() ; 
    void handleMovementKeyPress(char) ; 

    int getHeadX(void){return parts.at(parts.size()-1).x;}
    int getHeadY(void){return parts.at(parts.size()-1).y;}
    string getDirection(void){return snakeDirection; }
};



void snake::draw_snake(void)
{
    int i ;
    coloron(bodycolor) ; 
    for(  i =0 ; i<parts.size()-1 ; i++)
    {
        mvprintw(parts[i].y  , parts[i].x , "o") ; 
    }
    coloroff(bodycolor) ; 
    mvprintw(parts[i].y  , parts[i].x , "+") ; 
}

void snake::add_part(int x , int y , string direction = "right" ) //adds the part object taking the coordinates to the end of the part vector in snake
{
    snake_part obj(x , y) ; 
    if(direction=="right")
        parts.push_back(obj) ; 

    else if(direction=="left")
        parts.insert(parts.begin() ,obj) ; 
}

//called to make the snake appear on the screen for the first time 
void snake::init_snake_on_screen()
{
    add_part(GameObj.getCenterX() , GameObj.getCenterY()) ; 
    add_part(GameObj.getCenterX()+1 , GameObj.getCenterY()) ; 
    add_part(GameObj.getCenterX()+2 , GameObj.getCenterY()) ; 
    
    draw_snake() ;
}



//Used to move the snake in the given direction 
void snake::move_snake(string direction)
{
    parts.erase(parts.begin())  ;
    snake_part last_part = parts.at(parts.size()-1) ;

    if(direction=="right")
    {
        add_part((last_part.x+1)%max_x  , last_part.y) ;
        snakeDirection  = "right" ; 
    }

    else if(direction =="left") 
    {
        add_part((last_part.x-1)<0?max_x-1:(last_part.x-1) , last_part.y) ; 
        snakeDirection = "left" ; 
    }

    else if(direction =="up")
    {
        add_part(last_part.x , (last_part.y-1)<0?max_y-1:(last_part.y-1)) ;
        snakeDirection = "up" ; 
    }
    else if(direction=="down")
    {
        add_part(last_part.x , (last_part.y+1)%max_y) ;
        snakeDirection = "down" ; 
    }

    check_snake_overlap() ;

    if(getHeadX()==GameObj.getFoodX() && getHeadY() == GameObj.getFoodY())
    {
        //Only accept food if the snake is an offline local snake
        if(socket_descriptor<0 || player_sight=="s")
        {
        add_part(GameObj.getFoodX() , GameObj.getFoodY() ) ;
        setScore(getScore()+1) ; 
        GameObj.printFood("new") ;
        }
    }

    draw_snake() ;
    refresh() ; 
}



//Checks if the snake bites itself or not and then handles it 
void snake::check_snake_overlap() //also handler
{
    int headX = getHeadX() , headY  = getHeadY() ; 
    for(int i =0 ; i<parts.size()-1 ; i++)
    if(parts[i].x==headX && parts[i].y==headY)
        {
            //dont gameover if the player_sight is c and if it is a client . There is seperate message handler to handle .
            //gameover when the client sends the gameover message . Only then gameover
            if(socket_descriptor>0 && player_sight=="c") return ; 
            if(player_sight=="s")
                GameObj.server.sendData(socket_descriptor , "$"+std::to_string(score)+"$") ; 
            gameOverHandler() ; 
        }
}


void snake::handleMovementKeyPress(char ch )
{
    if(keyUp==ch){ if(getDirection() !="down") move_snake("up") ; } 
    else if(keyDown==ch){ if(getDirection()!= "up")move_snake("down") ; } 
    else if(keyRight==ch){ if(getDirection()!="left")move_snake("right") ; } 
    else if(keyLeft==ch){ if(getDirection()!="right")move_snake("left") ; } 
    else return ; 
}


void Game::printAnimated(string msg , int speed)
{
for(int c = 0  ; msg[c] ; c++)
    {
        cout<<msg[c] ; 
        cout.flush() ; 
        usleep(speed) ; 
    }
}


int Game::getSnakeIndexFromID(int id)
{
    for(int i =0 ; i<allSnakes.size() ; i++)
    {
        if(allSnakes[i].getID()==id)
            return i ; 
    }
}

void Game::printScores()
{
    int s  = 0 ; 
    string temp ; 

    coloron(WHITE) ; 
    for(int i =0 ; i<allSnakes.size() ; i++)
    {
        temp = allSnakes[i].player_name ; 
        mvprintw(0 , s,"%s:%d" , temp.c_str() , allSnakes[i].getScore()); 
        s+= temp.length()+4 ; 
    }
    coloroff(WHITE) ; 
}


void snake::gameOverHandler()
{
    clear() ;
    GameObj.initConsoleScreen("off") ;
    system("clear") ; 
    string gameovermessage = "\n\n\nGAME OVER FOR " + player_name+" :(\n\n" ; 
    gameovermessage+="Score : " + std::to_string(score)+"\nBetter Luck Next time :)\n\n" ; 

    GameObj.printAnimated(gameovermessage) ; 
    
    if(socket_descriptor>0)
    {
        GameObj.allSnakes.erase(GameObj.allSnakes.begin()+GameObj.getSnakeIndexFromDescriptor(socket_descriptor)) ;
        GameObj.server.closeSocket(socket_descriptor) ; 
    }

    else GameObj.allSnakes.erase(GameObj.allSnakes.begin()+GameObj.getSnakeIndexFromID(id)) ; 

    GameObj.setNoOfPlayers(GameObj.getNoOfPlayers()-1) ; 
    
    if(GameObj.getNoOfPlayers()==0)
    {
        GameObj.initConsoleScreen("off") ; 
        cout<<"\nNo Snakes Left to Play :(  Exiting Game ..." <<endl<<endl; 
        sleep(2) ;
        exit(3) ; 
    }

    GameObj.printAnimated("\n\nGame will continue in few seconds." ) ; 
    sleep(3) ;
}


void draw_border_window(int  , int) ; 
void draw_food(void) ; 

void draw_border_window( int max_x , int max_y)
{
    for(int i =0 , j = 1 ; (i<max_x || j<max_y ) ; )
    {
        if(i<max_x)
            mvprintw(1,i++,"--") ;

        if(j<max_y)
            mvprintw(j++ , 0 , "|") ; 
    }

    max_x -=1 ; max_y-=1 ; 

    for(int i =max_x , j = max_y ; (i>0 || j>0) ; )
    {
        if(i>0)
            mvprintw( max_y , i--  , "--") ; 

        if(j>0)
            mvprintw(j--,max_x  , "|") ; 
        
    }
}



void charecter_code_testing_fun(void)
{
    //CHARACTER CODE TESTING FUNTION
    char prev_char  ,ch  ; 
    for(;;)
    {
        ch = getch() ; 
        if(ch!=ERR) prev_char = ch ; 
        mvprintw(10 , 10 , "Char = %c  : %d" , prev_char , prev_char) ; 
        usleep(10000) ; 
    }
}

void printSpeed(snake snk)
{
    coloron(CYAN) ; 
    mvprintw(0 , max_x-15 , "Speed(+/-)= %lu" ,GameObj.getSpeed()/100) ; 
    refresh() ;
    coloroff(CYAN) ; 
}


void Game::ask_no_players(string player="single")
{
    char ch ; 

    for(;;)
    {
        ch = getch() ; 
        if(ch==27)
        {
            clear() ; 
            getch() ; 
            ch = getch() ; 
            if(ch=='A')
            {
                player = "single" ; 
            }

            else if(ch=='B')
            {
                player = "multi" ; 
            }
        }

        else if(ch==10)
        {
            //If enter key pressed break from the loop 
            GameObj.setGameMode(player) ; 
            break ; 
        }

    coloron(BLUE) ; 
    mvprintw(1 , 5 , " SINGLE PLAYER or") ;
    coloroff(BLUE) ; 
    coloron(GREEN) ; 
    mvprintw(1 +1, 5 , " MULTI PLAYER ? ") ;
    coloroff(GREEN) ; 

    if(player=="single")
        mvprintw(1 , 2 , ">>>") ; 
    else if(player=="multi")
        mvprintw(2 , 2, ">>>") ; 
    
    refresh() ; 
    usleep(20000) ; 
    clear() ;

    }


    if(GameObj.getGameMode()=="multi")
        {
            flushinp() ; 
            GameObj.initConsoleScreen("off") ; 
            system("clear")  ;
            int no_of_players ; 
            cout<<"Enter the number of players on this device (any number of LAN clients can join this play ) : "  ; 
            cin>> no_of_players;
            GameObj.setNoOfPlayers(no_of_players) ; 
        }


    //Add the first snake object
    if(GameObj.getNoOfPlayers())
    {
        snake first_snake  = snake('A' , 'B' , 'C' , 'D' , 0 ) ; 
        first_snake.setPlayerName("Default" ) ; 
        GameObj.allSnakes.push_back(first_snake) ;
    }

     for(int i =1 ; i<GameObj.getNoOfPlayers() ; i++)
    {

        char left , right , up , down ; 
        cout<<"Player "<<i<<" = Enter the key for Left Right Up Down  : " ;
        cin>>left >>right >>up >>down ; 

        snake * snk = new snake(up , down , right , left , -i) ; 
        GameObj.allSnakes.push_back(*snk) ; 
    }

    //Turn on the coordinate kind of behaviour for the terminal
    GameObj.initConsoleScreen("on") ; 
}


//Right now this is the MAIN KEYS HANDLING FUNCTION Which will handle all the keystrokes entered 
void Game::KeyPressHandler()
{
    char ch ; 
    if((ch = getch())!=ERR)
    {
        if(ch==27)
        {
            getch() ; // clear and reject 91 from buffer
            ch = getch() ;//Now store the actual value of arrow key pressed               getch() ; 
            if(GameObj.getNoOfPlayers())
                allSnakes[0].handleMovementKeyPress(ch) ;
        }

        if (GameObj.getGameMode()=="multi"){
            for(int i =0 ; i<allSnakes.size() ; i++)
            {
                //only move if the key press occurs for offline snakes
                if(allSnakes[i].getSocketDescriptor()<0 && allSnakes[i].getID()<0)
                    allSnakes[i].handleMovementKeyPress(ch) ; 
            }
        }


        //Decrease the snake speed
        if(ch==45)
        {
            GameObj.setSpeed(GameObj.getSpeed()+3000) ;
            if(GameObj.getGameMode()=="multi")
            {
                for(int temp =0 ; temp<allSnakes.size() ; temp++)
                {
                    int sd = allSnakes[temp].getSocketDescriptor() ; 
                    if(sd>0)
                    {
                        server.sendData(sd , "-") ; 
                    }
                }
            }
        }


        //Increase the snake speed
        if(ch==43)
        {
            GameObj.setSpeed(GameObj.getSpeed()-3000); 
            if(GameObj.getGameMode()=="multi")
            {
                for(int temp =0 ; temp<allSnakes.size() ; temp++)
                {
                    int sd = allSnakes[temp].getSocketDescriptor() ; 
                    if(sd>0)
                    {
                        server.sendData(sd , "+") ; 
                    }
                }

            }

        }

    }
    
}


void Game::draw_all_snakes()
{
    //add the first 3 dots to the snake 
    for(int i =0; i<allSnakes.size(); i++)
    {
        allSnakes[i].add_part(center_x+5 , center_y) ; 
        allSnakes[i].add_part(center_x+6 , center_y) ; 
        allSnakes[i].add_part(center_x+7 , center_y) ;
        allSnakes[i].draw_snake() ;
    }
}


void Game::moveAllSnakes()
{
    for(int i =0 ; i<allSnakes.size() ; i++)
        {
            allSnakes[i].move_snake(allSnakes[i].getDirection()) ;
        }

}

//Setter method to set the position of food 
void Game::setFoodPos(int x , int y)
{
   foodObj.x = x ; 
   foodObj.y = y ;  
}


//Initialise the console with the decision to turn off or on the enter key and cursor
void Game::initConsoleScreen(string state)
{
    if(state=="on")
    {
        initscr() ; //Init screen 
        noecho() ; // Dont show any pressed char  
        curs_set(false) ; // Don't show the cursor 
        start_color() ; 
        getmaxyx(stdscr , max_y , max_x) ; 
        center_x = max_x/2  , center_y = max_y/2 ; 
        cbreak() ; //Dont wait for enter to be pressed when using getch 
        nodelay(stdscr , 1) ;  //Use non blocking input for getch which just returns ERR if there is no input (ERR=-1)
    }

    else if(state=="off"){
        clear() ; 
        flushinp() ; 
        fflush(stdin) ; 
        use_default_colors() ; 
        endwin() ; 
        system("clear") ; 
    }
}


//Returns food obj with a copy of data of foodObj member of game class
food Game::getFoodPos()
{
    food obj = foodObj ;
    return obj ;
}


//sends the coordinates of food to all the clients  :15,20
void Game::LAN_sendFoodCoordinates(int x , int y)
{
    for(int temp= 0 ; temp<allSnakes.size() ; temp++)
    {
        int sd  = allSnakes[temp].getSocketDescriptor() ; 

        if(sd>0)
        {
            char foodcoord[20] ; 
            sprintf(foodcoord , ":%03d,%03d" ,x , y) ; 

            server.sendData(sd , string(foodcoord)) ; 
        }
    }
}


void Game::generateFood()
{
    int x = random()%max_x , y = random()%max_y  ; 

    if(x<2)x = 2 ; 
    if(y<2) y = 2 ; 

    logfile<<"\n\nGenerating food at pos:"<<x <<","<<y<<"\n\n" ; 
    coloron(RED) ; 
    mvprintw(y, x ,"#") ;   
    coloroff(RED) ; 
    setFoodPos(x , y) ;

    if(gamemode=="multi")
    {
        LAN_sendFoodCoordinates( getFoodX() , getFoodY()) ; 
    }
}

void Game::printFood(string status="old")
{
    if(status=="new")
        generateFood() ; 
    
    if(!getFoodX() && !getFoodY())
        generateFood() ; 
    coloron(RED) ; 
    mvprintw(getFoodY(), getFoodX() ,"#") ;   
    coloroff(RED) ; 
}



//THIS IS A VERY IMPORTANT FUNCTION WHICH SHOULD HAVE BEEN INCLUDED IN THE MAIN FUNCTION , BUT FOR CONVENIENCE , ITS INCLUDED HERE . 
//HANDLES EVERYTHING UNTILL THE MAIN EVENT LOOP STARTS IN THE main function . 
void HANDLE_EVERYTHING_TILL_EVENT_LOOP()
{
    srand(time(NULL)) ;
    GameObj.initConsoleScreen("on") ; 

    init_pair(RED , COLOR_RED , COLOR_BLACK) ; 
    init_pair(YELLOW , COLOR_YELLOW , COLOR_BLACK) ; 
    init_pair(GREEN , COLOR_GREEN , COLOR_BLACK) ;
    init_pair(WHITE , COLOR_WHITE , COLOR_BLACK) ; 
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK) ;
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK) ;
    init_pair(CYAN , COLOR_CYAN , COLOR_BLACK) ; 

    //Asks the number of players who want to play  (single or multiplayer (with 2 players )) ; 
    GameObj.ask_no_players() ; 

    GameObj.draw_all_snakes() ; 
}


void Game::initServerForMultiplayer()
{
    server.bindServer() ; 
    server.setupClientDescriptors() ; 
    server.startServer() ; 
}


void Game::handleNewConnection()
{
    int client_socket = server.handleNewConnection() ;
    snake * clientSnakePtr = new snake('A' , 'B' , 'C' , 'D' , 0 , client_socket) ; 
    GameObj.allSnakes.push_back(*clientSnakePtr) ; 
    GameObj.setNoOfPlayers(GameObj.getNoOfPlayers()+1) ; 

    //Add the initial parts of the snake 
    allSnakes[allSnakes.size()-1].add_part(center_x+5 , center_y) ; 
    allSnakes[allSnakes.size()-1].add_part(center_x+6 , center_y) ; 
    allSnakes[allSnakes.size()-1].add_part(center_x+7 , center_y) ;

    GameObj.generateFood() ; 
}


void Game::syncSnakeWithClient(snake& clientsnake)
{
    //Only call this when the client snakes eats a food 
    //food coordinate represents client snake's head. Translate the snake with the the offset in reference to the food coord.
    int x_diff = getFoodX() - clientsnake.getHeadX() ;
    int y_diff = getFoodY()  - clientsnake.getHeadY() ; 

    for(int i =0 ; i<clientsnake.parts.size() ; i++)
    {
        clientsnake.parts[i].x += x_diff ;
        clientsnake.parts[i].y+=y_diff ; 
    }

}


void Game::handleIOActivity()
{
    string msg ; 
    int snake_index ; 

    for(int i =0 ; i<clients.size() ; i++)
    {
        snake_index = getSnakeIndexFromDescriptor(clients[i]) ; 

        msg = server.handleIOActivity(clients[i]) ;   //handleIOActivity takes a client sd from the list of client sds which  and returns the string sent by client 

        //Handle disconnected clients 
        if(msg=="")
        {
            GameObj.initConsoleScreen("off") ; 
            system("clear") ; 
            cout<< allSnakes[snake_index].player_name <<" left the game.\n" ; 
            cout<<"Score : " <<allSnakes[snake_index].score <<"\n\n";
            GameObj.server.closeSocket(clients[i]) ; 
            allSnakes.erase(allSnakes.begin()+snake_index) ; 
            setNoOfPlayers(getNoOfPlayers()-1) ; 
            sleep(2) ; 
            GameObj.initConsoleScreen("on") ; 
        }


        else if(msg.find("init~~")!=string::npos)
        {
            LAN_sendFoodCoordinates(foodObj.x , foodObj.y) ; 
            string name="" , sight="" ; 
            int pos = msg.find("init~~") ;
            int i ; 

            for( i =6 ; msg[i]!='~' ; i++)
            {
                name+=msg[i] ; 
            }

            sight = msg[i+2] ; 

            logfile<<endl<<"\nName of player : " <<name <<"  , sight = " <<sight << endl<<endl; 

            GameObj.initConsoleScreen("off") ;   
            system("clear") ; 
            cout<<"\n\n"<<name<<" joined the game.\n\n" ; 
            sleep(1) ; 
            system("clear") ; 
            GameObj.initConsoleScreen("on") ; 
            allSnakes[snake_index].setPlayerName(name) ;  
            allSnakes[snake_index].setPlayerSight(sight) ;
            allSnakes[snake_index].setBodyColor(int(msg[msg.find("&")+1]-'0')) ; 
        }


        else if(msg.find("$")!=string::npos)
        {
            //game over handle if recevies "$" msg from client .  
            int start = msg.find("$") ;
            allSnakes[snake_index].gameOverHandler() ; 
        }

        
        //Handle key press
        else{
                for(int c = 0 ; c<msg.length() ; c++)
                {
                    if(msg[c]=='#')
                    {
                        logfile<<"\nMessage from clietn handler :\n "<<msg<<"\n" ; 
                        
                        //Client has eaten a food . Increase its part 
                        allSnakes[snake_index].add_part(allSnakes[snake_index].getHeadX() , allSnakes[snake_index].getHeadY()) ; 

                        allSnakes[snake_index].score++ ; 

                        syncSnakeWithClient(allSnakes[snake_index]) ; 
                        
                        printFood("new") ; 

                        if(gamemode=="multi")
                        {
                            LAN_sendFoodCoordinates( getFoodX() , getFoodY()) ; 
                        }

                    }

                    else if(msg[c]=='A' or msg[c]=='B' or msg[c]=='C' or msg[c]=='D')
                        allSnakes[snake_index].handleMovementKeyPress(msg[c]) ; 
                }
        }

        logfile<<"\n\nMessage from client : "<<msg <<endl<<endl ; 
    }
}



int Game::getSnakeIndexFromDescriptor(int sd)
{
    for(int temp=0 ; temp< allSnakes.size() ; temp++)
        {
            if(sd ==allSnakes[temp].getSocketDescriptor())
            {
                return temp ; 
            }
        }
}



void Game::handleActivity()
{
    //set the clients of game object ; 
    clients = server.handleActivity() ; 

    if(clients[0]==-1)
    {
        //no client is sending any msg , its a new conn
        handleNewConnection() ; 
    }

    else handleIOActivity() ; 
}


int Game::checkClientActivity(){
    return server.checkClientActiviy() ; 
}

void Game::reset_max_screen()
{
    getmaxyx(stdscr , max_y , max_x ) ; 
}

void Game::showInitialChoices()
{
    system("clear") ; 
    cout<<"\n1. Start Game *"  ; 
    cout<<"\n2. Show Features" ; 
    cout<<"\n3. Exit " ;
    cout<<"\n\nEnter choice : " ; 
    int ch ; 
    cin>>ch ; 
    string feat="" ;  //features string 

    switch(ch)
    {
        case 1:
            return ; 
            
        case 2:
            feat+="\nGame has two related programs , this is the Game Controller (main program) and other one is the client program which can be used to join the multiplayer mode of the game in a LAN network ." ; 
            feat+="\n\nHere are the features of the Game Controller : " ;
            feat+="\n1. Different Modes : \"Single\" or \"Multiplayer\" modes." ; 
            feat+="\n2. Parallel Control : Parallel controlling of all the snakes in multiplayer mode without any delay. This greatly ups user experience."  ; 
            feat+="\n3. Speed Control : User can change the speed of the snakes while playing." ; 
            feat+="\n4. Scoring System : It has a scoring system for both single and multiplayer modes." ; 
            feat+= "\n5. Looping screen :  On going through the edge of screen , the snake pops out of the opposite edge." ; 
            feat+="\n6. Beautiful colors: The snakes will be presented with good looking colors." ; 

            feat+="\n\n\nComing to Client Side Program : " ; 
            feat+="\n\n7. LAN MULTIPLAYER option: Play with anyone in the same network and see the snakes in both the screen play in synchronization." ; 
            feat+="\n8. Speed Changes in the Game Controller reflects back at the client side too . " ; 
            feat+="\n9. Food generated at the server side and client side are also synced." ; 
            feat+="\n10. On gameover of any client , the players are notified of that player's final score which is shown at the Game Controller." ; 
            feat+="\n11. Client user can either choose to have his snake displayed in his screen in addition to the snake on the game controller or to only control the snake on game controller without anything displayed on his screen. This is useful , if the client only wants to use his keyboard as a controller." ; 

            GameObj.printAnimated(feat , 10000) ; 

            cout<<"\n\nGo back ? (y) : " ; 
            char temp ; 
            cin>>temp ; 
            break; 

        case 3:
            exit(5) ; 
        default:
            return ; 
    }

    showInitialChoices() ; 
}


void signalHandler(int code)
{
    GameObj.initConsoleScreen("off") ; 
    cout.flush() ;
    stringstream out ; 
    out<<"1.Continue" <<endl; 
    out<<"2.Credits" <<endl; 
    out<<"3.Exit" <<endl; 
    out<<"\nEnter choice : " <<endl; 
    cout<<out.str() ; 
    int ch ; 
    cin>>ch ; 
    string credits="" ; 

    switch(ch)
    {
        case 2:
            credits+="\nProgrammer  : Natesh M Bhat" ; 
            credits+="\nLanguage    : C++" ; 
            credits+="\nEditor      : Sublime and Vscode" ; 
            credits+="\nBig Thanks to Prabodh sir for making Object Oriented Concepts clear enough to enable me to code this game. ^_^" ; 
            credits+="\nDedicated to Kavita mam (MC) :)" ; 
            GameObj.printAnimated(credits) ; 
            sleep(3) ; 
            break ; 

        case 3:
            GameObj.server.stopServer() ;
            logfile.close() ; 
            GameObj.printAnimated("\nThanks for playing ^_^ . Cya.....") ; 
            exit(1) ; 
            break ; 
        case 1:
            GameObj.initConsoleScreen("on") ; 
            break ; 
        default:
            GameObj.server.stopServer() ; 
            logfile.close() ; 
            exit(1) ;
    }
}


//MAIN FUNCTION 
int main(int argc , char * argv[]) 
{


    logfile.open("logfile.log" , ios::out) ; 
    GameObj.showInitialChoices() ; 

    signal(SIGINT, signalHandler);
    HANDLE_EVERYTHING_TILL_EVENT_LOOP() ; 
    int activity ; 
    

    if(GameObj.getGameMode()=="multi")
    {
        GameObj.initServerForMultiplayer() ; 
    }
   
    // draw_border_window( max_x , max_y); 

    for(;;)
    {
        GameObj.reset_max_screen() ; 
        GameObj.KeyPressHandler() ; //Handles key presses 

        if(GameObj.getGameMode()=="multi")
        {
            activity = GameObj.checkClientActivity() ;

            if(activity>=1)
            {
                GameObj.handleActivity() ; 
            }
            
        }


        flushinp();
        clear() ;

        GameObj.moveAllSnakes() ; 
        GameObj.printScores() ; 

        // if(no_players=="multi"){  snk1.printScore("right") ; snk1.move_snake(snk1.getDirection());  }

        if(GameObj.getNoOfPlayers())
            printSpeed(GameObj.allSnakes[0]) ; 

        GameObj.printFood() ;
        refresh() ;
        usleep(GameObj.getSpeed()) ;
    }
 
}
