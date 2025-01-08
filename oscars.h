#ifndef FALL24_OSCARS_H
#define FALL24_OSCARS_H

struct award {
   unsigned int year;
   unsigned int award;  // 0 = picture, 1 = actress, 2 = actor
   char name[50];
   char film[100];
   unsigned int winner;  // 1 = winner, 0 = nominee
};
typedef struct award Award;

struct oscarNode {
   const struct award * oscar;
   struct oscarNode * next;
};
typedef struct oscarNode Node;

struct oscarList {
   struct oscarNode * head;
   // no need for tail pointer
   unsigned int count;   // number of items in list
   unsigned int minYear;   // range of years in this list
   unsigned int maxYear;
};
typedef struct oscarList OscarList;

// read award information from file
// return -1 if cannot open, otherwise number of awards read
int readFile(const char* name);

// award is "ACTOR", "ACTRESS", "PICTURE", "*" (any)
// create and return a new list, sorted by year, then award
OscarList * findAward(const char *award, unsigned int fromYr, unsigned int toYr);

// str is a substring to match (case-insensitive)
// create and return a new list, sorted by name (case-sensitive), then year
OscarList * findName(const char *str);

// str is a substring to match (case-insensitive)
// create and return a new list, sorted by name (case-sensitive), then year
OscarList * findFilm(const char *str);

#endif //FALL24_OSCARS_H
