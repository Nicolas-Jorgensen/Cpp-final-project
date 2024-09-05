//Nicolas Jorgensen
//          <A dungeon room 'maker' using a multi-linked list.
//           Entering commands allows you to create, name, connect
//           and traverse rooms. There is also a working save 
//            and load feature. The Room class was created by my instructor and we
//            weren't allowed to edit it. If we were, I would have included a lot
//            of the functions that ended up in main as member functions of the
//            Room class to keep things cleaner. Sorry for messy formatting.>

#include "Room.h"
#include <fstream>
#include <iostream>

using namespace std;

void menu();
void look(Room* current_room);
int findRoom(const vector<Room*>& allRooms, const Room* roomPtr);
Direction findReturnDir(Room* currentRoom, Room* otherRoom);
void exits(Room* current_room);
void add(Room* current, vector<Room*>& allRooms);
void connect(Room* current, vector<Room*> allRooms);
void rename(Room* current);
void saveGame(const string& filename, const vector<Room*>& allRooms);
void loadGame(const string filename, vector<Room*>& allRooms);
void description(Room* current);
Direction ch2Direct(char c);
bool alreadyConnected(Room* current, char dir);
bool never_eat_soggy_waffles(char n_s_e_w);
void jump(Room* &current, vector <Room*> rooms);
template <class T>
void print(T thing);

int main()
{
    // initialize
    Room* current_room = new Room();
    vector<Room*> allRooms;
    allRooms.clear();
    allRooms.push_back(current_room);

    char choice;

    //exits(current_room);
    // menu loop
    do
    {
        cout<<"Enter a command (? to list): "<<endl;
        cin>>choice;
        switch(choice)
        {
            case '?':
                menu(); break;
            case 'a':
            case 'A':
                add(current_room, allRooms); break;
            case 'n':
            case 'N':
                if (current_room->north()!= NULL)
                    current_room = current_room->north();
                else
                    cout<<"You cannot go that way"<<endl;
                break;
            case 'r':
            case 'R':
                rename(current_room); break;
            case 'd':
            case 'D':
                description(current_room); break;
            case 's':
            case 'S':
                if (current_room->south()!= NULL)
                    current_room = current_room->south();
                else
                    cout<<"You cannot go that way"<<endl;
                break;
            case 'c':
            case 'C':
                connect(current_room, allRooms); break;
            case 'e':
            case 'E':
                if (current_room->east()!= NULL)
                    current_room = current_room->east();
                else
                    cout<<"You cannot go that way"<<endl;
                break;
            case 'w':
            case 'W':
                if (current_room->west()!= NULL)
                    current_room = current_room->west();
                else
                    cout<<"You cannot go that way"<<endl;
                break;
            case 'l':
            case 'L':
                look(current_room); break;
            case 'x':
            case 'X':
                exits(current_room);break;
            case 'j':
            case 'J':
                jump(current_room, allRooms); break;
            case 'q':
            case 'Q':
                cout << "Quitting..." << endl;
                break;
            case 'u':
            case 'U':
                saveGame("file.dat",allRooms);
                break;
            case 'y':
            case 'Y':
                loadGame("file.dat", allRooms);
                current_room = allRooms.at(0);
                break;
            default:
                cout<<"invalid command"<<endl;
        }
    }while(choice != 'q');



    // cleanup before quitting

    return 0;
}


// Show the name and description of a room */
void look(Room* r)
{
    cout << *r;
}




void add(Room* current, vector<Room*>& allRooms)
{
    Room *tempDirection = nullptr;
    char n_s_e_w, entrance; 
    Direction ent_string;

    if(current == nullptr)
    {
        cout << "Room doesn't exist, try again.";
        return;
    }

    do
    {
        cout << "Connect new room in which direction? (N,S,E,W)\n";
        cin >> n_s_e_w;
    }while( !(never_eat_soggy_waffles(n_s_e_w)));

    do
    {
        cout << "Which exit of the new room leads to this room?\n";
        cin >> entrance;
    }while(!(never_eat_soggy_waffles(entrance)));       


    ent_string = ch2Direct(entrance);

    switch(n_s_e_w)
    {

        case 'n': 
        case 'N':
            if(current->north() == nullptr)
            {
                tempDirection = new Room("NewRoom","This room is just waiting for shape and form");
                current->connect(NORTH, tempDirection, ent_string);
                allRooms.push_back(tempDirection);
                cout << "Room connection ok\n";
                return;
            }
            break;
        case 's':
        case 'S':    
            if(current->south() == nullptr)
            {
                tempDirection = new Room("NewRoom","This room is just waiting for shape and form");
                current->connect(SOUTH, tempDirection, ent_string);
                allRooms.push_back(tempDirection);
                cout << "Room connection ok\n";
                return;
            }
            break;
        case 'e':
        case 'E':
            if(current->east() == nullptr)
            {
                tempDirection = new Room("NewRoom","This room is just waiting for shape and form");
                current->connect(EAST, tempDirection, ent_string);
                allRooms.push_back(tempDirection);
                cout << "Room connection ok\n";
                return;
            }
            break;
        case 'w':
        case 'W':
            if(current->west() == nullptr)
            {
                tempDirection = new Room("NewRoom","This room is just waiting for shape and form");
                current->connect(WEST, tempDirection, ent_string);
                allRooms.push_back(tempDirection);
                cout << "Room connection ok\n";
                return;
            }
            break;

    }

    cout << "No available exits in the current room\n";
}


