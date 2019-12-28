/* File: Ratings.h
 * Interface to libRatings.A.dylib 1.1.
 *************************************/
 
#define WEAK_IMPORT __attribute__((weak_import))
 
/* Adds 'rating' to the set.
 *      rating: Each character adds 1 to the numeric rating
 *      Example: "" = 0, "*" = 1, "**" = 2, "wer " = 4.
 */
void addRating(char* rating);
 
/* Returns the number of ratings in the set.
 */
int ratings(void);
 
/* Returns the mean rating of the set.
 */
char* meanRating(void);
 
/* Returns the medianRating of the set.
 */
WEAK_IMPORT
char *medianRating(void);                       // 1
 
/* Returns the most frequent rating of the set.
 */
WEAK_IMPORT
char *frequentRating(void);                     // 2
 
/* Clears the set.
 */
void clearRatings(void);