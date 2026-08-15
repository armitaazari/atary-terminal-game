//Armita-Azarpeyvand-99442105

#include <ncurses.h>		
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>



#define width 25
#define height 19

struct player
{ 
    char firstname [20];
    char lastname [20];
    char username [20];
};

int b=1,score=0;

void init_screen(char screen[width][height]);
void refresh_screen(char screen [width][height]);
void draw_airplane(char screen[width][height], int x);
void delete_airplane(char screen[width][height], int x);
int  update_screen(char screen[width][height]);
void update_shoots(char screen[width][height]);
void update_barriers(char screen[width][height]);
void insert_new_barrier(char screen[width][height]);
int  check_for_explode(char screen[width][height]);
void draw_bax(char screen [width][height]);
void login(struct player *p);
void update_score();
void show_player(struct player pl);
void apply_key(char key, char screen[width][height], int *ax);
void init(char screen [width][height], struct player *pl);
int  kbhit(void);


int main()
{
    char screen[width+1][height];
    int  airplane_x=(width-5)/2;
    long int sleep_time=500000;
    char key;
    struct player p;

    init(screen,&p);
    while(1)
    {
        //if player get more than 5 score the game excutes faster
        if ((score) && !(score % 5) && (sleep_time > 300000))           
            sleep_time = sleep_time - 50000;

        usleep(sleep_time);
        if (kbhit())
        {
            key=getchar();
            apply_key(key,screen,&airplane_x);
        }
        if (!update_screen(screen))
        {
            refresh_screen(screen);
            show_player(p);
            refresh();
        }
        else
        {
            key=getch();
            while ((key!='q') && (key!='Q'))
                key=getch();
            break;
        }
 
    }
    endwin();    
    return(0);
}

void init(char screen[width][height], struct player *pl)
{
    char key;
    initscr();
    start_color();
    curs_set(0);
    init_screen(screen);
    login(pl);
    refresh_screen(screen);
    update_score();
    show_player(*pl);
    noecho();

    while ((key=getch()!='s'));
}

//this function apply user comand
void apply_key(char key, char screen[width][height], int *ax)
{
    switch(key)
    { 
        case 'm':                           //key m moves airplane right side
        case 'M':
            if(*ax < width-9)
            {
                delete_airplane(screen,*ax);
                (*ax)++;
                draw_airplane(screen,*ax);      
            }   
            break;

        case 'b':                           //key b moves airplane left side
        case 'B':
            if(*ax > width-23)
            {
                delete_airplane(screen,*ax);
                (*ax)--;
                draw_airplane(screen,*ax);
            }
            break;

        case ' ':                         //key space shot the shoots
            screen[*ax][height-5]='s';
            break;
        
        case 'p':                         //key p pause game  
        case 'P':
            key=getch();
            while ((key!='p') && (key!='P'))
                key=getch();

    }
}

//this function draw the main game frame
void init_screen(char screen[width][height])
{
    int i,j;
    
    for (i=0;i<width;i++)
        for (j=0;j<height;j++)
            screen[i][j]=' ';
 
    for (j=0;j<height;j++)
    {
        screen[0][j]='|';    
        screen[width][j]='|';    
        screen[width-7][j]='|';    
    }

    draw_airplane(screen, (width-5)/2);
}

//this function draw airplane whit airplane_x
void draw_airplane(char screen [width][height], int x)
{
    screen[x][height-4]='a';
    screen[x-1][height-3]='a';screen[x][height-3]='a';screen[x+1][height-3]='a';
    screen[x][height-2]='a';
    screen[x-1][height-1]='a';screen[x+1][height-1]='a';
}

//this function delete airplane whith airplane_x
void delete_airplane(char screen[width][height], int x)
{
    screen[x][height-4]=' ';
    screen[x-1][height-3]=' ';screen[x][height-3]=' ';screen[x+1][height-3]=' ';
    screen[x][height-2]=' ';
    screen[x-1][height-1]=' ';screen[x+1][height-1]=' ';

}

/*this function show the main game box
  init_pair(the number of color pair,foreground color,background color)*/  
void refresh_screen(char screen [width][height])
{  
    int i,j;
    for (j=0;j<height;j++)
        for (i=0;i<width;i++)
        {
            switch (screen[i][j])
            {
                case 'a':                                                 //case a show's airplane
                    init_pair(1,COLOR_YELLOW,COLOR_YELLOW);                   
                    attron(COLOR_PAIR(1));  
                    move(2*j,2*i); addch(ACS_DIAMOND);addch(ACS_DIAMOND);
                    move(2*j+1,2*i); addch(ACS_DIAMOND);addch(ACS_DIAMOND);
                    attroff(COLOR_PAIR(1));
                    break;
                case 'b':                                                //case b show's barriers
                    init_pair(2,COLOR_BLUE,COLOR_BLUE);         
                    attron(COLOR_PAIR(2));  
                    move(2*j,2*i); addch(ACS_DIAMOND);addch(ACS_DIAMOND);
                    move(2*j+1,2*i); addch(ACS_DIAMOND);addch(ACS_DIAMOND);
                    attroff(COLOR_PAIR(2));
                    break;
                case 's':                                                //case s show's shoots   
                    init_pair(3,COLOR_RED,COLOR_GREEN);
                    attron(COLOR_PAIR(3));               
                    move(2*j,2*i); printw("/");printw("\\");
                    move(2*j+1,2*i); printw("/");printw("\\");
                    attroff(COLOR_PAIR(3));
                    break;
                case ' ':                                                //this case show's background                    
                    if (i<width-7)
                    {
                        init_pair(4,COLOR_GREEN,COLOR_GREEN);
                        attron(COLOR_PAIR(4));
                        move(2*j,2*i); printw("  ");
                        move(2*j+1,2*i); printw("  ");
                        attroff(COLOR_PAIR(4));
                    }
                    else
                    {
                        init_pair(10,COLOR_BLUE,COLOR_BLUE);
                        attron(COLOR_PAIR(10));
                        move(2*j,2*i); printw("  ");
                        move(2*j+1,2*i); printw("  ");
                        attroff(COLOR_PAIR(10));
                    }
                    break;
                case '|':
                    init_pair(5,COLOR_WHITE,COLOR_WHITE);
                    attron(COLOR_PAIR(5));
                    move(2*j,2*i); printw("  ");
                    move(2*j+1,2*i); printw("  ");
                    attroff(COLOR_PAIR(5));
                    break;   
                default:
                    move(j,i); printw("Not Valid");
            }
        }
    update_score();
    refresh();
 }