void connect(Room* current, vector<Room*> allRooms)
{
    char n_s_e_w, other_room_exit;
    int connection;
    Room *other_room;
    bool is_used;
   // Direction cardinal;

    if(current == nullptr)
    {
        cout << "Room doesn't exist, try again.";
        return;
    }

    do
    {
        cout << "Connect new room in which direction? (N,S,E,W)\n";
        cin >> n_s_e_w; //enter a cardinal direction


       is_used = alreadyConnected(current, n_s_e_w);

    }while(!(never_eat_soggy_waffles(n_s_e_w)) || is_used); //can only enter a cardinal direction and room is available


    do
    {
        cout <<"Connect this room to which room? Choose a #\n";

        for(size_t i = 0; i < allRooms.size(); i++)
        {
            cout << i << ": " << allRooms.at(i)->name() <<endl;
        }

        cin >> connection;

    }while(connection > allRooms.size()-1 && connection >= 0);

    other_room = allRooms.at(connection);

    do
    {
        cout << "Which exit of the new room leads to this room?\n";
        cin >> other_room_exit;

        is_used = alreadyConnected(other_room, other_room_exit); //checking if the other room's exit is open


    }while(is_used || !(never_eat_soggy_waffles(other_room_exit)));

    current->connect(ch2Direct(n_s_e_w), other_room, ch2Direct(other_room_exit)); //connect the room
    cout << "Room connection ok\n";
}

// change the name of a room
void rename(Room* current)
{
    string newRoomName;
    if(current == nullptr)
    {
        cout<<"Sorry can't change name. Nothing there \n";  
        return;
    }
    cin.ignore();
    cout << "Enter a new name for this room and hit <enter>\n";
    getline(cin,newRoomName);
    current->set_name(newRoomName);
    //cout << "room has been renamed to " << newRoomName << endl;  
}

// change the description of a room
void description(Room* current)
{
    string text;
    cout<<"Enter a new description for this room and hit <enter>"<<endl;
    cin.ignore(1,'\n');
    getline(cin,text);
    current->set_description(text);
}

// for each exit from room r,
//   show the name of the room it connects to
//   or say "Nothing" if the link is null
void exits(Room* r)
{
    cout <<"  North -- ";
    if(r->north() != nullptr)
    {
        cout << (r->north())->name() <<endl;
    }
    else
        cout << "Nothing.\n";

    cout <<"  East -- ";
    if(r->east() != nullptr)
    {
        cout << (r->east())->name() <<endl;
    }
    else
        cout << "Nothing.\n";

    cout <<"  South -- ";
    if(r->south() != nullptr)
    {
        cout << (r->south())->name() <<endl;
    }
    else
        cout << "Nothing.\n";

    cout <<"  West -- ";
    if(r->west() != nullptr)
    {
        cout << (r->west())->name() <<endl;
    }
    else
        cout << "Nothing.\n";

   // cout << endl;
}
void menu()
{
    cout<<"Please choose from the following: "<<endl;
    cout<<"     n for north --- move north (if possible)"<<endl;
    cout<<"      e for east --- move east (if possible)"<<endl;
    cout<<"     s for south --- move south (if possible)"<<endl;
    cout<<"      w for west --- move at west (if possible)"<<endl;
    cout<<"      l for look --- look at room"<<endl;
    cout<<"     x for exits --- show room exits"<<endl;
    cout<<"   c for connect --- connect this room to another (already made) room"<<endl;
    cout<<"    r for rename --- rename this room"<<endl;
    cout<<"      d for desc --- change description for this room"<<endl;
    cout<<"       a for add --- add new room"<<endl;
    cout<<"      j for jump --- jump between any rooms"<<endl;
    cout<<"      u for save --- save your created rooms"<<endl;
    cout<<"      y for load --- load previously created rooms"<<endl;
    cout<<"      q for quit --- exit program"<<endl;
    cout<<"               ? --- show commands (DUH)"<<endl;
}


