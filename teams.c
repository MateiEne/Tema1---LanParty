#include "teams.h"

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

float calculateTeamPoints(Player* players, int nrOfPlayers) {
    float p = 0;

    int i;
    for (i = 0; i < nrOfPlayers; i++) {
        p += players[i].points;
    }

    return (p / nrOfPlayers);
}

Team getTeam(FILE* f) {
    Team team;

    fscanf(f, "%d ", &team.nrPlayers);

    team.players = (Player*)malloc(team.nrPlayers * sizeof(Player));

    char auxTeamName[56];

    int i;

    fgets(auxTeamName, 56, f);
    auxTeamName[strlen(auxTeamName) - 2] = '\0';
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

    printf("%s %f\n", head->team.name, head->team.points);
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

    float minTeamPoints = INT_MAX;

    Node* currentNode = head;
    Node* prevNodeToCurrentNode = head;
    Node* prevNodeToMin;

    while (currentNode) {
        if (currentNode->team.points < minTeamPoints) {
            minTeamPoints = currentNode->team.points;
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
        printf("%s \n", prevNodeToMinTeamPoints->next->team.name);
        eliminateTeamFromList(prevNodeToMinTeamPoints, head);
    }
}