//this function check explode and updates shoots and barriers
int  update_screen(char screen[width][height])
{ 
    int i,j;
    int explode;


    explode = check_for_explode(screen);
    if (!explode)
    {
        update_shoots(screen);
        update_barriers(screen);
        insert_new_barrier(screen);
    } else
    {   
        init_pair(6,COLOR_RED,COLOR_BLACK);
        attron(COLOR_PAIR(6));
        attron(A_BLINK);
        move(height-3,width-9);printw(" Game over");
        move(height-2,width-10);printw("high score:%d",score);
        attroff(A_BLINK);
        attroff(COLOR_PAIR(6));
        getch();
        return -1;
    }
    return 0;
}
//this function moves shoots up and make shoots delete barriers when shoots hit them    

void update_shoots(char screen[width][height])
{

        int i,j,p;

        for (j=1;j<=height-5;j++)
            for (i=1;i<=width;i++)
            {
                if (screen[i][j]=='s')
                {
                    if (screen[i][j-1]=='b')
                    {
                        score++;
                        screen[i][j-1]=' ';
                        screen[i][j]=' ';
                        p=i+1;
                        while(screen[p][j-1]=='b')
                        {
                            screen[p][j-1]=' ';
                            score++;
                            p++;
                        }
                        p=i-1;
                        while(screen[p][j-1]=='b')
                        {
                            screen[p][j-1]=' ';
                            score++;
                            p--;
                        }
                    }
                    else
                    {
                        screen[i][j-1]='s';
                        screen[i][j]=' ';
                    }
                }  
            } 
}  

//this function moves barriers down
void update_barriers(char screen[width][height])
{
    int i,j;

    for (j=height-1;j>=0;j--)
        for (i=1;i<width-6;i++)
            if (screen[i][j]=='b')
            {
                screen[i][j+1]='b';
                screen[i][j]=' ';
            }
}

//this function make random barrieres
void insert_new_barrier(char screen[width][height])
{
    int i;
    time_t t;
    int barrier[width-9];

    srand((unsigned) time(&t));

    for(i=1;i<width-8;i++)
    {
        screen[i][0]=' ';
        if (!b)
            if (!(rand() % 10))
                screen[i][0]='b';
    }

    b++;
    if (b==4) 
        b=0;
}
//this function check airplane and barrier have Collision or not

int check_for_explode(char screen[width][height])
{
    int explod=0;
    int i,j;

    for(j=height-1; j>=height-6; j--)
    {
        for(i=1; i<width-6; i++)
            if(screen[i][j]=='b' && screen[i][j+1]=='a')
            {
                explod=1;
                break;
            }
    }
    return explod;
} 

//this function draw login page and get player's information
void login(struct player *p)
{     
    WINDOW *win;                                                                          
    win=newwin(20,40,3,3);
    refresh();
    box(win,0,0);
    init_pair(6,COLOR_WHITE,COLOR_MAGENTA);
    attron(COLOR_PAIR(6));
    attron(WA_BLINK);
    move(4,13);printw("***LET'S PLAY ATARY***");
    wrefresh(win);
    attroff(COLOR_PAIR(6));
    attroff(WA_BLINK);

    attron(A_BOLD);
    move(7,5);printw("whats your firstname?");   
    move(7,27);scanw("%s",(*p).firstname);
    move(9,5);printw("whats your lastname?");
    move(9,27);scanw("%s",(*p).lastname);
    move(11,5);printw("enter your username?");
    move(11,27);scanw("%s",(*p).username);
    attroff(A_BOLD);
}
//this function show player's score
void update_score()
{
    init_pair(9,COLOR_BLACK,COLOR_WHITE);
    attron(COLOR_PAIR(9));
    mvprintw(2,41,"%-4d",score);
    attroff(COLOR_PAIR(9));
}

//this function print player's information and help show information during the game
void show_player(struct player pl)
{
    init_pair(8,COLOR_BLUE,COLOR_WHITE);
    attron(A_BOLD);
    attron(COLOR_PAIR(8));
    move(4,39);printw("%-10s", pl.firstname);
    move(5,39);printw("%-10s", pl.lastname);
    move(6,39);printw("%-10s", pl.username);
    attroff(COLOR_PAIR(8));
    attron(A_BOLD);
    
}

//this function show's that any key peressed or not
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
}