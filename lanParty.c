#include "teams.h"

int main() {
    Node* head = NULL;

    createTeamsList("date/t1/d.in", &head);
    
    int nrOfTeams = calculateNrOfTeams("date/t1/d.in");

    //printf("nr: %d\n", nrOfTeams);

    eliminateTeams(&head, &nrOfTeams);

    //printTeams(head);
}