/*****************************************************************************
 *	Name:     Devon Mack                                                     *
 *	Course:   ICS4U                                                          *  
 *	Date:     May 2017                                                       *
 *	                                                                         * 
 *	Purpose: Read from .csv files into structure                             *
 *	                                                                         *
 *	Usage:   Download health.csv								    		 *						
 *  https://think.cs.vt.edu/corgis/csv/healths/healths.html                  *
 *	                                                                         *
 *	Revision History:                                                        *  
 *	                                                                         *
 *	Known Issues:                                                            *
 *	                                                                         *  
 *****************************************************************************/
#include <iostream>
#include <fstream> 
#include <vector>
#include <string>
#include <cstdlib>
#include <pthread.h>
#include <ctime>
#include "timer.h"
#include "timer.cpp"

using namespace std;

struct Health {
	string disease;
	float increase;
	string loc;
	int number;
	int population;
	int year;
	float to_sort;
	string to_sort_str;
};

int moves = 0; // Counts number of moves that a sort takes to run
int comparisons = 0; // Counts number of comparisons

void printLine(Health e);
void printLineFile(ofstream &fout, Health e);
void insertionSort (vector<Health> &arr);
void insertionSortString (vector<Health> &arr);
void merge(vector<Health> &arr, int start, int mid, int end);
void mergeSort(vector<Health> &arr, int start, int end);
void swap(vector<Health> &arr, int val1, int val2);
void quickSort(vector<Health> &arr, int front, int end);
void readNext(ifstream &f, int &i, bool endline);
void readNext(ifstream &f, float &s, bool endline);
void readNext(ifstream &f, string &s, bool endline);
void printSortedBy(Health h, string type);

