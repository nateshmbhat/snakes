#include<bits/stdc++.h>
#include<iostream>
#include<unistd.h>
#include<ncurses.h>

using namespace std ; 
#define delay 10000

void draw_border_window(int  , int) ; 
void draw_snake(vector <snake_part>) ; 

class snake_part
{
    public :
    int x , y ; 

    snake_part(int  xx, int yy )
    {
        x = xx ; y = yy ; 
    }

};


void draw_snake(vector <snake_part>)
{

}


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



int main(int argc , char * argv[]) 
{
    initscr() ; //Init screen 
    noecho() ; // Dont show any pressed char  
    curs_set(false) ; // Don't show the cursor 
    
    float x= 0 , y =0 ; 
    int max_x  = 0 , max_y = 0  ;
    getmaxyx(stdscr , max_y , max_x) ; 
    int center_x = max_x/2  , center_y = max_y/2 ; 

    cbreak() ; //Dont wait for enter to be pressed when using getch 
    nodelay(stdscr , 1) ;  //Use non blocking input for getch which just returns ERR if there is no input (ERR=-1)
   
   //Create the container and add the first 3 dots of the snake
    vector <snake_part> parts ; 
    snake_part p1(center_x , center_y)  , p2(center_x+1 ,center_y) , p3(center_x+2 , center_y) ; 

    parts.push_back(p1) ;
    parts.push_back(p2) ; 
    parts.push_back(p3) ; 


    draw_border_window( max_x , max_y); 
    draw_snake(parts) ; 
    



    refresh() ;









    sleep(10) ; 
    endwin() ; 
}