// convert a char direction: 'N','S','E','W'
// into a Direction enum: NORTH, SOUTH, EAST, WEST
Direction ch2Direct(char c)
{
    switch(c)
    {
        case 'N': return NORTH;
        case 'n': return NORTH;
        case 'S': return SOUTH;
        case 's': return SOUTH;
        case 'E': return EAST;
        case 'e': return EAST;
        case 'W': return WEST;
        case 'w': return WEST;
        default: return NORTH;
    }
}



bool alreadyConnected(Room* current, char dir) //checks if the chosen direction is already used 
{

    switch(dir)
    {

        case 'n': 
        case 'N':
            if(current->north() != nullptr)
            {
                cout <<"That exit is already connected!, that's digusting.\n";
                return true;
            }
            break;
        case 's':
        case 'S':    
            if(current->south() != nullptr)
            {
                cout <<"That exit is already connected!, that's digusting.\n";
                return true;
            }
            break;
        case 'e':
        case 'E':
            if(current->east() != nullptr)
            {
                cout <<"That exit is already connected!, that's digusting.\n";
                return true;
            }
            break;
        case 'w':
        case 'W':
            if(current->west() != nullptr)
            {
                cout <<"That exit is already connected!, that's digusting.\n";
                return true;
            }
            break;
    }
   // cout << "exit is freed up, fam\n";
    return false;
}