int main() {
	vector<Health> health;
	Health e;
	ifstream fin("health.csv");
	if (!fin) {
		cout << "File not found" << endl;
		return 404;
	}
	// header row, throw out
	string value;
	char ch;
	getline(fin, value);
	int count = 0;
	while (fin.good()) {
		count++;
		readNext(fin, e.disease, false);
		readNext(fin, e.increase, false);
		readNext(fin, e.loc, false);
		readNext(fin, e.number, false);
		readNext(fin, e.population, false);
        readNext(fin, e.year, true); // last item read to end of line, rather than comma

		if (fin.good())  {
			health.push_back(e);
		}
	}

	fin.close();

	cout << "Number of data elements to sort: " << health.size() << endl;
//	cout << "Unsorted..." << endl;
//	for (int i = 0; i < 6; i++) {
//		printLine(health[i]);
//	}
	cout << endl;

    cout << "Please enter what you would like to sort by. Options are:" << endl << "disease, increase, location, number,"
			" population, and year." << endl;

    string input;
    bool type_is_string = false;
    while (true) {
        getline(cin, input);
        if (input == "disease") {
            type_is_string = true;
            for (int i = 0; i < health.size(); i++) {
                health[i].to_sort_str = health[i].disease;
            }
            break;
        } else if (input == "increase") {
            for (int i = 0; i < health.size(); i++) {
                health[i].to_sort = health[i].increase;
            }
            break;
        } else if (input == "location") {
            type_is_string = true;
            for (int i = 0; i < health.size(); i++) {
                health[i].to_sort_str = health[i].loc;
            }
            break;
        } else if (input == "number") {
            for (int i = 0; i < health.size(); i++) {
                health[i].to_sort = health[i].number;
            }
            break;
        } else if (input == "population") {
            for (int i = 0; i < health.size(); i++) {
                health[i].to_sort = health[i].population;
            }
            break;
        } else if (input == "year") {
            cout << "WARNING: The base data is already sorted by year!" << endl;
            for (int i = 0; i < health.size(); i++) {
                health[i].to_sort = health[i].year;
            }
            break;
        } else {
            cout << "Invalid input!" << endl;
        }
    }

    Timer t;
    comparisons = 0;
    moves = 0;
    vector<Health> unsorted = health;

    if (!type_is_string) {
        int type_of_sort;
        cout << "Please input the type of sort you would like to do:" << endl << "1: Quick Sort" << endl << "2: Merge Sort"
		     << endl << "3: Insertion Sort" << endl << "4: All" << endl;
        while (true) {
            cin >> type_of_sort;
            if (type_of_sort >= 1 && type_of_sort <= 4) {
                break;
            } else {
                cout << "Invalid input!" << endl;
            }
        }
        cout << endl;
		cout << "Sorting by " << input << endl;
        /* --- Start sorting --- */
        if (type_of_sort == 1 || type_of_sort == 4) { // Quicksort
            t.start();
            quickSort(health, 0, health.size()); //working
            t.stop();
            cout << "QuickSorted in " << t.currtime() << " seconds, " << moves << " moves, and " << comparisons
                 << " comparisons." << endl;
            for (int i = 0; i < 6; i++) {
                printLine(health[i]);
            }
            cout << endl;
        }

        if (type_of_sort == 2 || type_of_sort == 4) { // Mergesort
            health = unsorted;
            t.reset();
            comparisons = 0;
            moves = 0;

            t.start();
            mergeSort(health, 0, health.size() - 1); //working
            t.stop();
            cout << "MergeSorted in " << t.currtime() << " seconds, " << moves << " moves, and " << comparisons
                 << " comparisons." << endl;
            for (int i = 0; i < 6; i++) {
                printLine(health[i]);
            }

            cout << endl;
        }

        if (type_of_sort == 3 || type_of_sort == 4) { // Insertionsort
            health = unsorted;
            t.reset();
            comparisons = 0;
            moves = 0;

            t.start();
            insertionSort(health); //working
            t.stop();
            cout << "InsertionSorted in " << t.currtime() << " seconds, " << moves << " moves, and "
                 << comparisons << " comparisons." << endl;

            for (int i = 0; i < 6; i++) {
                printLine(health[i]);
            }
        }

    } else {
        cout << "Type is string, so will only sort using insertion sort. Please select a number if you want"
                " to compare the three types of sorting." << endl << endl;
        cout << "Sorting..." << endl;
        t.reset();
        comparisons = 0;
        moves = 0;
        t.start();
        insertionSortString(health); //working
        t.stop();
        cout << "InsertionSorted in " << t.currtime() << " seconds, "<< moves << " moves, and "
             << comparisons << " comparisons." << endl;

        for (int i = 0; i < 6; i++) {
            printLine(health[i]);
        }
    }
    cout << "-------------- Statistics --------------" << endl;
    cout << "Top 5" << endl;
    for (int i = health.size()-1; i > health.size()-6; i--) {
        cout << health[i].disease << " in " << health[i].loc << ", " << input << " ";
        printSortedBy(health[i], input);
        cout << endl;
        //printLine(health[i]);
    }
    cout << "Bottom 5" << endl;
    for (int i = 0; i <= 5; i++) {
        cout << health[i].disease << " in " << health[i].loc << ", " << input << " ";
        printSortedBy(health[i], input);

        cout << endl;
        //printLine(health[i]);
    }
    cout << "Median: " << endl;
    int mid = health.size()/2;
    cout << health[mid].disease << " in " << health[mid].loc << ", " << input << " ";
    printSortedBy(health[mid], input);
    cout << endl;
   

//  Write to file (uncomment if wanted)
//	const char* output = "output.txt";
//	ofstream fout(output);
//
//	for (int i = 0; i < health.size(); i++) {
//		printLineFile(fout, health[i]);
//	}
//	fout.close();
}


// read in integer
void readNext(ifstream &f, int &i, bool endline) {
    string value;
    if (endline) {
        getline(f, value); // to \n
    } else {
        getline(f, value, ',' );
    }

    i = atoi(value.c_str());
}

// read in float
void readNext(ifstream &f, float &s, bool endline) {
    string value;
    if (endline) {
        getline(f, value); // to \n
    } else {
        getline(f, value, ',' );
    }
    s = atof(value.c_str());
}

// read in string, check for quotes,
void readNext(ifstream &f, string &s, bool endline) {
    string s1;
    if (endline) {
        getline(f, s); // to \n
    } else {
        getline(f, s, ',' );
    }
    if (s[0] == '\"') {
        getline(f, s1, '\"' );
        s = s.substr(1, s.length()) + s1;	// strip of quotations.
    }
}

void insertionSort (vector<Health> &arr){
	// vars
	int j;
	Health temp;
		
	//For each element, find where to insert it in the sorted subarray
	for (int i = 1; i < arr.size(); i++) {
		// j is the index of the element we are trying to place in the array
		j = i;
		
		// Loop through each element all the way back to the start of the array.
		while (j > 0 && arr[j-1].to_sort > arr[j].to_sort) {
            // Switch elements j and j-1, moving arr[j] down the array since
            // it's bigger then the element below it
            comparisons++;
            swap(arr, j, j-1);
            j--;
        }
        comparisons++;
    }
}

