#include "oscars.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global variables for the awards database
static Award* awardsDatabase = NULL;
static size_t awardsCount = 0;

// Helper function to create a new list
static OscarList* createOscarList() {
    OscarList* list = malloc(sizeof(OscarList));
    if (list) {
        list->head = NULL;
        list->count = 0;
        list->minYear = 9999;
        list->maxYear = 0;
    }
    return list;
}

// Adds an award to the OscarList in sorted order
static void addToOscarList(OscarList* list, const Award* award) {
    if (!list || !award) return;

    Node* newNode = malloc(sizeof(Node));
    if (!newNode) return; // Memory allocation failure
    newNode->oscar = award;
    newNode->next = NULL;

    Node** current = &list->head;
    while (*current &&
           ((*current)->oscar->year < award->year ||
            ((*current)->oscar->year == award->year &&
             ((*current)->oscar->award < award->award ||
              ((*current)->oscar->award == award->award &&
               strcmp((*current)->oscar->name, award->name) < 0))))) {
        current = &(*current)->next;
    }

    newNode->next = *current;
    *current = newNode;

    // Update list metadata
    list->count++;
    if (award->year < list->minYear) list->minYear = award->year;
    if (award->year > list->maxYear) list->maxYear = award->year;
}

// Custom implementation of case-insensitive substring search
static const char* customStrcasestr(const char* haystack, const char* needle) {
    if (!haystack || !needle || *needle == '\0') return NULL;

    size_t needleLen = strlen(needle);
    for (; *haystack; haystack++) {
        if (strncasecmp(haystack, needle, needleLen) == 0) {
            return haystack;
        }
    }
    return NULL;
}

// Reads the file and populates the global awards database
int readFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file: %s\n", filename);
        return -1;
    }

    awardsCount = 0;
    awardsDatabase = realloc(awardsDatabase, 2000 * sizeof(Award));
    if (!awardsDatabase) {
        fclose(file);
        printf("Error: Memory allocation failed.\n");
        return -1;
    }

    char line[200];
    unsigned int year, awardType, winner;
    char name[50], film[100];

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';

        if (sscanf(line, "%u", &year) != 1) continue;

        if (!fgets(line, sizeof(line), file)) break;
        line[strcspn(line, "\r\n")] = '\0';
        if (strcmp(line, "PICTURE") == 0) awardType = 0;
        else if (strcmp(line, "ACTRESS") == 0) awardType = 1;
        else if (strcmp(line, "ACTOR") == 0) awardType = 2;
        else continue;

        if (!fgets(name, sizeof(name), file)) break;
        name[strcspn(name, "\r\n")] = '\0';

        if (!fgets(film, sizeof(film), file)) break;
        film[strcspn(film, "\r\n")] = '\0';

        if (!fgets(line, sizeof(line), file)) break;
        line[strcspn(line, "\r\n")] = '\0';
        if (strcmp(line, "TRUE") == 0) winner = 1;
        else if (strcmp(line, "FALSE") == 0) winner = 0;
        else continue;

        awardsDatabase[awardsCount].year = year;
        awardsDatabase[awardsCount].award = awardType;
        strncpy(awardsDatabase[awardsCount].name, name, sizeof(awardsDatabase[awardsCount].name) - 1);
        strncpy(awardsDatabase[awardsCount].film, film, sizeof(awardsDatabase[awardsCount].film) - 1);
        awardsDatabase[awardsCount].winner = winner;

        awardsCount++;
    }

    fclose(file);
    return awardsCount;
}

// Finds awards by type and year range
OscarList* findAward(const char* awardType, unsigned int fromYr, unsigned int toYr) {
    unsigned int typeCode = (strcmp(awardType, "ACTOR") == 0) ? 2 :
                            (strcmp(awardType, "ACTRESS") == 0) ? 1 :
                            (strcmp(awardType, "PICTURE") == 0) ? 0 : -1;

    OscarList* list = createOscarList();
    for (size_t i = 0; i < awardsCount; i++) {
        if ((typeCode == -1 || awardsDatabase[i].award == typeCode) &&
            awardsDatabase[i].year >= fromYr &&
            awardsDatabase[i].year <= toYr) {
            addToOscarList(list, &awardsDatabase[i]);
        }
    }
    return list;
}

// Finds awards by name substring (case-insensitive)
OscarList* findName(const char* str) {
    OscarList* list = createOscarList();
    if (!str || strlen(str) == 0) return list;

    for (size_t i = 0; i < awardsCount; i++) {
        const char* name = awardsDatabase[i].name;
        if (customStrcasestr(name, str)) {
            addToOscarList(list, &awardsDatabase[i]);
        }
    }
    return list;
}

// Finds awards by film substring (case-insensitive)
OscarList* findFilm(const char* str) {
    OscarList* list = createOscarList();
    if (!str || strlen(str) == 0) return list;

    for (size_t i = 0; i < awardsCount; i++) {
        const char* film = awardsDatabase[i].film;
        if (customStrcasestr(film, str)) {
            addToOscarList(list, &awardsDatabase[i]);
        }
    }
    return list;
}