bool never_eat_soggy_waffles(char n_s_e_w) //return true if the char is a cardinal direction
{
    if(n_s_e_w != 'n' && n_s_e_w != 'N' && n_s_e_w != 's' && n_s_e_w != 'S'
           && n_s_e_w != 'e' && n_s_e_w != 'E' && n_s_e_w != 'W' && n_s_e_w != 'w')
    {
        return false;
    }
    else
        return true;
}
void saveGame(const string& filename, const vector<Room*>& allRooms)
{
 ofstream outfile(filename);
    if(outfile.is_open())
    {
        for(size_t i = 0; i < allRooms.size(); i++)
        {
            int roomIndex = i;
            outfile<< roomIndex << endl;
            outfile<< allRooms.at(i)->name() << endl;
            outfile<< allRooms.at(i)->description() << endl;
            if(allRooms.at(i)->north() != nullptr){
                int northRoomPtrIndex = findRoom(allRooms, allRooms.at(i)->north());
                Direction nReturnDir = findReturnDir(allRooms.at(i),allRooms.at(i)->north());
                outfile <<northRoomPtrIndex << " " << nReturnDir << endl;
            }else{
                outfile << "-1 -1" << endl;
            }
            if(allRooms.at(i)->east() != nullptr){
                int eastRoomPtrIndex = findRoom(allRooms, allRooms.at(i)->east());
                Direction eReturnDir = findReturnDir(allRooms.at(i),allRooms.at(i)->east());
                outfile  <<eastRoomPtrIndex << " " << eReturnDir << endl;
            }else{
                outfile << "-1 -1" << endl;
            }
            if(allRooms.at(i)->south() != nullptr){
                int southRoomPtrIndex = findRoom(allRooms, allRooms.at(i)->south());
                Direction sReturnDir = findReturnDir(allRooms.at(i),allRooms.at(i)->south());
                outfile <<southRoomPtrIndex <<" " <<sReturnDir <<endl;
            }else{
                outfile << "-1 -1" << endl;
            }
            if(allRooms.at(i)->west() != nullptr){
                int westRoomPtrIndex = findRoom(allRooms, allRooms.at(i)->west());
                Direction wReturnDir = findReturnDir(allRooms.at(i),allRooms.at(i)->west());
                outfile <<westRoomPtrIndex <<" " << wReturnDir << endl;
            }else{
                outfile << "-1 -1" << endl;
            }

        }
        outfile.close();
    }
}
int findRoom(const vector<Room*>& allRooms, const Room* roomPtr) {
    for (size_t j = 0; j < allRooms.size(); j++) {
        if (allRooms[j] == roomPtr) {
            return j;
        }
    }
    return -1; // Return -1 if roomPtr is not found in the vector
}
Direction findReturnDir(Room* currentRoom, Room* otherRoom)
{
    if(otherRoom->north() == currentRoom)
    {
        return NORTH;
    }
    else if(otherRoom->south() == currentRoom)
    {
        return SOUTH;
    }
    else if(otherRoom->west() == currentRoom)
    {
        return WEST;
    }
    else
    {
        return EAST;
    }

}
void loadGame(const string filename, vector<Room*>& allRooms)
{
    allRooms.clear();
    cout << "Loading file..." << endl;
    ifstream infile;
    infile.open(filename);
    if(!(infile.is_open()))
    {
        cout << "File is not open!" << endl;
        return;
    } else {
        cout << "File opened..." << endl;
    }
    int index;
    string name, description, line;
    int northRoomPtrIndex, eastRoomPtrIndex, southRoomPtrIndex, westRoomPtrIndex;
    int nReturnDir, eReturnDir, sReturnDir, wReturnDir;

    //use a loop to read the index, name, description, the roomPtrIndex, the returnPtr
    //and use them to add Room* and new Rooms to allRooms
    while (getline(infile>>ws, line) && !line.empty())//the last line in the save file is blank which ends the loop
    {
        index = stoi(line);  //our line with room index was used by getline to check the while loop
        getline(infile>>ws,name); // so we turn it to an int and assign index to its value
        getline(infile>>ws, description);
        Room* newRoom = new Room(name,description);
        allRooms.push_back(newRoom);
        //index++;
        infile >> northRoomPtrIndex >> nReturnDir;
        infile >> eastRoomPtrIndex >> eReturnDir;
        infile >> southRoomPtrIndex >> sReturnDir;
        infile >> westRoomPtrIndex >> wReturnDir;

    }
    // read the file add rooms discriptions and names then close it
    //open it up again and do the connections
    infile.close(); 
    line = "line";
    infile.open(filename);
    if(!(infile.is_open()))
    {
        cout << "File is not open!" << endl;
        return;
    }
while (getline(infile>>ws, line) && !line.empty())   //the last line in the save file is blank which ends the loop
{
    index = stoi(line);//our line with room index was used by getline to check the while loop
    infile.ignore();// so we turn it to an int and assign index to its value
    getline(infile >> ws, name);
    getline(infile >> ws, description);

    infile >> northRoomPtrIndex >> nReturnDir;
    if (northRoomPtrIndex >= 0 && northRoomPtrIndex < allRooms.size())
    {
        Room *northRoom = allRooms.at(northRoomPtrIndex);
        allRooms.at(index)->connect(NORTH, northRoom, static_cast<Direction>(nReturnDir));
    }

    infile >> eastRoomPtrIndex >> eReturnDir;
    if (eastRoomPtrIndex >= 0 && eastRoomPtrIndex < allRooms.size())
    {
        Room *eastRoom = allRooms.at(eastRoomPtrIndex);
        allRooms.at(index)->connect(EAST, eastRoom, static_cast<Direction>(eReturnDir));
    }

    infile >> southRoomPtrIndex >> sReturnDir;
    if (southRoomPtrIndex >= 0 && southRoomPtrIndex < allRooms.size())
    {
        Room *southRoom = allRooms.at(southRoomPtrIndex);
        allRooms.at(index)->connect(SOUTH, southRoom, static_cast<Direction>(sReturnDir));
    }

    infile >> westRoomPtrIndex >> wReturnDir;
    if (westRoomPtrIndex >= 0 && westRoomPtrIndex < allRooms.size())
    {
        Room *westRoom = allRooms.at(westRoomPtrIndex);
        allRooms.at(index)->connect(WEST, westRoom, static_cast<Direction>(wReturnDir));
    }
}

    infile.close(); 

    cout << "File loaded!" << endl;
}


void jump(Room* &current, vector <Room*> rooms) //allows you to jump directly to a room, passes current room by ref
{
    int num_choice = 0;
    cout << "Pick a room number to jump to, pard?:\n\n";

    for(size_t i = 0; i < rooms.size(); i++) //prints every room name
    {
        cout << i+1 <<".\t" << rooms.at(i)->name()<< endl;
    }

    do
    {
        cin >> num_choice; // take user choice for room to jump to

        num_choice--;
        //
    }while(num_choice < 0|| num_choice > rooms.size()-1); //blocks illegal room number entries

    if(rooms.at(num_choice) == current) //if you select your current room
    {
        cout << "You jump in place...Nothing happened\n";
        return;
    }

    cout << "Looks like you're hopping off to " << "\""<< rooms.at(num_choice)->name() <<"\"\n\n";

    current = rooms.at(num_choice); //makes current room user selection

}


template <class T>
void print(T thing)
{

 cout <<"\t" <<thing << endl;   
}