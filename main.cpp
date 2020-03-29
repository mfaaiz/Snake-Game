// including headers and library
#include <iostream> 
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <iomanip>

using namespace std;

// declaring functions
void menu();
void game();
void load();
void controls(char s[][22],int temp[][22],int eating);
void credits();
void table(char s[][22],int score);
bool checkFood(char s[][22],int fx,int fy);
void spawnFood(char s[][22],int & fx,int & fy);
bool gameover(char s[][22],int temp[][22]);
void endgame(int score);
void help();
void options();
void difficulty();
void pause();

int length; //several variables
char contr='n';
int chek[32][22];
double diff=0.5; //difficulty variable, how fast snake moves (default-normal(0.5 sec)

int main()
{
    int i;
    char sl; //selection variable
    for(i=0;i<9999;i++)
    {
        if(i>0)
        {
            cout<<""<<endl;
            cout<<"Do you want to play again ? [y/n]   ";
            cin>>sl;
            if(sl=='y')
                {system("cls"); //clearing screen
                goto start;}
            else goto quit;
        } // if
        start: //starting program
        { menu(); } //showing menu screen
        cin>>sl; //selections for playing,options and exit
        if(sl=='1')
        {
            system("cls"); //clears screen
            load();
            game(); //calling game function, starting game
            i=1;
        }
        else if(sl=='2')
        {
            options();
            system("cls");
            goto start;
            i=1;
        }
        else if(sl=='3') goto quit;
        else
        {
            system("cls");
            i=1;
            goto start;
        }
    } // for
    quit: //quits program
        {system("exit");}

    return 0;
} // main

void table(char s[][22],int score)  //function for showing table (screen in which snake moves
{
    int x=32;
    int y=22;
    for(int i=0;i<y;i++)
    {
      for(int j=0;j<x;j++)
      {
       if(j<(x-1)) cout<<s[j][i];
       if(j==(x-1)) cout<<s[j][i]<<endl;
      } // inner for
    } // outer for
    cout<<"\nYour score: "<<score;
} // table

void game()
{
    char s[32][22]; //a 2-dimensional array for storing all particles in the screen;
    int temp[32][22]; //an array for numbering snake parts and storing them
    int i, j, fx, fy, x, y, score=0; //declaring variables
    double k;
    int eating; //if snake eats food, this becomes 1, and snake gains length
    //preparations for game: setting table borders
    system("Color 5E");
    for(i=0;i<22;i++)
    {
      for(j=0;j<32;j++)
      {
       if(i==0 || i==21) s[j][i]='+';
       if(i>0 && i<21) s[j][i]=' ';
       if(j==0 || j==31) s[j][i]='+';
      } // inner for
    } // outer for
    spawnFood(s,fx,fy); //calling food spawn function
    for(y=1;y<21;y++)
    {
        for(x=1;x<31;x++)
        {
            temp[x][y]=0;   //setting all the temp values to 0,
        } // inner for      //to see where snake parts are, and where nothing (0);
    } // outer for
    s[17][10]='o'; //finally, sets primary snake position. o is head.
    s[16][10]='*';
    s[15][10]='*';

    temp[15][10]=3; //numbering snake parts
    temp[16][10]=2;
    temp[17][10]=1;

    system("cls");
    table(s,score); //calling table function to show a table
    length=3; //snake primary length

    while(true) //game starts
    {
        eating=0; //setting value to 0, because snake hasnt eaten anything
        Sleep(diff*1000); //delaying some time, how fast snake moves depends on difficulty
        if(kbhit()) //checking if some key is pressed
        {
         contr=getch(); //if pressed, getting info about that key. this is needed for snake controls
         if(contr=='0') pause(); //if 0, pauses game
         goto movemake; //starts making snake moves
        } //if
        else
        {goto movemake;} //if no key is pressed, snake just moves
        
     movemake:
      {
         for(x=1;x<31;x++) //a loop for checking where are snake parts
         {
          for(y=1;y<21;y++)
           {
           if(s[x][y]=='*')
           {chek[x][y]=1;} //this is needed for checking if snake hits its tail or borders
           else chek[x][y]=0;
           } // for
         } // outer for
          if(checkFood(s,fx,fy)==true) //if snake eats something, some info needs to be sent for snake to gain length
          {
           eating=1;
           score=score+1;
           controls(s,temp,eating); //calling snake control and movement function
           spawnFood(s,fx,fy);
          } // if
          else
          {controls(s,temp,eating);} //snake only moves
      
        if(gameover(s,temp)==true) goto gameEnd; //if snake has hit something, game terminates
        else
        {goto LoopEnd;}
      } // movemake

        LoopEnd: //when loop ends, screen clears, updated table and score shows
        {
         system("cls");
         table(s,score);
        } // loopEnd
  } //while
    gameEnd:
    {
      endgame(score); //calls endgame screen
      return; //returns to int main
    } // gameEnd
} // game function