void insertionSortString (vector<Health> &arr){
	// vars
	int j;
	Health temp;
		
	//For each element, find where to insert it in the sorted subarray
	for (int i = 1; i < arr.size(); i++) {
		// j is the index of the element we are trying to place in the array
		j = i;
		
		// Loop through each element all the way back to the start of the array.
		while (j > 0 && arr[j-1].to_sort_str > arr[j].to_sort_str) {
            // Switch elements j and j-1, moving arr[j] down the array since
            // it's bigger then the element below it
            comparisons++;
            swap(arr, j, j-1);
            j--;
        }
        comparisons++;
    }
}

void merge(vector<Health> &arr, int start, int mid, int end) {
    int arr_1_size = mid - start + 1;
    int arr_2_size =  end - mid;
 	
    // Temporary array
    vector<Health> arr_1(arr_1_size);
	vector<Health> arr_2(arr_2_size);
    // Copy data into the temporary arrays (this uses a lot of moves/time)
    // start to middle
    int i;
    for (i = 0; i < arr_1_size; i++) {
    	arr_1[i] = arr[start + i];
    	moves++;
	}
    //middle to end
    int j;
    for (j = 0; j < arr_2_size; j++) {
    	arr_2[j] = arr[mid + j + 1];
    	moves++;
	}

    // Merge the temporary arrays together
    i = 0; // array 1 counter
    j = 0; // array 2 counter
    int k = start; // merged array counter (only sorting the slice of the full array)
    while (i < arr_1_size && j < arr_2_size) { // While both arrays still have element
        // Compare element from each array and put the bigger element into the merged array
        // increment the merged array counter
        comparisons++;
        moves++;
		if (arr_1[i].to_sort <= arr_2[j].to_sort) {
            arr[k] = arr_1[i];
            i++;
        } else {
            arr[k] = arr_2[j];

            j++;
        }
        k++;
    }
 	// Copy remaining elements of array 1 into the merged array
    while (i < arr_1_size) {
        arr[k] = arr_1[i];
        moves++;
        i++;
        k++;
    }
 	// Copy remaining elements of array 2 into the merged array
    while (j < arr_2_size) {
        arr[k] = arr_2[j];
        moves++;
        j++;
        k++;
    }

}

void mergeSort(vector<Health> &arr, int start, int end) {
    if (start < end) { // Until the subsection is only 1 element long
    	int mid = (start+end)/2;
        // Split it into two halves
        // start to middle

		mergeSort(arr, start, mid);
		//middle to end

        mergeSort(arr, mid+1, end);
        // merge the two halves that were just sorted
        merge(arr, start, mid, end);
    }
}

void swap(vector<Health> &arr, int val1, int val2) {
    moves += 2;
    Health temp = arr[val2];
    arr[val2] = arr[val1];
    arr[val1] = temp;
}

void quickSort(vector<Health> &arr, int front, int end) {
	if(front < end) {
		int leftwall = front;
		// partition
		for (int i = front + 1; i < end; i++) { // The first element of the subarray is the pivot so skip over it
			comparisons++;
			if (arr[i].to_sort < arr[front].to_sort) { // If greater than pivot, swap
				leftwall++;
				swap(arr, i, leftwall);
			}
		}
		swap(arr, front, leftwall); // Move the pivot to the middle of both sides

		quickSort(arr, front, leftwall);
		quickSort(arr, leftwall + 1, end);
	}
}

void printLine(Health e) {
	cout << e.disease << ", ";
	cout << e.loc << ", ";
	cout << e.number << ", ";
    cout << e.year << ", ";
    cout << e.increase << ", ";
	cout << e.population << endl;
}
void printLineFile(ofstream &fout, Health e) {
	cout << e.disease << ", ";
	cout << e.loc << ", ";
	cout << e.number << ", ";
    cout << e.year << ", ";
    cout << e.increase << ", ";
	cout << e.population << endl;
}

void printSortedBy(Health h, string type) {
	if (type == "disease") {
            cout << h.disease;
        } else if (type == "increase") {
            cout << h.increase;
        } else if (type == "location") {
            cout << h.loc;
        } else if (type == "number") {
            cout << h.number;
        } else if (type == "population") {
            cout << h.population;
        } else if (type == "year") {
            cout << h.year;
    }
}
