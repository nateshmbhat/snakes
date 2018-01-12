#include<bits/stdc++.h>
#include<iostream>
#include<unistd.h>
#include<ncurses.h>

using namespace std ; 

typedef struct food
{
    int x,y ;
}food ; 


//global variables for the program 
int max_x  = 0 , max_y = 0  ;  //Make max_x and max_y as global since the values are used by many methods 
food global_food  = {0 , 0 }; 


class player
{
    private:
    int score ;

    public:
    int getScore(void){return score ; }
    
    void printScore(string pos = "left")
    {
        mvprintw(0 , 0 , "Score : %d" , score) ;  
    }

} ;


class snake_part
{
    public :
    int x , y ; 

    snake_part(int  xx, int yy )
    {
        x = xx ; y = yy ; 
    }
};


void generateFood()
{
    int x = random()%max_x , y = random()%max_y  ; 
    if(!x)x = 2 ; 
    if(!y) y = 2 ; 
    mvprintw(y, x ,"#") ;   
    global_food.x = x ; 
    global_food.y = y ;
    flushinp();
}

void printFood(string status="old")
{
    if(status=="new")
        generateFood() ; 
    
    if(!global_food.x && !global_food.y)
        generateFood() ; 
    mvprintw(global_food.y, global_food.x ,"#") ;   
}


class snake
{
    private :
    vector <snake_part> parts ;
    string snakeDirection  ;
    unsigned long int speed ; 
    int score ;
    char keyUp , keyDown , keyRight , keyLeft ; 

    public :
    snake(char up , char down , char right , char left)
    {
        keyUp = up , keyDown = down , keyRight = right , keyLeft = left ; 
        speed = 40000 ;
        score = 0 ; 
        snakeDirection = "right" ; 
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

        if(getHeadX()==global_food.x && getHeadY() == global_food.y)
        {
            add_part(global_food.x , global_food.y ) ;
            setScore(getScore()+1) ; 
            printFood("new") ;
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
                    mvprintw(max_y/2 , max_x/2 -6 , "GAME IS OVER ! ") ;
                    refresh() ; 
                    sleep(5) ;
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

    
    unsigned long int setSpeed(unsigned long int s){speed = s>90000?90000:s<10000?10000:s ; return speed ; }

    int getHeadX(void){return parts.at(parts.size()-1).x;}
    int getHeadY(void){return parts.at(parts.size()-1).y;}
    string getDirection(void){return snakeDirection; }
    long int getSpeed(void){return speed; }


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
    mvprintw(0 , max_x-20 , "Speed= %lu" ,snk.getSpeed()) ; 
    refresh() ;
}


string ask_no_players(string player="single")
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
                player="single" ; 
            }

            else if(ch=='B')
            {
                player="multi" ; 
            }
        }

        else if(ch==10)
        {
            return player ; 
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
}



int main(int argc , char * argv[]) 
{
    
    initscr() ; //Init screen 
    noecho() ; // Dont show any pressed char  
    curs_set(false) ; // Don't show the cursor 
    srand(time(NULL)) ;
        


    float x= 0 , y =0 ; 
    getmaxyx(stdscr , max_y , max_x) ; 
    int center_x = max_x/2  , center_y = max_y/2 ; 
    char ch ; 

    
    cbreak() ; //Dont wait for enter to be pressed when using getch 
    nodelay(stdscr , 1) ;  //Use non blocking input for getch which just returns ERR if there is no input (ERR=-1)

    
    //Asks the number of players who want to play  (single or multiplayer (with 2 players )) ; 
    string no_players = ask_no_players() ;
    

   
    //Initialize the snake object
    snake snk('A' , 'B', 'C', 'D') , snk1('w','s', 'd' , 'a') ;

   //add the first 3 dots to the snake 
    snk.add_part(center_x+5 , center_y) ; 
    snk.add_part(center_x+6 , center_y) ; 
    snk.add_part(center_x+7 , center_y) ;

    snk1.add_part(center_x-7 , center_y) ; 
    snk1.add_part(center_x-6 , center_y) ; 
    snk1.add_part(center_x-5 , center_y) ;

    
    // draw_border_window( max_x , max_y); 
    snk.draw_snake() ;
    if(no_players=="multi")
        snk1.draw_snake() ; 


    for(;;)
    {

        if((ch = getch())!=ERR)
        {
            if(ch==27)
            {
                getch() ; // clear and reject 91 from buffer
                ch = getch() ;//Now store the actual value of arrow key pressed               getch() ; 
                snk.handleMovementKeyPress(ch) ; 
            }

            else if (no_players=="multi"){
                snk1.handleMovementKeyPress(ch) ; 
            }


            //INcrease the snake speed
            if(ch==45)
            {
                snk.setSpeed(snk.getSpeed()+3000) ;
            }
            //Decrease the snake speed
            if(ch==43)
                snk.setSpeed(snk.getSpeed()-3000); 
            
        }



        clear() ;
        // snk.draw_snake() ;
        snk.move_snake(snk.getDirection())  ;
        if(no_players=="multi"){  snk1.printScore("right") ; snk1.move_snake(snk1.getDirection());  }
        snk.printScore() ;
        // draw_border_window(max_x , max_y) ; 
        printSpeed(snk) ; 
        printFood() ;
        refresh() ;
        usleep(snk.getSpeed()) ;
    }



    refresh() ;


    sleep(10) ; 
    endwin() ; 
}



