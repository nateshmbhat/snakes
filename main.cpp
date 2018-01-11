#include<bits/stdc++.h>
#include<iostream>
#include<unistd.h>
#include<ncurses.h>

using namespace std ; 
#define delay 10000

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

    public :
    snake(void)
    {
        ;
    }

    void draw_snake(void)
    {
        for( int i =0 ; i<parts.size() ; i++)
        {
        mvprintw(parts[i].y  , parts[i].x , "o") ; 
        }
    
    }

    void add_part(int x , int y ) //adds the part object taking the coordinates to the end of the part vector in snake
    {
        snake_part obj(x , y) ; 
        parts.push_back(obj) ; 
    }

    void move_snake(string direction)
    {
        
        parts.erase(parts.begin())  ;
        snake_part last_part = parts.at(parts.size()-1) ;

        if(direction=="right")
        {
            add_part(last_part.x+1  , last_part.y) ;
        }

        if(direction =="left") 
        {
            add_part(last_part.x-1 , last_part.y) ; 
        }

        if(direction =="up")
        {
            add_part(last_part.x , last_part.y-1) ;
        }
        if(direction=="down")
        {
            add_part(last_part.x , last_part.y+1) ;
        }

        draw_snake() ;
        refresh() ; 
    }

   
} ; 




void draw_border_window(int  , int) ; 
void draw_food(void) ; 


void draw_border_window( int max_x , int max_y)
{
    for(int i =0 , j = 1 ; (i<max_x || j<max_y ) ; )
    {
        if(i<max_x)
            mvprintw(0,i++,"--") ;

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


void draw_food(void)
{
    ;
}


int max_x  = 0 , max_y = 0  ;  //Make max_x and max_y as global since the values are used by many methods 


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
   
    //Initialize the snake object
    snake snk ;

   //add the first 3 dots to the snake 
    snk.add_part(center_x , center_y) ; 
    snk.add_part(center_x+1 , center_y) ; 
    snk.add_part(center_x+2 , center_y) ; 
    
    draw_border_window( max_x , max_y); 
    snk.draw_snake() ;

    cout<<KEY_DOWN <<" "<< KEY_UP ;
    sleep(100) ;


    for(int j = 0 ; j<100000 ; j++)
    {
        if((ch = getch())!=ERR)
        {
            if(ch==27) //ANSI escape sequence for arrow keys starts with 27 91 and 65 66 67 68 for up down right left
            {
                getch() ; // clear and reject 91 from buffer
                ch = getch() ;//Now store the actual value of arrow key pressed  
            }
            switch(ch)
            {
                case KEY_DOWN : snk.move_snake("down") ; break ; 
                case KEY_UP : snk.move_snake("up") ; break ; 
                case KEY_RIGHT : snk.move_snake("right") ; break ; 
                case KEY_LEFT : snk.move_snake("left") ; break ; 
            }
        }
        clear() ;
        draw_border_window(max_x , max_y) ; 
        usleep(50000) ;
    }


    refresh() ;


    sleep(10) ; 
    endwin() ; 
}



