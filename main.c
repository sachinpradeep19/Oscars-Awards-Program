#include <stdio.h>
#include "oscars.h"

// Print the award in a text-friendly way
void printAward(const Award* a) {
   const char * type[3] = {"PICTURE", "ACTRESS", "ACTOR"};
   const char * win[2] = {"Nominee", "WINNER"};
   if (!a) return;  // Do nothing if NULL pointer
   printf("%u BEST %s: ", a->year, type[a->award]);
   if (a->name[0]) {
      printf("%s (%s) -- %s\n", a->name, a->film, win[a->winner]);
   } else {
      printf("%s -- %s\n", a->film, win[a->winner]);
   }
}

// Print all awards from a list
void printAwardList(const OscarList* list) {
   if (!list || list->count == 0) {
      printf("No awards found.\n\n");
      return;
   }
   printf("%u awards from %u to %u\n", list->count, list->minYear, list->maxYear);
   for (Node *p = list->head; p; p = p->next) {
      printAward(p->oscar);
   }
   printf("\n");
}

int main(void) {
   printf("Testing Oscars Program...\n");

   // Test 1: Read File
   printf("\n[TEST 1] Reading the awards file...\n");
   int numAwards = readFile("oscars.txt");
   if (numAwards < 0) {
      printf("Error: Could not open file.\n");
      return 1;
   } else {
      printf("Number of awards read: %d\n", numAwards);
   }

   // Test 2: Find Awards by Type and Year
   printf("\n[TEST 2] Finding 'ACTOR' awards from 2000 to 2023...\n");
   OscarList* actorAwards = findAward("ACTOR", 2000, 2023);
   printAwardList(actorAwards);

   // Test 3: Find by Name
   printf("\n[TEST 3] Searching for awards with the name containing 'Meryl'...\n");
   OscarList* nameResults = findName("Meryl");
   printAwardList(nameResults);

   // Test 4: Find by Film
   printf("\n[TEST 4] Searching for awards with films containing 'Titanic'...\n");
   OscarList* filmResults = findFilm("Titanic");
   printAwardList(filmResults);

   return 0;
}
