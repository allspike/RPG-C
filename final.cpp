#include <iostream>
#include <fstream>
#include <limits>
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
    int maxHealth;
    int potion_count = 3;
};
// create MinMax struct to store ranges for random number generator
struct GameData {
    string game_location;
    int f_min;
    int f_max;
    int w_min;
    int w_max;
    int h_min;
    int h_max;
};
// function to assess if a player has lost
void check_health(Player* p1, Player* p2) {
    ofstream oFile;
    oFile.open("rpg-results.txt", ios::app);

    if (oFile.is_open()) {
        if (p1->health <= 0) {
            cout << setw(32) << setfill('-') << '\n';
            cout << p1->name << ": " << p1->health << " | " << p2->name << ": " << p2->health << '\n';
            cout << setw(32) << setfill('-') << '\n';
            cout << endl;
            cout << p2->name << " has defeated " << p1->name << "!" << '\n';

            oFile.open("rpg-results.txt", ios::app);
            oFile << setw(32) << setfill('-') << '\n';
            oFile << p1->name << ": " << p1->health << " | " << p2->name << ": " << p2->health << '\n';
            oFile << setw(32) << setfill('-') << '\n';
            oFile << endl;
            oFile << p2->name << " has defeated " << p1->name << "!" << '\n';
            oFile.close();
            exit(0);
        } else if (p2->health <= 0) {
            cout << setw(32) << setfill('-') << '\n';
            cout << p1->name << ": " << p1->health << " | " << p2->name << ": " << p2->health << '\n';
            cout << setw(32) << setfill('-') << '\n';
            cout << endl;
            cout << p1->name << " has defeated " << p2->name << "!" << '\n';

            oFile.open("rpg-results.txt", ios::app);
            oFile << setw(32) << setfill('-') << '\n';
            oFile << p1->name << ": " << p1->health << " | " << p2->name << ": " << p2->health << '\n';
            oFile << setw(32) << setfill('-') << '\n';
            oFile << endl;
            oFile << p1->name << " has defeated " << p2->name << "!" << '\n';
            oFile.close();
            exit(0);
        }
    } else {
        cerr << "Unable to open file" << endl;
        exit(0);
    }
    oFile.close();
} // end check_health function