void controls(char s[][22],int temp[][22],int eating) 
{
    int i,x,y;
    for(i=length;i>0;i--) //snake starts moving from tail to head
    {
     if(i==length) //when i equals to the number of snakes last part
     {
      for(x=1;x<31;x++)
      {
       for(y=1;y<21;y++)
       {
        if(temp[x][y]==i && s[x][y]=='*')
        {
         if(eating==0) //if snake doesnt eat anything, the last part just disappears
         {
          s[x][y]=' '; temp[x][y]=0;
          x=30; y=20;
         } // inner if
         else if(eating==1) //if snake eats food, it moves, but the last part stays there,
         {                  //giving snake 1 more part
          s[x][y]='*'; temp[x][y]=length+1;
          length++; //this new part number will be new length
          x=30;
          y=20;
         } // elseif
         } // outer if
         } // inner for
         } // outer for
       } // if
       else if(i>1 && i<length) // all other snake parts stays where they are, but
       {                        // their values (temp) will increase by 1
        for(x=1;x<31;x++)
        {
         for(y=1;y<21;y++)
         {
          if(temp[x][y]==i && s[x][y]=='*')
          {
           temp[x][y]=i+1;
           x=30;
           y=20;
          } // if
         } // inner for
        } // outer for
       } // elseif
       else if(i==1) // when i is equal to snake's head value.
       {
        for(x=1;x<31;x++)
        {
         for(y=1;y<21;y++)
         {
          if(temp[x][y]==i && s[x][y]=='o')
          {
           movecheck:
           if(contr!='8' && contr!='2' && contr!='4' && contr!='6') //if no control button is pressed,
           {                                                      //snake just moves in its direction forward
            s[x][y]='*'; temp[x][y]=i+1;
            if(temp[x-1][y]==i+2) //several situations
            {
             s[x+1][y]='o'; temp[x+1][y]=1;
             return; //when snake moves its head, loop ends, we return to void game
            } // if
            else if(temp[x+1][y]==i+2)
            {
             s[x-1][y]='o'; temp[x-1][y]=1;
             return;
            } // elseif
            else if(temp[x][y+1]==i+2)
            {
             s[x][y-1]='o'; temp[x][y-1]=1;
             return;
            } // elseif
            else if(temp[x][y-1]==i+2)
            {
              s[x][y+1]='o'; temp[x][y+1]=1;
              return;
            } // elseif
          } // if
         else if(contr=='8') //if 8 pressed, snake moves upwards
         {
          if(temp[x][y-1]==0) 
          {                   
           s[x][y]='*'; temp[x][y]=i+1;
           s[x][y-1]='o'; temp[x][y-1]=1;
           return;
          } // if
          else //if not, we return to checking if control button is pressed
          {    //the new contr value will not be control button
           contr='n';
           goto movecheck;
          } // else
         } // elseif
        else if(contr=='2')//if 2 pressed, snake moves downwards
        {
         if(temp[x][y+1]==0)
         {
          s[x][y]='*'; temp[x][y]=i+1;
          s[x][y+1]='o'; temp[x][y+1]=1;
          return;
         } // if
          else
           {
            contr='n';
            goto movecheck;
           } // else
        } // elseif
        else if(contr=='4')//if 4 pressed, snake moves left
        {
         if(temp[x-1][y]==0)
         {
          s[x][y]='*'; temp[x][y]=i+1;
          s[x-1][y]='o'; temp[x-1][y]=1;
          return;
         } // if
         else
         {
          contr='n';
          goto movecheck;
         } //else
        } // elseif
       else if(contr=='6')//if 8 pressed, snake moves right
       {
        if(temp[x+1][y]==0)
        {
         s[x][y]='*'; temp[x][y]=i+1;
         s[x+1][y]='o'; temp[x+1][y]=1;
         return;
        } // if
        else
        {
         contr='n';
         goto movecheck;
        } // else
       } // elseif
    } // if
   } // inner for
  } // outer for
  } // elseif
 } // outermost for
} // controls function

