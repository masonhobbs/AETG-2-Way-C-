#include "DataHandler.h"

/* Initializes all variables as well as our "found" 2d array */
DataHandler::DataHandler(int f, int o) {
  factors = f;
  options = o;
  totalOptions = factors * options;
  totalPairs = calc_max_pairs(factors, options);
  foundPairs = 0;
  suiteSize = 0;

  // Initialize our 2d vector with ints; inefficient space-wise, but turns all our options into a grid representing pairs formed
  int i;
  for(i = 0; i < totalOptions; i++)
    found.push_back(vector<int>(factors * options, 0));

}

/* Evaluates whether or not we've found all possible pairs from our data set. If so, we're done, if not, keep generating candidates */
bool DataHandler::allPairsFound() {
  if(foundPairs == totalPairs)
    return true;
  return false;
}


/* Prints out which pairs have been found so far, along with the possible max pairs
 * for each option and how many we've discovered so far */
void DataHandler::printAllFound() {
  int i, j;
  cout << "\n<><><><><><><><><><><><><><><><><><><><><>\n    |";
  for(i = 0; i < totalOptions; i++) {
    cout << i << " ";
  }
  cout << "\n-------------------------------\n";
  for(i = 0; i < totalOptions; i++) {
    cout << "<" << i << "> |";
    for(j = 0; j < totalOptions; j++) {
      if(found[i][j] == 1)
        cout << found[i][j] << " ";
      else
        cout << "  ";
    }
    cout << "\n" << endl;
  }

}

/* Calculates the number of unique 2-way pairs possible from a given amount
   of factors and options. */
int DataHandler::calc_max_pairs(int factors, int options) {
  int i, j;
  int result = 0;

  for(i = 1; i < factors; i++)
    result += (totalOptions - (i * options)) * options;

  return result;
}

/* For a given set of options (a test candidate), it will find the number of pairs it contains */
int DataHandler::findPairs(vector<int>& combination, int length) {
  int i, j;
  int pairsGenerated = 0;

  for(i = 0; i < length; i++) {
    for(j = i+1; j < length+1; j++) {
        pairsGenerated += 1 - found[combination[i]][combination[j]];
    }
  }
  return pairsGenerated;
}

/* Returns, for a given element/option, how many undiscovered possible pairs it has left */
int DataHandler::pairsLeft(int element) {
  int i;
  int undiscoveredPairs = 0;

  // i = element + options - (element % options) is the starting position of the options that the given input, element, can start forming pairs with.
  for(i = element + options - (element % options); i < found[element].size(); i++) {
    if(found[element][i] == 0)
      undiscoveredPairs++;
  }

  return undiscoveredPairs;
}

/* Generates the entire test suite for this object. While we haven't found all possible pairs, continue to generate a best candidate for insertion.
 * After generation, insert the best one into our matrix of pair information. Returns the size of the test suite. */
int DataHandler::generate_test_suite() {
  vector<int> best_candidate;

  while(!allPairsFound()) {
    suiteSize++;

    // Generates the best candidate out of 50 generated ones and stores it in best_candidate.
    best_candidate = generate_candidate();
    insert_candidate(best_candidate);

  }
  return suiteSize;
}

// Generates 50 candidates and returns the best one.
vector<int> DataHandler::generate_candidate() {

  vector<int> best_candidate(factors, 0); // Stores the best candidate generated
  vector<int> test_candidate(factors, 0); // Stores the candidate we're currently testing.
  vector<int> factor_order(factors, 0);   // Stores the factors we're using, 1 through factors

  int i;
  int j;
  int k;
  int currentPairs, bestPairs, bestOption, range, tieBreaker,
      currentCandidatePairs;
  int bestCandidatePairs = -1;

  // Initialize factor starting information
  for(i = 1; i <= factors; i++)
      factor_order[i-1] = i;

  // If this is not the first candidate in our suite, greedily pick options. Otherwise, we generate an entirely random first candidate to insert.
  if(suiteSize != 1) {
    for(i = 0; i < 50; i++) {
      // Shuffles factor order to be random per candidate generated
      random_shuffle(&factor_order[0], &factor_order[factors-1]);

      // Reset test candidate each time
      test_candidate = vector<int>(factors, 0);

      // Generate an option for every factor in a test candidate
      for(j = 0; j < factor_order.size(); j++) {
        currentPairs = 0;
        bestPairs = -1;
        bestOption = 0;
        range = factor_order[j] * options - options; // Determines, index wise, where options are for this factor.

        // Try all options for this given factor to find the best one
        for(k = 0; k < options; k++) {
              // Check how many uncovered pairs this option has, if it is the only option so far. Necessary because we can't form any pairs
              // with just one option, so all attempted checks for pairs will return 0, so we will just check what pairs it hasn't formed yet
              if(j == 0) {
                currentPairs = pairsLeft(range+k);
              }

              // If we have other options to test with this factor, start checking the whole test candidate
              else {
                test_candidate[j] = range+k;
                currentPairs = findPairs(test_candidate, j);
              }

              // If current option(s) selected's pair amount is equal to the best known pair amount, randomly choose one
              if(currentPairs == bestPairs) {
                  if(rand() > RAND_MAX / 2) {
                    bestPairs = currentPairs;
                    bestOption = range+k;
                  }
              }

              // If current option(s) selected is better than best known pairs, update our records
              if(currentPairs > bestPairs) {
                bestPairs = currentPairs;
                bestOption = range+k;
              }
          }

          // Put in our best option found
          test_candidate[j] = bestOption;
      }

      // Compare this candidate's undiscoved pairs it produces against best known undiscovered pairs by a candidate
      currentCandidatePairs = findPairs(test_candidate, factors-1);

      // If the two candidates are equal, randomly pick one
      if(currentCandidatePairs == bestCandidatePairs) {
        if(rand() > RAND_MAX / 2) {
          bestCandidatePairs = currentCandidatePairs;
          best_candidate.swap(test_candidate);
        }
      }

      // If our current candidate is better than best known one, store it as the best
      else if(currentCandidatePairs > bestCandidatePairs) {
        bestCandidatePairs = currentCandidatePairs;
        best_candidate.swap(test_candidate);
      }
    }
  }

  // If this is the first slot for our suite, generate an entirely random candidate
  else {
    random_shuffle(&factor_order[0], &factor_order[factors-1]);
    for(i = 0; i < factors; i++) {
      range = factor_order[i] * options - options;
      best_candidate[i] = (factor_order[i] * options - options) + (rand() % options);
    }
  }

  return best_candidate;
}

/* For use in inserting our best candidate discovered in generate_candidate, marks off all pairs it covers */
void DataHandler::insert_candidate(vector<int>& candidate) {
  int i, j;

  for(i = 0; i < factors-1; i++) {
    for(j = i+1; j < factors; j++) {
      if(found[candidate[i]][candidate[j]] == 0 ) {
        found[candidate[i]][candidate[j]] = 1;
        found[candidate[j]][candidate[i]] = 1;
        foundPairs++;
      }
    }
  }
  suite.push_back(candidate);
}

/* Returns the suite generated */
vector< vector<int> > DataHandler::getSuite() {
  return suite;
}