// function to display each round
void display_round(Player* p1, Player* p2) {
    // check player's health before continuing with displaying the round information
    check_health(p1, p2);
    ofstream oFile;
    oFile.open("rpg-results.txt", ios::app);

    cout << setw(32) << setfill('-') << '\n';
    cout << p1->name << ": " << p1->health << " | " << p2->name << ": " << p2->health << '\n';
    cout << setw(32) << setfill('-') << '\n';
    cout << endl;

    oFile << setw(32) << setfill('-') << '\n';
    oFile << p1->name << ": " << p1->health << " | " << p2->name << ": " << p2->health << '\n';
    oFile << setw(32) << setfill('-') << '\n';
    oFile << endl;

    cout << "It's " << p1->name << "'s move. Current Health is " << p1->health << '\n';
    cout << "1. Fight" << '\n';
    cout << "2. Use '" << p1->weapon << "'" << '\n';
    cout << "3. Drink a Healing Potion (" << p1->potion_count << " remaining)" << '\n';
    cout << "4. Exit game.\n";
    cout << "Action: ";
}
// function to process the input file and place appropriate values into the structs
void process_files(Player* player_1, Player* player_2, GameData* game_data) {
    ifstream iFile;
    iFile.open("rpg-data.txt");

    if (iFile.is_open()) {
        // read the file line by line to store necessary game variables
        getline(iFile, game_data->game_location);

        string line;
        getline(iFile, line);
        // parse each line using stringstream
        stringstream P1(line);
        P1 >> player_1->name >> player_1->health >> player_1->weapon;
        player_1->maxHealth = player_1->health;

        getline(iFile, line);
        stringstream P2(line);
        P2 >> player_2->name >> player_2->health >> player_2->weapon;
        player_2->maxHealth = player_2->health;

        getline(iFile, line);
        stringstream fight(line);
        fight >> game_data->f_min >> game_data->f_max;

        getline(iFile, line);
        stringstream weapon(line);
        weapon >> game_data->w_min >> game_data->w_max;

        getline(iFile, line);
        stringstream heal(line);
        heal >> game_data->h_min >> game_data->h_max;
    } else {
        cerr << "Error: could not open file." << endl;
    }
    // close the input file as we are done with it
    iFile.close();
}
// function to determine the outcome of a fight choice
void fight(Player* p1, Player* p2, GameData* gd) {
    ofstream oFile;
    oFile.open("rpg-results.txt", ios::app);
    if (oFile.is_open()){
        random_device eng;
        mt19937 gen(eng());
        uniform_int_distribution<> distrib(gd->f_min, gd->f_max);
        int player_1_fight = distrib(gen);
        int player_2_fight = distrib(gen);
        cout << p1->name << " has challenged " << p2->name << " to fight" << '\n';
        cout << "    " << p1->name << ": " << player_1_fight << " | " << p2->name << ": " << player_2_fight << '\n';

        oFile << p1->name << " has challenged " << p2->name << " to fight" << '\n';
        oFile << "    " << p1->name << ": " << player_1_fight << " | " << p2->name << ": " << player_2_fight << '\n';
        if (player_1_fight > player_2_fight) {
            p2->health -= (player_1_fight - player_2_fight);
            cout << p1->name << " deals " << player_1_fight - player_2_fight << " damage to " << p2->name << '\n';
            cout << endl;

            oFile << p1->name << " deals " << player_1_fight - player_2_fight << " damage to " << p2->name << '\n';
            oFile << endl;
        } else if (player_1_fight < player_2_fight) {
            p1->health -= (player_2_fight - player_1_fight);
            cout << p2->name << " deals " << player_2_fight -player_1_fight << " damage to " << p1->name << '\n';
            cout << endl;

            oFile << p2->name << " deals " << player_2_fight -player_1_fight << " damage to " << p1->name << '\n';
            oFile << endl;
        } else {
            cout << "The fight is a draw. Neither player loses any health points \n";
            cout << endl;

            oFile << "The fight is a draw. Neither player loses any health points \n";
            oFile << endl;
        }
    } else {
        cerr << "Unable to open file" << endl;
        exit(0);
    }
    oFile.close();
}
// function to determine the outcome of the use_weapon choice
void use_weapon(Player* p1, Player* p2, GameData* gd) {
    ofstream oFile;
    oFile.open("rpg-results.txt", ios::app);

    if (oFile.is_open()) {
        random_device eng;
        mt19937 gen(eng());
        uniform_int_distribution<> distrib(gd->w_min, gd->w_max);
        int player_1_weapon = distrib(gen);
        p2->health -= player_1_weapon;
        cout << p1->name << " uses a " << p1->weapon << " to attack " << p2->name << " for " << player_1_weapon << " damage \n";
        cout << endl;

        oFile << p1->name << " uses a " << p1-> weapon << " to attack " << p2->name << " for " << player_1_weapon << " damage \n";
        oFile << endl;
    } else {
        cerr << "Unable to open file" << endl;
        exit(0);
    }
    oFile.close();
}
// function to determine the outcome of the use_potion choice
void use_potion(Player* p1, GameData* gd) {
    ofstream oFile;
    oFile.open("rpg-results.txt", ios::app);

    if (oFile.is_open()) {
        random_device eng;
        mt19937 gen(eng());
        uniform_int_distribution<> distrib(gd->h_min, gd->h_max);
        int heal_points = distrib(gen);
        if (p1->potion_count > 0) {
            p1->health += heal_points;
            // check to make sure player does not heal past maximum HP
            if (p1->health > p1->maxHealth) {
                p1->health = p1->maxHealth;
                cout << p1->name << " drinks a healing potion and is now at " << p1->maxHealth << " health! \n";
                cout << endl;

                oFile << p1->name << " drinks a healing potion and is now at " << p1->maxHealth << " health! \n";
                oFile << endl;
            } else {
                p1->potion_count -= 1;
                cout << p1->name << " drinks a healing potion and regains " << heal_points << " health points." << '\n';
                cout << endl;

                oFile << p1->name << " drinks a healing potion and regains " << heal_points << " health points." << '\n';
                oFile << endl;
            }
        } else {
            cout << p1->name << " is out of healing potions!" << '\n';
            cout << endl;

            oFile << p1->name << " is out of healing potions!" << '\n';
            cout << endl;
        }
    } else {
        cerr << "Unable to open file" << endl;
        exit(0);
    }
    oFile.close();
}
// function to process the player choice
void process_choice(Player* p1, Player* p2, GameData* gd) {
    int choice;
    bool done = false;
    // do-while loop to continuously ask for a choice if the player selects an invalid option
    do {
        cin >> choice;
        // if cin fails, clear the buffer reset the input stream
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        switch (choice) {
            case 1:
                fight(p1, p2, gd);
                done = true;
                break;
            case 2:
                use_weapon(p1, p2, gd);
                done = true;
                break;
            case 3:
                use_potion(p1, gd);
                done = true;
                break;
            case 4:
                cout << "Thank you for playing!\n";
                exit(0);
            default:
                cout << choice << " is an invalid entry. Please select 1-3" << '\n';
                break;
        }
    } while (!done);
}



int main() {
    Player player_1;
    Player player_2;
    GameData game_data;

    Player *p1 = &player_1;
    Player *p2 = &player_2;
    GameData *gd = &game_data;

    process_files(p1, p2, gd);

    ofstream oFile;
    oFile.open("rpg-results.txt");

    cout << setw(30) << setfill('=') << '=' << endl;
    cout << "The Battle of " << game_data.game_location << endl;
    cout << setw(30) << setfill('=') << '=' << endl;
    cout << '\n';
    cout << player_1.name << " (" << player_1.health << ") with a " << player_1.weapon << '\n';
    cout << "    vs" << '\n';
    cout << player_2.name << " (" << player_2.health << ") with a " << player_2.weapon << '\n';
    cout << '\n';

    oFile << setw(30) << setfill('=') << '=' << endl;
    oFile << "The Battle of " << game_data.game_location << '\n';
    oFile << setw(30) << setfill('=') << '=' << endl;
    oFile << endl;
    oFile << player_1.name << " (" << player_1.health << ") with a " << player_1.weapon << '\n';
    oFile << "    vs" << '\n';
    oFile << player_2.name << " (" << player_2.health << ") with a " << player_2.weapon << '\n';
    oFile << endl;

    oFile.close();

    while (player_1.health >0 && player_2.health > 0) {
        display_round(p1, p2);
        process_choice(p1, p2, gd);
        display_round(p2, p1);
        process_choice(p2, p1, gd);

    }
}