bool checkFood(char s[][22],int fx,int fy) //function for checking if snake has eaten the food
{
    int x,y,c=0;
    for(x=1;x<31;x++)
    {
     for(y=1;y<21;y++)
     {
      if(s[x][y]=='o' && x==fx && y==fy)
      {c=1;}
     } // inner for
    } // outer for
    if(c==1) return true;
    else return false;
} // checkFood

void spawnFood(char s[][22],int & fx,int & fy) //function for spawning new food
{
  fx=(rand()%30)+1; //generates random value
  fy=(rand()%20)+1;
  for(int j=1;j<31;j++)
  {
   for(int i=1;i<21;i++)
   {
    if(s[j][i]!='*' && s[j][i]!='o') //checks if there is something in generated value square
    {
		s[fx][fy]='#';
     return;
    } // if
   else //if there is something, generates again, and checks
   {
    fx=(rand()%30)+1;
    fy=(rand()%20)+1;
    i=1; j=1;
   } // else
   } // inner for
  } // outer for
} // spwanFood function

bool gameover(char s[][22],int temp[][22]) //checking if snake has hit its tail or borders,
{
   int x,y,c=0;
   for(x=0;x<32;x++)
   {
    for(y=0;y<22;y++)
     {
      if((x==0 || x==31 || y==0 || y==21) && s[x][y]=='o')
      {c=1;}
      if(chek[x][y]==1 && s[x][y]=='o')
      {c=1;}
     } // inner for
    } // outer for
   if(c==1) return true; //if yes, game ends
   if(c==0) return false;
} // gameover

void endgame(int score) 
{
    system("cls");
    cout<<" ------------------------------------------------------------------------- "<<endl;
    cout<<"|    *****      *     *       * ******       ****  *       ****** ****    |"<<endl;
    cout<<"|   *          * *    * *   * * *           *    *  *     * *     *   *   |"<<endl;
    cout<<"|   *  ****   *   *   *  * *  * *****       *    *   *   *  ****  ****    |"<<endl;
    cout<<"|   *  *  *  *******  *   *   * *           *    *    * *   *     * *     |"<<endl;
    cout<<"|    *****  *       * *       * ******       ****      *    ***** *   *   |"<<endl;
    cout<<" ------------------------------------------------------------------------- "<<endl;
    cout<<"\n\n                        Y O U R   S C O R E : \a"<<score<<endl<<endl<<endl<<endl;
} // endgame
void menu()
{
    cout<<""<<endl;
    system("Color F6");
    cout<<"\n\t\t _______________________________________________  "<<endl;
    cout<<"\t\t|                                               | "<<endl;
    cout<<"\t\t|    ******   *     *      *     *  *  *****    | "<<endl;
    cout<<"\t\t|   *         * *   *     * *    * *   *        | "<<endl;
    cout<<"\t\t|    ******   *  *  *    *   *   **    ****     | "<<endl;
    cout<<"\t\t|          *  *   * *   *******  * *   *        | "<<endl;
    cout<<"\t\t|    ******   *     *  *       * *  *  *****    | "<<endl;
    cout<<"\t\t|                                               | "<<endl;
    cout<<"\t\t|                                               | "<<endl;
    cout<<"\t\t|     *****      *      *       *   ******      |"<<endl;
    cout<<"\t\t|    *          * *     * *   * *   *           |"<<endl;
    cout<<"\t\t|    *  ****   *   *    *  * *  *   *****       |"<<endl;
    cout<<"\t\t|    *  *  *  *******   *   *   *   *           |"<<endl;
    cout<<"\t\t|     *****  *       *  *       *   ******      | "<<endl;
    cout<<"\t\t|                                               | "<<endl;    
    cout<<"\t\t -----------------------------------------------  "<<endl<<endl;
    cout<<"                     \t\t\" M E N U\"    \a"<<endl;
    cout<<"                  1: Start Game  \n"<<endl;
    cout<<"                  2: Options    \n"<<endl;
    cout<<"                  3: Exit     "<<endl;
	cout<<"Enter: ";
} // menu

void help()
{
    char sel;
    system("cls");
    while(true)
    {
    system("Color B2");
    cout<<endl<<endl<<endl;
    cout<<"\t\t ----------------------------------------------"<<endl;
    cout<<"\t\t|                                              |"<<endl;
    cout<<"\t\t|                     8 - up                   |"<<endl;
    cout<<"\t\t|                                              |"<<endl;
    cout<<"\t\t|            4 - left        6 - right         |"<<endl;
    cout<<"\t\t|                                              |"<<endl;
    cout<<"\t\t|                    2 - down                  |"<<endl;
    cout<<"\t\t|                                              |"<<endl;
    cout<<"\t\t|                    0 - Pause                 |"<<endl;
    cout<<"\t\t|                                              |"<<endl;
    cout<<" \t\t----------------------------------------------- "<<endl<<endl<<endl;
    cout<<"Go back? [y/n]  ";
    cin>>sel;
    if(sel=='y') return;
    else system("cls");
    }
} // help

