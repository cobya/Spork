/**************************************************************************************************/

/*
 * File: sporkprofile.c
 * Author: Coby Allred
 * NetID: cobyallred
 * Date: 9/15/16
 *
 * Description: Function definitions for reading, processing, and writing Spork profiles.
 *
 */

/**************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sporkprofile.h"

/**************************************************************************************************/

/* Reads up to maxProfiles Spork profiles from an input file specified by fileName. Functions reads
 * the input file line-by-line using the format:
 *
 *    BusinessName X.XX Y.YY R.RR A
 *
 * BusinessName is the name of the restauarant/business. The buseiness name will not include any
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
 * Returns: The number of Spork profiles read from the file. If the input file could not be opened,
 *          the function returns -1.
 *
 */

int ReadSporkDataFromFile(SporkProfile sporkProfiles[], int maxProfiles, char *fileName) {
	// Initialize variables used for analyzing string and incrementing number of profiles
	int currentProfile = 0;
	int argRead = 0;
	char holdText[255];
	char afterString[10];

	// Open the file for reading
	FILE* sporkInput = NULL;
	sporkInput = fopen(fileName, "r");
	
	if (sporkInput == NULL) { 
		// If error reading file, return -1 for error in main.c
		return -1;
	} else { 
		// If no error, continue
		while (!feof(sporkInput) && currentProfile < maxProfiles) { // Continue reading while file has not ended and maxProfiles not reached
			fgets(holdText, 255, sporkInput); // Scan in line to be analyzed

			// Review the line and assign data, assign # of arguments to argRead in case of a wrong # of inputs
			argRead = sscanf(holdText, "%100s %lf %lf %lf %d %10s", sporkProfiles[currentProfile].businessName, &sporkProfiles[currentProfile].locX, &sporkProfiles[currentProfile].locY, &sporkProfiles[currentProfile].avgRating, &sporkProfiles[currentProfile].adLevel, afterString); 
			
			if (argRead != 5 || (sporkProfiles[currentProfile].adLevel != 0 && sporkProfiles[currentProfile].adLevel != 1 && sporkProfiles[currentProfile].adLevel != 2)) {
				// If inputs are wrong, do nothing, don't increment so data is overwritten with next line
			} else { 
				// If inputs are correct, assign default values, distMiles and increments
				sporkProfiles[currentProfile].isGood = false; 
				sporkProfiles[currentProfile].isNearby = false;
				sporkProfiles[currentProfile].distMiles = 0.0;
				++currentProfile;
			}
		}
	}

	fclose(sporkInput); // Close the input file
	
	return currentProfile; // Return the number of profiles

}

/**************************************************************************************************/

/*
 * Determines if each business is nearby the user,  sets the Spork profile's isNearby flag to
 * true (is nearby) or false (not nearby), and stores the distance in miles in the Spork profile.
 *
 * userLocX, userLocY: Indicates the x and y coordiante of the user's location
 * maxDist: Indicates the maxmimum distance between the user and a nearby business
 *
 */

void FindNearbyBusinesses(SporkProfile sporkProfiles[], int numProfiles, double userLocX, double userLocY, double maxDist) {
	int i;

	for (i = 0; i < numProfiles; i++) { // Incrementing through profiles
		double distAway = sqrt(pow(userLocX - sporkProfiles[i].locX, 2) + pow(userLocY - sporkProfiles[i].locY, 2)); // Calculating distance from user

		if (distAway <= maxDist) { // If dist < maxDist, set isNearby to true for that profile
			sporkProfiles[i].isNearby = true;
			sporkProfiles[i].distMiles = distAway;
		}
	}
	
	return;

}


/**************************************************************************************************/

/*
 * Determines if each business is good based on the user's minimum required average rating.
 * Sets the Spork profile's isGood flag to true if the business' avgRating is greater than or
 * equal to minRating, and false otherwise.
 *
 */

void FindGoodBusinesses(SporkProfile sporkProfiles[], int numProfiles, double minRating) {
	int i;

	for (i = 0; i < numProfiles; i++) { // Incrementing through profiles, if avgRating is >= minRating, set isGood to true
		if (sporkProfiles[i].avgRating >= minRating) {
			sporkProfiles[i].isGood = true;
		}
	}

	return;

}

/**************************************************************************************************/

/*
 * Returns the index of the Spork profile that is nearby, good, and has the highest adLevel. If
 * there is a tie, the index of the first entry found with the highest ad level should be returned.
 * If no businesses are nearby and good, the function will return -1.
 *
 */

int GetIndexMaxSponsor(SporkProfile sporkProfiles[], int numProfiles) {
	// Initialize variables used to analyze company adLevel
	int numGood = 0;
	int maxIndex = 0;
	int currentMaxAd = 0;
	int i;
	
	for (i = 0; i < numProfiles; i++) { // Incrementing through profiles
		if (sporkProfiles[i].isGood == true && sporkProfiles[i].isNearby == true) { // If isGood and isNearby true, compare adLevel to previous high
			++numGood; // Increase numGood businesses
			
			if (sporkProfiles[i].adLevel > currentMaxAd) { // If new adLevel > previous, set maxIndex to current index
				currentMaxAd = sporkProfiles[i].adLevel;
				maxIndex = i;
			}
		}
	}

	if (numGood == 0) { // If no good businesses, return -1
		return -1;
	} else { // Else return maxIndex
		return maxIndex;
	}
	
}

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

int WriteSporkResultsToFile(SporkProfile sporkProfiles[], int numProfiles, int maxSponsorIndex, char *fileName) {
	// Open the file
	FILE* sporkOut = NULL;
	sporkOut = fopen(fileName, "w");
	
	if (sporkOut == NULL) { // If error reading file, return -1
		return -1;
	} else { // Else continue
		if (maxSponsorIndex != -1 ) { // Print maxSponsorIndex first if there is one
			fprintf(sporkOut, "%s\t%.2lf\t%.2lf\n", sporkProfiles[maxSponsorIndex].businessName, sporkProfiles[maxSponsorIndex].avgRating, sporkProfiles[maxSponsorIndex].distMiles);
		}
		int i;

		for (i = 0; i < numProfiles; i++) { // Increment through profiles and print list
			if (i != maxSponsorIndex && sporkProfiles[i].isGood == true && sporkProfiles[i].isNearby == true) { // Print rest of list when index is not maxSponsorIndex
				fprintf(sporkOut, "%s\t%.2lf\t%.2lf\n", sporkProfiles[i].businessName, sporkProfiles[i].avgRating, sporkProfiles[i].distMiles);
			}
		}
	}

	fclose(sporkOut); // Close output file

	return 0;
}

/**************************************************************************************************/
