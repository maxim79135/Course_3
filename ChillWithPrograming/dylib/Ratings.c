/* File: Ratings.c
 * Compile with -fvisibility=hidden.
 **********************************/
 
#include "Ratings.h"
#include <Averages.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
 
#define EXPORT __attribute__((visibility("default")))
#define MAX_NUMBERS 99
//#define MAX_NUMERIC_RATING 10               // published in Ratings.h
 
static char *_char_rating(float rating) {
    char result[10] = "";
    int int_rating = (int)(rating + 0.5);
    for (int i = 0; i < int_rating; i++) {
        strncat(result, "*", sizeof(result) - strlen(result) - 1);
    }
    return strdup(result);
}
 
EXPORT
void addRating(char *rating) {
    if (rating != NULL) {
        int numeric_rating = 0;
        int pos = 0;
        while (*rating++ != '\0' && pos++ < 5) {
            numeric_rating++;
        }
        add((float)numeric_rating);     // libAverages.A:add()
    }
}
 
EXPORT
char *meanRating(void) {
    return _char_rating(mean());        // libAverages.A:mean()
}
 
EXPORT
char *medianRating(void) {
    return _char_rating(median());      // libAverages.A:median()
}
 
EXPORT
char *frequentRating(void) {
    int lib_mode = mode();                // libAverages.A:mode()
    return _char_rating(lib_mode);
}
 
EXPORT
int ratings(void) {
    return count();                     // libAverages.A:count()
}
 
EXPORT
void clearRatings(void) {
    clear();                            // libAverages.A:clear()
}
 
 
/* Ratings.c revision history
 * 1. First version.
 * 2. Added medianRating, frequentRating.
 *    Removed initializer, finalizer.
 */