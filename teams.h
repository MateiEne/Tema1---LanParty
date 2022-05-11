#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAX 10000

typedef struct Player {
    char* firstName;
    char* secondName;
    int points;
}Player;

typedef struct Team {
    int nrPlayers;
    char* name;
    Player* players;
    float points;
}Team;

struct Elem {
    Team team;
    struct Elem* next;
};

typedef struct Elem Node;

int calculateNrOfTeams(char fileName[]);

Team getTeam(FILE* f);

float calculateTeamPoints(Player* players, int nrOfPlayers);

void createTeamsList(char fileName[], Node** head);

void insertNode(Node** head, Team team);

void deepCopy(Team team, Team* copyTeam);

void printTeams(Node* head);

int calculateNumberOfTeamsToEliminate(int nrOfTeams);

void eliminateTeams(Node** head, int* nrOfTeams);

void eliminateTeamFromList(Node* node, Node** head);

Node* findPrevTeamOfMinPointsTeam(Node* head);