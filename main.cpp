#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>


using namespace std;
// create Player struct
struct Player {
    string name;
    string weapon;
    int health;
    int potion_count = 3;
};
// create MinMax struct to store ranges for random number generator
struct MinMax {
    int f_min;
    int f_max;
    int w_min;
    int w_max;
    int h_min;
    int h_max;
};
// Function that takes in two Player pointers and a MinMax pointer and calculates each round
void display_round( Player* player_1,  Player* player_2, MinMax* min_max) {
    // declare choice integer for the switch case
    int choice;
    // boolean variable to end the game
    bool done = false;
    // declare and open the output file
    ofstream oFile;
    oFile.open("rpg-results.txt", ios::app);
    // do-while loop to continue the game until conditions are met to end it
    if (oFile.is_open()) {
        do {
            // formatting each round
            cout << setw(32) << setfill('-') << '\n';
            cout << player_1->name << ": " << player_1->health << " | " << player_2->name << ": " << player_2->health << '\n';
            cout << setw(32) << setfill('-') << '\n';
            cout << endl;

            oFile << setw(32) << setfill('-') << '\n';
            oFile << player_1->name << ": " << player_1->health << " | " << player_2->name << ": " << player_2->health << '\n';
            oFile << setw(32) << setfill('-') << '\n';
            oFile << endl;

            cout << "It's " << player_1->name << "'s move. Current Health is " << player_1->health << '\n';
            cout << "1. Fight" << '\n';
            cout << "2. Use '" << player_1->weapon << "'" << '\n';
            cout << "3. Drink a Healing Potion (" << player_1->potion_count << " remaining)" << '\n';
            cout << "Action: ";
            // take in the players choice
            cin >> choice;

            // switch statement to evaluate each choice
            switch (choice) {
                // logic for choosing the "fight" option
                case 1:
                    {
                    // create a random number generator utilizing the range set by minmax
                    random_device eng;
                    mt19937 gen(eng());
                    uniform_int_distribution<> distrib(min_max->f_min, min_max->f_max);
                    int player_1_fight = distrib(gen);
                    int player_2_fight = distrib(gen);
                    cout << player_1->name << " has challenged " << player_2->name << " to fight" << '\n';
                    cout << "    " << player_1->name << ": " << player_1_fight << " | " << player_2->name << ": " << player_2_fight << '\n';

                    oFile << player_1->name << " has challenged " << player_2->name << " to fight" << '\n';
                    oFile << "    " << player_1->name << ": " << player_1_fight << " | " << player_2->name << ": " << player_2_fight << '\n';
                    if (player_1_fight > player_2_fight) {
                        player_2->health -= (player_1_fight - player_2_fight);
                        cout << player_1->name << " deals " << player_1_fight - player_2_fight << " damage to " << player_2->name << '\n';
                        cout << endl;

                        oFile << player_1->name << " deals " << player_1_fight - player_2_fight << " damage to " << player_2->name << '\n';
                        oFile << endl;
                    } else if (player_1_fight < player_2_fight) {
                        player_1->health -= (player_2_fight - player_1_fight);
                        cout << player_2->name << " deals " << player_2_fight -player_1_fight << " damage to " << player_1->name << '\n';
                        cout << endl;

                        oFile << player_2->name << " deals " << player_2_fight -player_1_fight << " damage to " << player_1->name << '\n';
                        oFile << endl;
                    } else {
                        cout << "The fight is a draw. Neither player loses any health points \n";
                        cout << endl;

                        oFile << "The fight is a draw. Neither player loses any health points \n";
                        oFile << endl;
                    }
                    // sets done to true which breaks the loop
                    oFile.close();
                    done = true;
                    // breaks out of the switch cases
                    break;
                    }
                // logic for performing the "use weapon" action
                case 2:
                    // logic for creating a random distribution between the weapon ranges
                    {
                        random_device eng;
                        mt19937 gen(eng());
                        uniform_int_distribution<> distrib(min_max->w_min, min_max->w_max);
                        int player_1_weapon = distrib(gen);
                        player_2->health -= player_1_weapon;
                        cout << player_1->name << " uses a " << player_1->weapon << " to attack " << player_2->name << " for " << player_1_weapon << " damage \n";
                        cout << endl;

                        oFile << player_1->name << " uses a " << player_1-> weapon << " to attack " << player_2->name << " for " << player_1_weapon << " damage \n";
                        oFile << endl;
                        // sets done to true which breaks the loop
                        oFile.close();
                        done = true;
                        // breaks out of the switch case
                        break;
                    }
                // logic for performing the "use potion" action
                case 3:
                    {
                        // create a random distribution for the potion healing
                        random_device eng;
                        mt19937 gen(eng());
                        uniform_int_distribution<> distrib(min_max->h_min, min_max->h_max);
                        int heal_points = distrib(gen);
                        if (player_1->potion_count > 0) {
                            player_1->health += heal_points;
                            player_1->potion_count -= 1;
                            cout << player_1->name << " drinks a healing potion and regains " << heal_points << " health points" << '\n';
                            cout << endl;

                            oFile << player_1->name << " drinks a healing potion and regains " << heal_points << " health points" << '\n';
                            oFile << endl;
                        } else {
                            cout << player_1->name << " is out of healing potions!" << '\n';
                            cout << endl;

                            oFile << player_1->name << " is out of healing potions!" << '\n';
                            cout << endl;
                        }
                        // breaks the loop
                        oFile.close();
                        done = true;
                        // breaks out of the switch case
                        break;
                    }
                default:
                    // default case to inform the user that the entry isn't valid. This restarts the loop and shows the options to choose from again
                    cout << choice << " is an invalid entry. Please select 1-3" << '\n';
                    cout << endl;

            }
        } while (!done);
    } else {
        cerr << "Error: Could not open file." << endl;
    }
}
int main() {
    // declare P1 and P2 name variables
    string Game_Location;

    Player player_1;
    Player player_2;
    // create pointers for the Player structs to allow pass by reference to the helper function
    Player *p1 = &player_1;
    Player *p2 = &player_2;

    MinMax min_max;
    // create pointer for the min_max struct to allow pass by reference to the helper function
    MinMax *mm = &min_max;
    int Min_Fight, Max_Fight;
    int Min_Weapon, Max_Weapon;
    int Min_Heal, Max_Heal;

    // declare file variables
    ifstream iFile;
    ofstream oFile;

    // open both files
    iFile.open("rpg-data.txt");
    oFile.open("rpg-results.txt");
    // verify the input file is open to begin reading the contents
    if (iFile.is_open()) {
        // read the file line by line to store necessary game variables
        getline(iFile, Game_Location);

        string line;
        getline(iFile, line);
        // parse each line using stringstream
        stringstream P1(line);
        P1 >> player_1.name >> player_1.health >> player_1.weapon;

        getline(iFile, line);
        stringstream P2(line);
        P2 >> player_2.name >> player_2.health >> player_2.weapon;

        getline(iFile, line);
        stringstream fight(line);
        fight >> min_max.f_min >> min_max.f_max;

        getline(iFile, line);
        stringstream weapon(line);
        weapon >> min_max.w_min >> min_max.w_max;

        getline(iFile, line);
        stringstream heal(line);
        heal >> min_max.h_min >> min_max.h_max;
    } else {
        cerr << "Error: could not open file." << endl;
    }
    // close the input file as we are done with it
    iFile.close();

    // show the initial game start screen
    cout << setw(30) << setfill('=') << '=' << endl;
    cout << "The Battle of " << Game_Location << endl;
    cout << setw(30) << setfill('=') << '=' << endl;
    cout << '\n';
    cout << player_1.name << " (" << player_1.health << ") with a " << player_1.weapon << '\n';
    cout << "    vs" << '\n';
    cout << player_2.name << " (" << player_2.health << ") with a " << player_2.weapon << '\n';
    cout << '\n';

    oFile << setw(30) << setfill('=') << '=' << endl;
    oFile << "The battle of " << Game_Location << '\n';
    oFile << setw(30) << setfill('=') << '=' << endl;
    oFile << endl;
    oFile << player_1.name << " (" << player_1.health << ") with a " << player_1.weapon << '\n';
    oFile << "    vs" << '\n';
    oFile << player_2.name << " (" << player_2.health << ") with a " << player_2.weapon << '\n';
    oFile << endl;

    oFile.close();
    while (player_1.health >0 && player_2.health > 0) {

        if (player_1.health <= 0) {
            cout << setw(32) << setfill('-') << '\n';
            cout << player_1.name << ": " << player_1.health << " | " << player_2.name << ": " << player_2.health << '\n';
            cout << setw(32) << setfill('-') << '\n';
            cout << endl;
            cout << player_2.name << " has defeated " << player_1.name << "!" << '\n';

            oFile.open("rpg-results.txt", ios::app);
            oFile << setw(32) << setfill('-') << '\n';
            oFile << player_1.name << ": " << player_1.health << " | " << player_2.name << ": " << player_2.health << '\n';
            oFile << setw(32) << setfill('-') << '\n';
            oFile << endl;
            oFile << player_2.name << " has defeated " << player_1.name << "!" << '\n';
            oFile.close();
            break;
        } else if (player_2.health <= 0) {
            cout << setw(32) << setfill('-') << '\n';
            cout << player_1.name << ": " << player_1.health << " | " << player_2.name << ": " << player_2.health << '\n';
            cout << setw(32) << setfill('-') << '\n';
            cout << endl;
            cout << player_1.name << " has defeated " << player_2.name << "!" << '\n';

            oFile.open("rpg-results.txt", ios::app);
            oFile << setw(32) << setfill('-') << '\n';
            oFile << player_1.name << ": " << player_1.health << " | " << player_2.name << ": " << player_2.health << '\n';
            oFile << setw(32) << setfill('-') << '\n';
            oFile << endl;
            oFile << player_1.name << " has defeated " << player_2.name << "!" << '\n';
            oFile.close();
            break;
        }
        display_round(p1, p2, mm);
        if (player_1.health <= 0) {
            cout << setw(32) << setfill('-') << '\n';
            cout << player_1.name << ": " << player_1.health << " | " << player_2.name << ": " << player_2.health << '\n';
            cout << setw(32) << setfill('-') << '\n';
            cout << endl;
            cout << player_2.name << " has defeated " << player_1.name << "!" << '\n';

            oFile.open("rpg-results.txt", ios::app);
            oFile << setw(32) << setfill('-') << '\n';
            oFile << player_1.name << ": " << player_1.health << " | " << player_2.name << ": " << player_2.health << '\n';
            oFile << setw(32) << setfill('-') << '\n';
            oFile << endl;
            oFile << player_2.name << " has defeated " << player_1.name << "!" << '\n';
            oFile.close();
            break;
        } else if (player_2.health <= 0) {
            cout << setw(32) << setfill('-') << '\n';
            cout << player_1.name << ": " << player_1.health << " | " << player_2.name << ": " << player_2.health << '\n';
            cout << setw(32) << setfill('-') << '\n';
            cout << endl;
            cout << player_1.name << " has defeated " << player_2.name << "!" << '\n';

            oFile.open("rpg-results.txt", ios::app);
            oFile << setw(32) << setfill('-') << '\n';
            oFile << player_1.name << ": " << player_1.health << " | " << player_2.name << ": " << player_2.health << '\n';
            oFile << setw(32) << setfill('-') << '\n';
            oFile << endl;
            oFile << player_1.name << " has defeated " << player_2.name << "!" << '\n';
            oFile.close();
            break;
        }
        display_round(p2, p1, mm);
    }
}