void load()
{  int r,q, c=176;
   cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n";
   cout<<setw(43)<<"loading...\n\n";    
   for(r=1;r<=20;r++)
   {   
    for(q=0;q<=100000000;q++);//to display the character slowly
    {
    if (r==1)
    cout<<"\t\t\t  ";
    cout<<(char)c;
	} // inner for
   } // outer for
   cout<<"\a";
   getch();
} // load

void credits()
{
    char sel;
    system("cls");
    while(true)
    {
    system("Color B2");
    cout<<endl<<endl<<endl<<endl;
    cout<<"\t\t********************************************"<<endl;
    cout<<"\t\t*                                          *"<<endl;
    cout<<"\t\t*  1-Saud Khan"<<setw(30)<<"Roll No: 61  *"<<endl ;
    cout<<"\t\t*  2-Ayyan Jaffri"<<setw(27)<<"Roll No: 64  *"<<endl;
    cout<<"\t\t*  3-Zain-ul-abidin"<<setw(25)<<"Roll No: 69  *"<<endl;
    cout<<"\t\t*  4-Muhammad Faaiz"<<setw(25)<<"Roll No: 56  *"<<endl;    
    cout<<"\t\t*                                          *"<<endl;
    cout<<"\t\t********************************************"<<endl;
    cout<<"\n\nGo back? [y/n]  ";
    cin>>sel;
    if(sel=='y') return;
    else system("cls");
    } // while
} // credits

void options()
{
    char sel;
    system("cls");
    while(true)
    {
    system("cls");
    system("Color B2");
    cout<<""<<endl<<endl;
    cout<<"                          O P T I O N S:      "<<endl<<endl<<endl;
    cout<<"                       1: Choose Difficulty   "<<endl<<endl;
    cout<<"                       2: Controls            "<<endl<<endl;
    cout<<"                       3: Credits             "<<endl<<endl;
    cout<<"                       4: Go Back             "<<endl<<endl;
    cin>>sel;
    if(sel=='1') difficulty();
    else if(sel=='2') help();
    else if(sel=='3') credits();
    else if(sel=='4') return;
    else system("cls");
    } // while
} // options

void difficulty() //function for setting difficulty (how fast snake moves)
{
    char sel;
    system("cls");
    while(true)
    {
    cout<<""<<endl<<endl;
    system("Color B2");
    cout<<"            C U R R E N T   D I F F I C U L T Y:"<<endl<<endl;
    if(diff==1) cout<<"                      A M A T E U R"<<endl<<endl;
    else if(diff==0.5) cout<<"                      S E M I - P R O"<<endl<<endl;
    else if(diff==0.2) cout<<"                   P R O F E S S I O N A L"<<endl<<endl;
    else if(diff==0.1) cout<<"                    W O R L D C L A S S"<<endl<<endl;
    else if(diff==0.03) cout<<"                    L E G E N D A R Y"<<endl<<endl;
    cout<<"            C H O O S E   D I F F I C U L T Y :"<<endl<<endl;
    cout<<"                       1: Amateur        "<<endl<<endl;
    cout<<"                       2: Semi-Pro     "<<endl<<endl;
    cout<<"                       3: Professional        "<<endl<<endl;
    cout<<"                       4: World Class  "<<endl<<endl;
    cout<<"                       5: Legendary  "<<endl<<endl<<endl<<endl;
    cout<<"                       6: Go Back     "<<endl<<endl;
    cin>>sel;
    if(sel=='1')  //there are 5 ready difficulties
        
    {
        diff=1;
        system("cls");
    }
    else if(sel=='2')
    {
        diff=0.5;
        system("cls");
    }
    else if(sel=='3')
    {
        diff=0.2;
        system("cls");
    }
    else if(sel=='4')
    {
        diff=0.1;
        system("cls");
    }
    else if(sel=='5')
    {
        diff=0.03;
        system("cls");
    }
    else if(sel=='6') return;
    else system("cls");
    }
} //difficulty

void pause()
{
    char c;
    cout<<""<<endl<<endl;
    cout<<"Game paused. Do you want to continue? [y/n]  \a";
    ooo:
    cin>>c;
    if(c=='y') return;
    else goto ooo;
} //pause game

/* =============================================== The End ================================================*/