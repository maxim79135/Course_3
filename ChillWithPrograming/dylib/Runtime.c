/* Runtime.c
 * Tests libRatings.A.dylib 1.1 as a runtime-loaded library.
 **********************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include "Ratings.h"
 
#define PASSFAIL "Passed":"Failed"
#define UNTST "Untested"
 
int main(int argc, char** argv) {
    printf("[start_test]\n");
 
    // Open the library.
    char *lib_name = "./libRatings.A.dylib";
    void *lib_handle = dlopen(lib_name, RTLD_NOW);
    if (lib_handle) {
        printf("[%s] dlopen(\"%s\", RTLD_NOW): Successful\n", __FILE__, lib_name);
    }
    else {
        printf("[%s] Unable to open library: %s\n",
            __FILE__, dlerror());
        exit(EXIT_FAILURE);
    }
 
    // Get the function addresses.
    void (*addRating)(char*) = dlsym(lib_handle, "addRating");
    if (addRating) {
        printf("[%s] dlsym(lib_handle, \"addRating\"): Successful\n", __FILE__);
    }
    else {
        printf("[%s] Unable to get symbol: %s\n",
            __FILE__, dlerror());
        exit(EXIT_FAILURE);
    }
    char* (*meanRating)(void) = dlsym(lib_handle, "meanRating");
    if (meanRating) {
        printf("[%s] dlsym(lib_handle, \"meanRating\"): Successful\n", __FILE__);
    }
    else {
        printf("[%s] Unable to get symbol: %s\n",
            __FILE__, dlerror());
        exit(EXIT_FAILURE);
    }
    void (*clearRatings)(void) = dlsym(lib_handle, "clearRatings");
    if (clearRatings) {
        printf("[%s] dlsym(lib_handle, \"clearRatings\"): Successful\n", __FILE__);
    }
    else {
        printf("[%s] Unable to get symbol: %s\n",
            __FILE__, dlerror());
        exit(EXIT_FAILURE);
    }
    int (*ratings)(void) = dlsym(lib_handle, "ratings");
    if (ratings) {
        printf("[%s] dlsym(lib_handle, \"ratings\"): Successful\n", __FILE__);
    }
    else {
        printf("[%s] Unable to get symbol: %s\n",
            __FILE__, dlerror());
        exit(EXIT_FAILURE);
    }
    char *(*medianRating)(void) = dlsym(lib_handle, "medianRating");        // weak import
    char *(*frequentRating)(void) = dlsym(lib_handle, "frequentRating");    // weak import
 
    // Setup.
    addRating(NULL);
    addRating("");
    addRating("*");
    addRating("**");
    addRating("***");
    addRating("*****");
    addRating("*****");
 
    // ratings.
    printf("[%s] ratings(): %s\n", __FILE__, (ratings() == 6? PASSFAIL));
 
    // meanRating.
    printf("[%s] meanRating(): %s\n", __FILE__, (strcmp(meanRating(), "**") == 0)? PASSFAIL);
 
    // medianRating.
    if (medianRating) {
        printf("[%s] medianRating(): %s\n", __FILE__, (strcmp(medianRating(), "**") == 0? PASSFAIL));
    }
    else {
        printf("[%s] medianRating(): %s\n", __FILE__, UNTST);
    }
 
    // frequentRating.
 
    if (frequentRating) {
        char* test_rating = "*****";
        int test_rating_size = sizeof(test_rating);
        printf("[%s] frequentRating(): %s\n", __FILE__, strncmp(test_rating, frequentRating(), test_rating_size) == 0? PASSFAIL);
    }
    else {
        printf("[%s] mostFrequentRating(): %s\n", __FILE__, UNTST);
    }
 
    // clearRatings.
    clearRatings();
    printf("[%s] clearRatings(): %s\n", __FILE__, (ratings() == 0? PASSFAIL));
 
    // Close the library.
    if (dlclose(lib_handle) == 0) {
        printf("[%s] dlclose(lib_handle): Successful\n", __FILE__);
    }
    else {
        printf("[%s] Unable to open close: %s\n",
            __FILE__, dlerror());
    }
 
    printf("[end_test]\n");
    return 0;
}