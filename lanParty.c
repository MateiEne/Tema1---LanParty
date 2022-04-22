#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player {
    char* firstName;
    char* secondName;
    int points;
}Player;

typedef struct Team {
    int nrPlayers;
    char* name;
    Player* players;
}Team;

struct Elem {
    Team team;
    struct Elem* next;
};

typedef struct Elem Node;

Team getTeam(FILE* f) {
    Team team;

    char c;

    fscanf(f, "%d ", &team.nrPlayers);

    team.players = (Player*)malloc(team.nrPlayers * sizeof(Player));
    
    char auxTeamName[56];

    fgets(auxTeamName, 56, f);
    team.name = (char*)malloc((strlen(auxTeamName) + 1) * sizeof(char));
    strcpy(team.name, auxTeamName);

    printf("%s\n", auxTeamName);

    char auxPlayerFirstName[26];
    char auxPlayerSecondName[26];

    int i;
    for (i = 0; i < team.nrPlayers; i++) {
        fscanf(f, "%s", auxPlayerFirstName);
        team.players[i].firstName = (char*)malloc((strlen(auxPlayerFirstName) + 1) * sizeof(char));
        strcpy(team.players[i].firstName, auxPlayerFirstName);

        fscanf(f, "%s", auxPlayerSecondName);
        team.players[i].secondName = (char*)malloc((strlen(auxPlayerSecondName) + 1) * sizeof(char));
        strcpy(team.players[i].secondName, auxPlayerSecondName);

        fscanf(f, "%d", &team.players[i].points);
    }

    return team;
}

void deepCopy(Team team, Team* copyTeam) {
    (*copyTeam).name = (char*)malloc((strlen(team.name) + 1) * sizeof(char));
    strcpy((*copyTeam).name, team.name);

    (*copyTeam).nrPlayers = team.nrPlayers;

    (*copyTeam).players = (Player*)malloc(team.nrPlayers * sizeof(Player));

    int i;
    for (i = 0; i < team.nrPlayers; i++) {
        (*copyTeam).players[i].firstName = (char*)malloc((strlen(team.players[i].firstName) + 1) * sizeof(char));
        strcpy((*copyTeam).players[i].firstName, team.players[i].firstName);

        (*copyTeam).players[i].secondName = (char*)malloc((strlen(team.players[i].secondName) + 1) * sizeof(char));
        strcpy((*copyTeam).players[i].secondName, team.players[i].secondName);

        (*copyTeam).players[i].points = team.players[i].points;
    }
}

void insertNode(Node** head, Team team) {
    Node* node = (Node*)malloc(sizeof(Node));


    node->next = *head;
    deepCopy(team, &node->team);
    *head = node;
}

void createTeamsList(char fileName[], Node** head) {
    FILE* f = fopen(fileName, "rt");

    if (f == NULL) {
        printf("cannot open the file\n");
        exit(-1);
    }

    Team team;

    int nrOfTeams;

    fscanf(f, "%d", &nrOfTeams);

    int i, j;
    for (i = 0; i < nrOfTeams; i++) {
        team = getTeam(f);

        insertNode(head, team);
    }

    fclose(f);
}

int main() {
    Node* head = NULL;

    createTeamsList("date/t1/d.in", &head);

    printf("%s\n", head->team.name);
}