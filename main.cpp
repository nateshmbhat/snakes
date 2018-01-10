#include<bits/stdc++.h>
#include<iostream>
#include<unistd.h>
#include<ncurses.h>

using namespace std ; 
#define delay 10000

int main(int argc , char * argv[]) 
{
    initscr() ;
    noecho() ; 
    curs_set(false) ;
    
    float x= 0 , y =0 ; 
    int max_x  = 0 , max_y = 0  ;

    int flag = 1 ,ch=97 ; //left = 0  ,   right = 1 ; 
    // noecho() ;
    cbreak() ; 
    nodelay(stdscr , 1) ;
    char prev_char = 'a' ; 

    while(1){
        clear() ;

        mvprintw(10 , 0 , "Character : %c    , Code : %d" , prev_char , prev_char) ;
        getmaxyx(stdscr , max_y , max_x) ; 
        
        mvprintw(y,x , "o" ) ;
        refresh() ; 

        
        if(flag)
            x++ ;
        else
            x--  ;
        if(flag && x>max_x) 
            flag = 0 ;
        else if(x==0)
            flag = 1 ;
        
        if((ch = getch())!=ERR)
        {
            prev_char = ch ; 
            flag^=1 ; 
        }
        mvprintw(10 , 0 , "Character : %c    , Code : %d" , prev_char , prev_char) ;
        usleep(delay) ; 
    }

   
    sleep(1) ; 
    endwin() ; 
}



