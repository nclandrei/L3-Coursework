//
// Name: Andrei-Mihai Nicolae
// GUID: 2147392N
// Title of assignment: AP3 Exercise 1
// This is my own work as defined in the Academic Ethics agreement I have signed.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "date.h"

struct date {
    int day;
    int month;
    int year;
};

Date *date_create(char *datestr) {
    Date *date = (Date *) malloc(sizeof(Date));
    char *token;
    const char del[2] = "/";

    if (date != NULL) {
        token = strtok(datestr, del);
        date->day = atoi(token);
        token = strtok(NULL, del);
        date->month = atoi(token);
        token = strtok(NULL, del);
        date->year = atoi(token);
    }
    return date;
};

Date *date_duplicate(Date *d) {
    Date *dupl = (Date *) malloc(sizeof(Date));
    if (dupl != NULL) {
        dupl->day = d->day;
        dupl->month = d->month;
        dupl->year = d->year;
    }
    return dupl;
};


int date_compare(Date *date1, Date *date2) {
    if (date1->year > date2->year)
        return 1;
    else if (date1->year < date2->year)
        return -1;
    else {
        if (date1->month < date2->month)
            return -1;
        else if (date1->month > date2->month)
            return 1;
        else {
            if (date1->day < date2->day)
                return -1;
            else if (date1->day > date2->day)
                return 1;
            else
                return 0;
        }
    }
};

void date_destroy(Date *d) {
    if (d != NULL) {
        free(d);
    }
};
