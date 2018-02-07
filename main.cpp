#include<bits/stdc++.h>
#include<iostream>
#include<unistd.h>
#include<ncurses.h>
#include<"./server/server.h">

using namespace std ; 

class snake ; 
class Game ;

//global variables for the program 
int max_x  = 0 , max_y = 0  ;  //Make max_x and max_y as global since the values are used by many methods 


typedef struct food
{
    int x,y ;
    char foodChar ;
}food ; 


class Game 
{
    private:
    int noOfPlayers ;
    string gamemode ; 
    socketHandler server ; 
    vector<int> clients ; 
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
    
    void generateFood() ; 
    void draw_all_snakes() ; 
    void KeyPressHandler() ; 
    void printFood(string ) ;
    void MainEventLoop() ; 
    void ask_no_players(string) ; 
    void moveAllSnakes() ; 
    void setFoodPos(int , int) ;
    void handleNewConnection() ; 
    void handleIOActivity() ; 
    void handleMessageFromServer(string ) ;
    void initServerForMultiplayer(socketHandler & ) ; 
    int getFoodX() ; 
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
    private :
    vector <snake_part> parts ;
    string snakeDirection  ;
    int score ;
    char keyUp , keyDown , keyRight , keyLeft ; 
    int id ; 

    public :
    snake(char up , char down , char right , char left , int id )
    {
        keyUp = up , keyDown = down , keyRight = right , keyLeft = left ; 
        score = 0 ; 
        snakeDirection = "right" ; 
        id = id ; 
    }

    int getScore(void){return score ; }
    int setScore(int s){score = s ; return score ; }


    void draw_snake(void)
    {
        int i ; 
        for(  i =0 ; i<parts.size()-1 ; i++)
        {
            mvprintw(parts[i].y  , parts[i].x , "o") ; 
        }
        mvprintw(parts[i].y  , parts[i].x , "+") ; 
    }

    void add_part(int x , int y , string direction = "right" ) //adds the part object taking the coordinates to the end of the part vector in snake
    {
        snake_part obj(x , y) ; 
        if(direction=="right")
            parts.push_back(obj) ; 

        else if(direction=="left")
            parts.insert(parts.begin() ,obj) ; 
    }

    //called to make the snake appear on the screen for the first time 
    void init_snake_on_screen()
    {
        add_part(GameObj.getCenterX() , GameObj.getCenterY()) ; 
        add_part(GameObj.getCenterX()+1 , GameObj.getCenterY()) ; 
        add_part(GameObj.getCenterX()+2 , GameObj.getCenterY()) ; 
        
        draw_snake() ;
    }

    //Used to move the snake in the given direction 
    void move_snake(string direction)
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
            add_part(GameObj.getFoodX() , GameObj.getFoodY() ) ;
            setScore(getScore()+1) ; 
            GameObj.printFood("new") ;
        }

        draw_snake() ;
        refresh() ; 
    }

//Checks if the snake bites itself or not ! :D 
    int check_snake_overlap()
    {
    int headX = getHeadX() , headY  = getHeadY() ; 
    for(int i =0 ; i<parts.size()-1 ; i++)
        if(parts[i].x==headX && parts[i].y==headY)
            {

                clear() ;
                mvprintw(max_y/2 , max_x/2 -20 , "GAME OVER for Player %d !" , id) ;
                refresh() ; 
                sleep(50000) ;
            }
    }

    void printScore(string pos="left")
    {
        if(pos=="right")
            mvprintw(0 , 15, "Score = %d" , score) ; 
        mvprintw(0 , 0 , "Score = %d" , score) ; 
    }

    void handleMovementKeyPress(char ch )
    {
  
        if(keyUp==ch){ if(getDirection() !="down") move_snake("up") ; } 
        else if(keyDown==ch){ if(getDirection()!= "up")move_snake("down") ; } 
        else if(keyRight==ch){ if(getDirection()!="left")move_snake("right") ; } 
        else if(keyLeft==ch){ if(getDirection()!="right")move_snake("left") ; } 
        else return ; 
      
    }

    

    int getHeadX(void){return parts.at(parts.size()-1).x;}
    int getHeadY(void){return parts.at(parts.size()-1).y;}
    string getDirection(void){return snakeDirection; }

}; 



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
    mvprintw(0 , max_x-20 , "Speed= %lu" ,GameObj.getSpeed()) ; 
    refresh() ;
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

    mvprintw(1 , 5 , " SINGLE PLAYER or") ;
    mvprintw(1 +1, 5 , " MULTI PLAYER ? ") ;

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
            cout<<"Enter the number of players : "  ; 
            cin>> no_of_players;
            GameObj.setNoOfPlayers(no_of_players) ; 
        }


     for(int i =1 ; i<GameObj.getNoOfPlayers() ; i++)
    {

        char left , right , up , down ; 
        cout<<"Player "<<i<<" = Enter the key for Left Right Up Down  : " ;
        cin>>left >>right >>up >>down ; 

        snake * snk = new snake(up , down , right , left , i) ; 
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
            allSnakes[0].handleMovementKeyPress(ch) ;
        }

        else if (GameObj.getGameMode()=="multi"){
            for(int i =1 ; i<allSnakes.size() ; i++)
            {
                allSnakes[i].handleMovementKeyPress(ch) ; 
            }
        }


        //INcrease the snake speed
        if(ch==45)
        {
            GameObj.setSpeed(GameObj.getSpeed()+3000) ;
        }
        //Decrease the snake speed
        if(ch==43)
            GameObj.setSpeed(GameObj.getSpeed()-3000); 

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

