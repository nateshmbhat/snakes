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

    snake_part()
    {
        x = y = 0 ; 
    }

};


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
    initscr() ;
    noecho() ; 
    curs_set(false) ;
    
    float x= 0 , y =0 ; 
    int max_x  = 0 , max_y = 0  ;
    getmaxyx(stdscr , max_y , max_x) ; 
    cbreak() ; 
    nodelay(stdscr , 1) ;
   


    draw_border_window( max_x , max_y); 
    refresh() ;









    sleep(10) ; 
    endwin() ; 
}



