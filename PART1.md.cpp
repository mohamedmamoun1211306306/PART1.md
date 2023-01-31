#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <string>
#include <ctime>
#include <climits>
#include <string>
#include <sstream>
using namespace std;

int BOARDROWS = 5, BOARDCOLUMNS = 9;
int ZOMBIESNUM = 1, NOWTURN = 0;

struct Object
{
    char type;
    int x, y;
};
struct Alien
{
    int life;
    int attack;
    char direction = '^';
    int x, y;
};

struct Zombie
{
    char id;
    int life;
    int attack;
    int range;
    int x, y;
};

void setGameSettings();
void setBoardSettings();
vector<vector<char>> getBoard();
void makeBoard(vector<vector<char>> &board);
char getRandomObject();
void encPod(Alien &alien, vector<Zombie> &zombies);
void encHealth(Alien &alien);
void encArrow(char nextSpot, Alien &alien);
void displayBoard(vector<vector<char>> &board);
void initializeBoard(Alien &alien, vector<Zombie> &zombies, vector<vector<char>> &board);
void alienGame(Alien &alien, vector<vector<char>> &board, vector<Zombie> &zombies);
bool inputCommand(Alien &alien,vector<Zombie> &zombies,vector<vector<char>> &board);
void checkNextStops(char nextLocation, Alien &alien, vector<Zombie> &zombies);
void displayStatus(Alien &alien, vector<Zombie> &zombies);
bool checkZombieCommingMove(int newX, int newY, vector<vector<char>> &board);

// set the boarder with the rows and cols from user input
void setBoardSettings()
{
    system("cls");
    int tempRow = 1, tempCol = 1;
    do
    {
        if (tempRow % 2 == 0 || tempCol % 2 == 0)
        {
            cout << "Invalid input. to set the board Rows and columns must be odd numbers." << endl;
        }
        cout << "Enter rows => ";
        cin >> tempRow;
        cout << "Enter columns => ";
        cin >> tempCol;
    } while (tempRow % 2 == 0 || tempCol % 2 == 0);
    do
    {
        if (ZOMBIESNUM < 0)
        {
            cout << "Invalid input. Number of zombies must be a positive integer." << endl;
        }
        cout << "Enter number of zombies => ";
        cin >> ZOMBIESNUM;
    } while (ZOMBIESNUM <= 0);
    BOARDCOLUMNS = (tempCol * 2) + 1;
    BOARDROWS = (tempRow * 2) + 1;
   system("pause");
}
// set the Game settings in the start of the game
void setGameSettings()
{
    char choice;
    cout << "Default Game Settings"
         << "\n------------------------"
         << "\nBoard Rows     :" << BOARDROWS
         << "\nBoard Columns  :" << BOARDCOLUMNS
         << "\nZombie Count   :" << ZOMBIESNUM
         << endl;

    cout << "Do u which to change the game settings (y/n)? ";
    cin >> choice;
    if (tolower(choice) == 'y')
    {
        setBoardSettings();
    }
    else
    {
        BOARDCOLUMNS = (BOARDCOLUMNS * 2) + 1;
        BOARDROWS = (BOARDROWS * 2) + 1;
    }
}

vector<vector<char>> getBoard()
{
    vector<vector<char>> temp(BOARDROWS, vector<char>(BOARDCOLUMNS));
    return temp;
}

void makeBoard(vector<vector<char>> &board)
{
    
    for (int x = 0; x < BOARDCOLUMNS; x++)
    {
        
        for ( int y = 0 ; y < BOARDROWS; y++)
        {
            board[x][y] = ' ';
            if (x == (BOARDCOLUMNS+1)/2 && y == (BOARDROWS+1)/2 )
            {
                board[x][y] = 'A';
            }
            
        }; 
    }

}

char getRandomObject()
{
    vector<char> nonTrailObjects = {'^', 'v', '<', '>', 'h', 'p', 'r', ' '};
    int randomIndex = rand() % nonTrailObjects.size();
    return nonTrailObjects[randomIndex];
}

void initializeFrame(vector<vector<char>> &board)
{
    for (int i = 0; i < BOARDROWS; i++)
    {
        for (int j = 0; j < BOARDCOLUMNS; j++)
        {
            if (i % 2 == 0)
            {
                if (j % 2 == 0)
                {
                    board[i][j] = '+';
                }
                else
                {
                    board[i][j] = '-';
                }
            }
            else
            {
                if (j % 2 == 0)
                {
                    board[i][j] = '|';
                }
                else
                {
                    board[i][j] = ' ';
                }
            }
        }
    }
}

void placeAlien(Alien &alien, vector<vector<char>> &board)
{
    // Place the alien at the center of the board
    alien.x = BOARDROWS / 2;
    alien.y = BOARDCOLUMNS / 2;
    board[alien.x][alien.y] = 'A';
}

