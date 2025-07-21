#ifndef UTILS_H
#define UTILS_H

#include <string>

using namespace std;

/**
 * Utility class providing helper functions like
 * screen clearing, pausing, and getting date/time.
 */
class Utils {
public:
    // Clears the console screen (platform-independent)
    static void clearScreen();

    // Prompts the user to press Enter to continue
    static void pressEnterToContinue();

    // Returns a formatted string of the current date and time
    static string getCurrentDateTime();

    // Displays a welcome banner/message
    static void displayWelcomeMessage();
};

#endif // UTILS_H
