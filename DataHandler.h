#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

class DataHandler {
  private:
    int factors;
    int options;
    int totalOptions;
    int totalPairs;
    int foundPairs;
    int suiteSize;

    vector< vector<int> > found; // 2d int vector grid. Keeps track of each table element's pairs and whether or not they've been found yet
    vector< vector<int> > suite; // Stores the candidates inserted into the suite; sent to main if this object is the best suite generated

   public:
    DataHandler() { }
    DataHandler(int factors, int options);
    bool allPairsFound(); // Checks if the amount of pairs we've found is equal to the max possible pairs; returns true if so, false if not.
    void printAllFound(); // Prints our grid of found values, used for verifying visually if all pairs were found
    int calc_max_pairs(int factors, int options); // Calculates the number of possible pairs from a given amount of factors and options for each */
    int findPairs(vector<int>& combination, int length);    // Returns how many pairs a combination of options form, but doesn't mark them as found
    int pairsLeft(int option);  // Checks, for a single given option, how many possible pairs it has undiscovered
    int generate_test_suite();	// Generate and fill an entire test suite that covers all pairs
    vector<int> generate_candidate();  // Generate 50 potential candidates and returns the best one.
    void insert_candidate(vector<int>& candidate); // Takes the best candidate, inserts it, and marks off the pairs it produces as found.
    vector< vector<int> > getSuite();  // Returns the suite generated

};