void placeObjects(vector<Object> &Objects, vector<vector<char>> &board)
{
    srand(time(NULL));
    int numberOfObjects = rand() % (BOARDROWS * BOARDCOLUMNS - 1);
    for (int i = 0; i < numberOfObjects; i++)
    {
        Object obj;
        obj.type = getRandomObject();
        obj.x = (rand() % (BOARDROWS - 1)) | 1;
        obj.y = (rand() % (BOARDCOLUMNS - 1)) | 1;

        for (int j = 0; j < Objects.size(); j++)
        {
            while (obj.x == Objects[j].x && obj.y == Objects[j].y)
            {
                obj.x = (rand() % (BOARDROWS - 1)) | 1;
                obj.y = (rand() % (BOARDCOLUMNS - 1)) | 1;
            }
        }

        board[obj.x][obj.y] = obj.type;
        Objects.push_back(obj);
    }
}

void placeZombies(vector<Zombie> &zombies, vector<vector<char>> &board)
{
    for (int i = 0; i < zombies.size(); i++)
    {
        zombies[i].x = (rand() % (BOARDROWS - 1)) | 1;
        zombies[i].y = (rand() % (BOARDCOLUMNS - 1)) | 1;
        // check if the location is not occupied by other zombies
        for (int j = 0; j < i; j++)
        {
            while (zombies[i].x == zombies[j].x && zombies[i].y == zombies[j].y)
            {
                zombies[i].x = (rand() % (BOARDROWS - 1)) | 1;
                zombies[i].y = (rand() % (BOARDCOLUMNS - 1)) | 1;
            }
        }
        board[zombies[i].x][zombies[i].y] = zombies[i].id;
    }
}

void initializeBoard(Alien &alien, vector<Zombie> &zombies, vector<vector<char>> &board)
{
    initializeFrame(board);
    srand(time(NULL));
    vector<Object> Objects;

    placeObjects(Objects, board);
    placeZombies(zombies, board);
    placeAlien(alien, board);



}

void initializeBoard(Alien &alien, vector<Zombie> &zombies, vector<Object> &Objects, vector<vector<char>> &board)
{
    makeBoard(board);
    placeObjects(Objects, board);
    placeZombies(zombies, board);
    placeAlien(alien, board);

}

void displayBoard(vector<vector<char>> &board)
{
    // print the board
    for (int i = 0; i < BOARDROWS; i++)
    {
        for (int j = 0; j < BOARDCOLUMNS; j++)
        {
            cout << board[i][j];
        }
        cout << endl;
    }
}

void initializePlayers(Alien &alien, vector<Zombie> &zombies)
{
    srand(time(NULL));
    for (int i = 0; i < ZOMBIESNUM ; i++)
    {
        Zombie newZombie;
        newZombie.id = i + '1';
        newZombie.life = rand() % 80 + 20;
        newZombie.attack = rand() % 20 + 10;
        newZombie.range = rand() % (min(BOARDROWS, BOARDCOLUMNS) / 2) + 1;
        newZombie.x = rand() % BOARDROWS;
        newZombie.y = rand() % BOARDCOLUMNS;
        zombies.push_back(newZombie);
    }
    alien.life = 100;
    alien.attack = rand() % alien.life + 1;
}

void checkNextStops(char nextSpot, Alien &alien, vector<Zombie> &zombies)
{
    switch (nextSpot)
    {
    case '^':
    case 'v':
    case '<':
    case '>':
        encArrow(nextSpot,alien);
        break;
    case 'h':
        // add 20 life to alien
        encHealth(alien);
        break;
    case 'p':
        // inflict 10 damage to zombies
        encPod(alien,zombies);
        break;
    case ' ':
        cout << "this spot is empty" << endl;
        break;
    }
}

void encArrow(char nextSpot, Alien &alien){
    alien.attack += 20;
    alien.direction = nextSpot;
    cout << "Alein change direction to " << nextSpot << endl;
}

void encHealth(Alien &alien){
    if(alien.life <= 100){
        alien.life += 20;
    }
    cout << "\nAlein encounter a health pack" 
         << "\nAlein's lifes increased by 20 "
         << endl;
}

void encPod(Alien &alien, vector<Zombie> &zombies){
    
}

void alienGame(Alien &alien, vector<vector<char>> &board, vector<Zombie> &zombies)
{
    bool moveable = inputCommand(alien ,zombies,board);
    if(moveable){
            while (true)
        {
            int nextX = alien.x;
            int nextY = alien.y;
            // calculate the next location based on the current direction
            switch (alien.direction)
            {
            case '^':
                nextX = alien.x - 2;
                break;
            case 'v':
                nextX = alien.x + 2;
                break;
            case '<':
                nextY = alien.y - 2;
                break;
            case '>':
                nextY = alien.y + 2;
                break;
            }
            if (nextX == -1 || nextX >= BOARDROWS - 1 || nextY == -1 || nextY >= BOARDCOLUMNS - 1)
            {
                cout << "Alien hit a board border and its turn ended." << endl;
                return;
            }
            else
            {
                char nextLocation = board[nextX][nextY];
                if (nextLocation == 'r')
                {
                    cout << "Alien hits a rock can't move" << endl;
                    while (board[nextX][nextY] == 'r')
                    {
                        board[nextX][nextY] = getRandomObject();
                    }
                    break;
                }
                else
                {
                    checkNextStops(nextLocation, alien, zombies);
                    board[nextX][nextY] = 'A';
                    board[alien.x][alien.y] = '.';
                    alien.x = nextX;
                    alien.y = nextY;
                    system("pause");
                    system("CLS");
                    displayBoard(board);
                    displayStatus(alien, zombies);
                }
            }
        }
    }
    
}

