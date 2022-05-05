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

float calculateTeamPoints(Player* players, int nrOfPlayers) {
    float p = 0;

    int i;
    for (i = 0; i < nrOfPlayers; i++) {
        p += players[i].points;
    }

    return (p / nrOfPlayers);
}

int calculateNrOfTeams(char fileName[]) {
    FILE* f = fopen(fileName, "rt");

    if (f == NULL) {
        printf("cannot open the file\n");
        exit(-1);
    }

    int nrOfTeams;
    fscanf(f, "%d", &nrOfTeams);

    fclose(f);

    return nrOfTeams;
}

Team getTeam(FILE* f) {
    Team team;

    fscanf(f, "%d ", &team.nrPlayers);

    team.players = (Player*)malloc(team.nrPlayers * sizeof(Player));

    char auxTeamName[56];

    int i;

    fgets(auxTeamName, 56, f);
    auxTeamName[strlen(auxTeamName) - 1] = '\0';
    team.name = (char*)malloc((strlen(auxTeamName)) * sizeof(char));
    strcpy(team.name, auxTeamName);

    char auxPlayerFirstName[26];
    char auxPlayerSecondName[26];

    for (i = 0; i < team.nrPlayers; i++) {
        fscanf(f, "%s", auxPlayerFirstName);
        team.players[i].firstName = (char*)malloc((strlen(auxPlayerFirstName) + 1) * sizeof(char));
        strcpy(team.players[i].firstName, auxPlayerFirstName);

        fscanf(f, "%s", auxPlayerSecondName);
        team.players[i].secondName = (char*)malloc((strlen(auxPlayerSecondName) + 1) * sizeof(char));
        strcpy(team.players[i].secondName, auxPlayerSecondName);

        fscanf(f, "%d", &team.players[i].points);
    }

    team.points = calculateTeamPoints(team.players, team.nrPlayers);

    return team;
}

void deepCopy(Team team, Team* copyTeam) {
    (*copyTeam).name = (char*)malloc((strlen(team.name) + 1) * sizeof(char));
    strcpy((*copyTeam).name, team.name);

    (*copyTeam).nrPlayers = team.nrPlayers;

    (*copyTeam).points = team.points;   

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

    int i;
    for (i = 0; i < nrOfTeams; i++) {
        team = getTeam(f);

        insertNode(head, team);
    }

    fclose(f);
}

void printTeams(Node* head) {
    if (head == NULL) {
        return;
    }

    printf("%s\n", head->team.name);
    printTeams(head->next);
}

int calculateNumberOfTeamsToEliminate(int nrOfTeams) {
    int i = 2;

    while (i * 2 <= nrOfTeams) {
        i *= 2; 
    }

    return nrOfTeams - i;
}

Node* findPrevTeamOfMinPointsTeam(Node* head) {
    if (head == NULL) {
        return NULL;
    }

    if (head->next == NULL) {
        return head;
    }

    int minTeamPoints = INT_MAX;

    Node* currentNode = head;
    Node* prevNodeToCurrentNode = head;
    Node* prevNodeToMin;

    while (currentNode) {
        if (currentNode->team.points < minTeamPoints) {
            minTeamPoints = head->team.points;
            prevNodeToMin = prevNodeToCurrentNode;
        }

        prevNodeToCurrentNode = currentNode;
        currentNode = currentNode->next;
    }

    return prevNodeToMin;
}

void eliminateTeamFromList(Node* node, Node** head) {
    if (node == *head) {
        *head = (*head)->next; 
        free(node);

        return;
    }

    Node* nodeToEliminate = node->next;
    node->next = nodeToEliminate->next;

    free(nodeToEliminate);
}

void eliminateTeams(Node** head, int* nrOfTeams) {
    int nrOfTeamsToEliminate = calculateNumberOfTeamsToEliminate(*nrOfTeams);

    *nrOfTeams -= nrOfTeamsToEliminate;

    int i;

    Node* prevNodeToMinTeamPoints;

    for (i = 0; i < nrOfTeamsToEliminate; i++) {
        prevNodeToMinTeamPoints = findPrevTeamOfMinPointsTeam(*head);
        printf("%c\n", prevNodeToMinTeamPoints->next->team.name[strlen(prevNodeToMinTeamPoints->next->team.name) - 2]);
        printf("%s \n", prevNodeToMinTeamPoints->next->team.name);
        eliminateTeamFromList(prevNodeToMinTeamPoints, &(*head));
    }
}

int main() {
    Node* head = NULL;

    createTeamsList("date/t1/d.in", &head);
    
    int nrOfTeams = calculateNrOfTeams("date/t1/d.in");

    eliminateTeams(&head, &nrOfTeams);

    //printTeams(head);
}

// TO DO: at the end of every team name, there was an \n and I replaced that with \0 but still something is wrong. Fix that!
// TO DO: the teams are not elimineated corected (not by minimum points)