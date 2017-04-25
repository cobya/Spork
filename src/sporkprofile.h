/**************************************************************************************************/

/*
 * File: sporkprofile.h
 * Author: Coby Allred
 * NetID: cobyallred
 * Date: 9/15/16
 *
 * Description: Definition of SporkProfile for storing profile data for a restauarant/business
 *              declaration of functions for reading, processing, and writing Spork profiles.
 *
 */

/**************************************************************************************************/

#ifndef SPORKPROFILE_H
#define SPORKPROFILE_H

/**************************************************************************************************/

#include <stdbool.h>

/**************************************************************************************************/

#define MAX_BUSINESSNAME_LEN 100

/**************************************************************************************************/

/*
 * Data structure for representing a Spork profile for restauarant/business. businessName is the
 * name of the restauarant/business. locX and locY are the x and y coordinates of the business.
 * avgRating is the average rating for the business. adLevel is a value ranging from 0 to 2
 * describing the business advertising level. isNearby is used to indicate if the business is near
 * the user's location. isGood indicates if the business is greater than or equal to
 * the user's minimum desired rating.
 *
 */
typedef struct SporkProfile_struct {
   char businessName[MAX_BUSINESSNAME_LEN];
   double locX;
   double locY;
   double distMiles;
   double avgRating;
   int adLevel;
   bool isNearby;
   bool isGood;
} SporkProfile;

/**************************************************************************************************/

/* Reads up to maxProfiles Spork profiles from an input file specified by fileName. Functions reads
 * the input file line-by-line using the format:
 *
 *    BusinessName X.XX Y.YY R.RR A
 *
 * BusinessName is the name of the restaurant/business. The business name will not include any
 * whitespace characters
 *
 * X.XX represents the X location in miles using a Cartesian coodinate system
 *
 * Y.YY represents the Y location in miles using a Cartesian coodinate system
 *
 * R.RR represents the average rating for the business
 *
 * A is the advertising level, which should 0, 1, or 2
 *
 * Alpha submission: ReadSporkDataFromFile() function should initialize isNearby and isGood
 *                   to true and initialize distMiles to 0.0 for all profile entries.
 *                   Note: This requirement is different for the final project submission.
 *
 * Project submission: ReadSporkDataFromFile() function should initialize isNearby and isGood
 *                     to false for all profile entries.
 *                     Note: This requirement is different for the alpha project submission.
 *
 * Returns: The number of Sprok profiles read from the file. If the input file could not be opened,
 *          the function returns -1.
 *
 */

int ReadSporkDataFromFile(SporkProfile sporkProfiles[], int maxProfiles,
                          char *fileName);

/**************************************************************************************************/

/*
 * Determines if each business is nearby the user, sets the Spork profile's isNearby flag to
 * true (is nearby) or false (not nearby), and stores the distance in miles in the Spork profile.
 *
 * userLocX, userLocY: Indicates the x and y coordiante of the user's location
 * maxDist: Indicates the maxmimum distance between the user and a nearby business 
 *
 */
void FindNearbyBusinesses(SporkProfile sporkProfiles[], int numProfiles,
                        double userLocX, double userLocY, double maxDist);


/**************************************************************************************************/

/*
 * Determines if each business is good based on the user's minimum required average rating.
 * Sets the Spork profile's isGood flag to true if the business' avgRating is greater than or 
 * equal to minRating, and false otherwise.
 *
 */
void FindGoodBusinesses(SporkProfile sporkProfiles[], int numProfiles,
                        double minRating);

/**************************************************************************************************/

/*
 * Returns the index of the Spork profile that is neary, good, and has the highest adLevel. If 
 * there is a tie, the index of the first entry found with the highest ad level should be returned.
 * If no businesses are nearby and good, the function will return -1.
 *
 */

int GetIndexMaxSponsor(SporkProfile sporkProfiles[], int numProfiles);

/**************************************************************************************************/

/*
 * Writes all good and nearby business to an output file specified by fileName using the format:
 *
 *    BusinessName R.RR D.DD
 *
 * R.RR is the average rating with exactly two decimal digits of precision.
 * D.DD is the distance in miles with exactly two decimal digits of precision.
 *
 * If maxSponsorIndex is not -1, the cooresponding profile entry will be output first. All other
 * nearby and good profiles will be output in the order they are stored in the sporkProfiles array.
 *
 * Returns: -1 if the output file could not be opened, and 0 otherwise.
 *
 */
 int WriteSporkResultsToFile(SporkProfile sporkProfiles[], int numProfiles,
                            int maxSponsorIndex, char *fileName);

/**************************************************************************************************/

#endif

/**************************************************************************************************/