bool inputCommand(Alien &alien, vector<Zombie> &zombies,vector<vector<char>> &board)
{
    bool moveable = false;
    cout << "\n\nEnter the command or help to show the menu " << endl;
    string command;
    cin >> command;
    if (command == "up")
    {   
        alien.direction = '^';
        moveable = true;
    }
    else if(command == "down")
    {
        alien.direction = 'v';
        moveable = true;
    }
        
    else if(command == "left")
    {
        alien.direction = '<';
        moveable = true;
    }
    else if(command == "right")
    {
        alien.direction = '>';
        moveable = true;
    }
    else if(command == "arrow")
    {    
    }
    else if(command =="help")
    {
        cout << "\nCommands:"
        <<"\n up       to move up"
        <<"\ndown      to move down"
        <<"\nleft      to move left"
        <<"\nright     to move right"
        <<"\narrow     to chang arrow direction "
        <<"\nhelp      to see all the comands"
        <<"\nsave      to save your curent game"
        <<"\nload      to load an old game"
        <<"\nquit      to quit the game" << endl;
    }

    else if(command == "save")
    {    
        
    }
    else if(command == "load")
    {
       
    }
    else if(command == "quit")
    {
        cout << "Are you sure you want to quit? y/n"<<endl;
        char confirm;
        cin >> confirm;
        if (tolower(confirm) == 'y')
        {
            exit(0);
        }
    }
    else
    {
        cout << "Invalid command. Type 'help' for a list of commands." << endl;
    }

return moveable ;
}


void displayStatus(Alien &alien, vector<Zombie> &zombies)
{

    string select1= "    " ;
    if (0 == NOWTURN)
    {
        select1 = "->  ";
    }
    cout << select1 << "Alien   :"
         << "  Life : " << alien.life << "    Attack: " << alien.attack << endl;

    for (auto  i = 0; i < zombies.size(); i++)
    {
        string select= "    " ;
        
        if (i+1 == NOWTURN )
        {
            select = "->  ";
        }

        cout << select         << "Zombie " << i + 1 << ":"
             << "  Life : "    << zombies[i].life    
             << "    Attack: " << zombies[i].attack 
             << "     Range: " << zombies[i].range   <<endl;
        
    }
    cout << endl
         << endl;
}

void zombiesGame(Alien &alien, vector<vector<char>> &board, Zombie &zombie)
{

    int newX = zombie.x;
    int newY = zombie.y;
    char moves[4] = {'v', '^', '<', '>'};
    int moveIndex;
    string moveDirection;
    do
    {
        moveIndex = rand() % 4;
        char move = moves[moveIndex];
        switch (move)
        {
            case 'v':
                newX = zombie.x + 2;
                moveDirection = "Down";
                break;
            case '^':
                newX = zombie.x - 2;
                moveDirection = "Up";
                break;
            case '>':
                newY = zombie.y + 2;
                moveDirection = "Right";
                break;
            case '<':
                newY = zombie.y - 2;
                moveDirection = "Left";
                break;
        }
    } while (!(checkZombieCommingMove(newX, newY, board)));

    cout << "Zombie moves " << moveDirection << endl;
    // check if new coordinates are within board boundaries and not occupied by alien
    board[zombie.x][zombie.y] = ' ';
    zombie.x = newX;
    zombie.y = newY;
    board[zombie.x][zombie.y] = zombie.id;
}

bool checkZombieCommingMove(int newX, int newY, vector<vector<char>> &board)
{

    if (newX >= 0 && newX < BOARDROWS && newY >= 0 && newY < BOARDCOLUMNS && board[newX][newY] != 'A' && board[newX][newY] != 'Z')
    {
        return true;
    }
    else
    {
        return false;
    }
}


int main()
{
    Alien alien;
    vector<Zombie> zombies;
    vector<vector<char>> board;

    system("cls");
    setGameSettings();
    board = getBoard();
    initializePlayers(alien, zombies);
    initializeBoard(alien, zombies, board);
    displayBoard(board);

    displayStatus(alien, zombies);
    while (true)
    {
        NOWTURN = 0;
        alienGame(alien, board, zombies);
        
        for (auto i = 0; i < zombies.size(); i++)
        {
            NOWTURN++;
            zombiesGame(alien, board, zombies[i]);
            system("pause");
            system("CLS");
            displayBoard(board);
            displayStatus(alien, zombies);
        }
    }
}
// *********************************************************
// Course: TCP1101 PROGRAMMING FUNDAMENTALS
// Year: Trimester 1, 2022/23 (T2215)
// Lab: TxxL
// Names: mohamed mamoun mohamed|Alharthi, Hamzah Hamed S 
// IDs: 1211306306 |1211305485
// Emails: mohamed1161901324@gmail.com| 1211305485@student.mmu.edu.my
// Phones: 01161901324|01127880328
// *********************************************************