void Game::handleMessageFromServer(string msg)
{
    for(int i =0 ; i<msg.length()  ; i++)
    {
        if(msg[i]=='-')
            GameObj.setSpeed(GameObj.getSpeed()+3000) ;             
        else if(msg[i]=='+')
            GameObj.setSpeed(GameObj.getSpeed()-3000) ; 
    }
}



//Initialise the console with the decision to turn off or on the enter key and cursor
void Game::initConsoleScreen(string state)
{
    if(state=="on")
    {
        initscr() ; //Init screen 
        noecho() ; // Dont show any pressed char  
        curs_set(false) ; // Don't show the cursor 
            

        getmaxyx(stdscr , max_y , max_x) ; 
        center_x = max_x/2  , center_y = max_y/2 ; 
        
        cbreak() ; //Dont wait for enter to be pressed when using getch 
        nodelay(stdscr , 1) ;  //Use non blocking input for getch which just returns ERR if there is no input (ERR=-1)

    }

    else if(state=="off"){
        endwin() ; 
    }
}

//Returns food obj with a copy of data of foodObj member of game class
food Game::getFoodPos()
{
    food obj = foodObj ;
    return obj ;
}



void Game::generateFood()
{
    int x = random()%max_x , y = random()%max_y  ; 
    if(!x)x = 2 ; 
    if(!y) y = 2 ; 
    mvprintw(y, x ,"#") ;   
    GameObj.setFoodPos(x , y) ;
}

void Game::printFood(string status="old")
{
    if(status=="new")
        GameObj.generateFood() ; 
    
    if(!GameObj.getFoodX() && !GameObj.getFoodY())
        generateFood() ; 
    mvprintw(GameObj.getFoodY(), GameObj.getFoodX() ,"#") ;   
}





//THIS IS A VERY IMPORTANT FUNCTION WHICH SHOULD HAVE BEEN INCLUDED IN THE MAIN FUNCTION , BUT FOR CONVENIENCE , ITS INCLUDED HERE . 
//HANDLES EVERYTHING UNTILL THE MAIN EVENT LOOP STARTS IN THE main function . 
void HANDLE_EVERYTHING_TILL_EVENT_LOOP()
{

    srand(time(NULL)) ;
    GameObj.initConsoleScreen("on") ; 

    //Add the first snake object
    snake first_snake  = snake('A' , 'B' , 'C' , 'D' , 0) ; 
    GameObj.allSnakes.push_back(first_snake) ;

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
    

}

void Game::handleIOActivity()
{
    server.handleActivity() ;  
}



void Game::handleActivity(clients)
{
    if(clients[0]==-1)
    {
        //no client is sending any msg , its a new conn
        GameObj.handleNewConnection() ; 
    }

    else server.handleIOActivity(clients) ; 
}



int main(int argc , char * argv[]) 
{
    
    HANDLE_EVERYTHING_TILL_EVENT_LOOP() ; 
    

    if(GameObj.getGameMode()=="multi")
    {
        GameObj.initServerForMultiplayer() ; 
    }
   
    // draw_border_window( max_x , max_y); 

    for(;;)
    {
        
        GameObj.KeyPressHandler() ; //Handles key presses 

        if(GameObj.getGameMode()=="multi")
        {
            activity = server.checkClientActiviy() ;

            if(activity==1)
            {
                GameObj.setClientsList(server.handleActivity("dummy")) ; 
                GameObj.handleActivity(clients) ; 

           }
            
        }

        flushinp();
        clear() ;

        GameObj.moveAllSnakes() ; 

        // if(no_players=="multi"){  snk1.printScore("right") ; snk1.move_snake(snk1.getDirection());  }
        printSpeed(GameObj.allSnakes[0]) ; 
        GameObj.printFood() ;
        refresh() ;
        usleep(GameObj.getSpeed()) ;
    }
 
}






// int main(int argc , char *argv[])  
// {  
//     socketHandler server ; 
//     server.bindServer() ; 
//     server.setupClientDescriptors() ; 
//     server.startServer() ; 

//     int activity ;

//     while(true)  
//     {  
   
//         activity = server.checkClientActiviy() ;

//         if(activity==1)
//         {
//             server.handleActivity() ;             
//         }
        
//         cout<<"\nSleeping for 2 seconds : \n" ; 
//         sleep(2) ; 

//     }  
        
//     return 0;  
// }  
