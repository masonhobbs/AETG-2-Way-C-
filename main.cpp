#include "DataHandler.h"
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;

int main() {

  srand(time(0)); // For breaking ties between multiple best options for factors, and also for ties between best candidates

  int factors;
  int options;

  int repeat = 0;
  int currentSuiteSize,
      bestSuiteSize,
      worstSuiteSize,
      s;
  double averageSuiteSize,
         exec_time;

  // DataHandler processes candidates and finds pairs. It also stores found pairs. Main is responsible for feeding test suite generation data into DataHandler.
  DataHandler * data;

  // Stores the suite data (the candidates inserted) of the best suite generated
  vector< vector<int> > bestSuiteList;

  clock_t start_time,
          end_time;

  // For writing the results for the best suite to a .txt file
  ofstream resultsOut;

  cout << "\n<><><><><><><><><><><><><><><><><><><><><><><>\n";
  cout << "\tMason Hobbs\n\tAETG Algorithm\n\tCSCE 4444\n";
  cout << "<><><><><><><><><><><><><><><><><><><><><><><>\n";
  // Receive user input specifications
  cout << "\nEnter the number of factors: ";
  cin >> factors;
  cout << "Enter the number of options for each factor: ";
  cin >> options;

  // Program loop; user can continue to create test suites of the given factors and options
  // Done because the results can vary between runs, mainly used for testing
  do {
    currentSuiteSize = 0;
    bestSuiteSize = 0;
    worstSuiteSize = 0;
    averageSuiteSize = 0.0;
    exec_time = 0.0;

  	cout << "\n\n[Beginning test suite generation...]\n\n";
	  start_time = clock();

  	// Generates 100 test suites`
  	for(s = 0; s < 100; s++) {

    	// Creates a fresh DataHandler object for each test suite.
    	data = new DataHandler(factors, options);

    	// Generate and fill a test suite, return its size
    	currentSuiteSize = data->generate_test_suite();

    	// Perform end-of-suite generation procedures. Track size of suite as well as time taken for it.
    	if(currentSuiteSize < bestSuiteSize || bestSuiteSize == 0) {
      		bestSuiteSize = currentSuiteSize;
          bestSuiteList = data->getSuite();
      		cout << ":^)! Found new ~best~ suite -> #" << s+1 << " <^> Size: " << bestSuiteSize << endl;
    	}

    	else if(currentSuiteSize > worstSuiteSize) {
      		worstSuiteSize = currentSuiteSize;
      		cout << ":^(! Found new ~worst~ suite -> #" << s + 1 << " <^> Size: " << worstSuiteSize << endl;
    	}

    	averageSuiteSize += currentSuiteSize;

    	delete data;

  	} // End 100 test-suite generator loop

    // Program ending procedures; display time, test suite sizes. Repeat program again if user desires.
  	end_time = clock();
  	exec_time = double(end_time-start_time) / CLOCKS_PER_SEC;
  	averageSuiteSize /= 100;

  	cout << "\n<Program finished in " << exec_time << " seconds | Average test suite time: " << exec_time / 100 << ">\n";
  	cout << "<Best suite: " << bestSuiteSize << " | Worst suite: " << worstSuiteSize << " | Average suite: " << averageSuiteSize << ">\n\n";
    cout << "[Run again using same factors & options?]\n\t0 = No\t1 = Yes\n >> ";
    cin >> repeat;

  }while(repeat != 0);

  // User has decided to end program; write best suite information to a file
  int i, j;
  string fileName;
  fileName += to_string(options) + "x" + to_string(factors) + ".txt";
  resultsOut.open(fileName);
  resultsOut << bestSuiteSize << "\n\n";

  for(i = 0; i < bestSuiteSize; i++) {
    sort(bestSuiteList[i].begin(), bestSuiteList[i].end());
    for(j = 0; j < factors; j++) {
      resultsOut << bestSuiteList[i][j] << " ";
    }
    resultsOut << "\n";
  }
  resultsOut.close();

  cout << "\n<Best candidate information written to " << fileName << ">\n\n";

  return 0;
}
