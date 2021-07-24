#include <iostream>
#include <random>
#include "BloomFilter.h"
#include "BloomFilter.cpp"
using namespace std;

//BERK BILIR 23621


int main() {
  
  //a hash function is simply in the form h(x) = aX + b mod bitCount where X is obtained by traversing the characters of x
  //we fix the number of hash functions to two but in general a BloomFilter uses an optimal number of hash functions
  int as[2], bs[2];
  as[0] = 47903; bs[0] = 77081; //constants for the first hash function
  as[1] = 49019; bs[1] = 27409; //constants for the second hash function
  
  //The total number bits in the filter
  //int bitcount = 48; //6 chars, 3 shorts, 2 int, 1 long longs
  //int bitcount = 96; //12 chars, 6 shorts, 3 int, 2 long longs
  //int bitcount = 128; //16 chars, 8 shorts, 4 int, 2 long longs
  int bitcount;
  
  cout << "Enter the number of bits: " << endl;
  cin >> bitcount;

  //This prints 1 2 4 8: we will try only the following types to store the bits of the Bloom Filter
  cout << sizeof(char) << " " << 
    sizeof(short) << " " << 
    sizeof(int) << " " << 
    sizeof(long long) << endl; 

  //creating bloom filters with different types and 2 hash functions with constants as and bs
  //   the first type is the type of the objects used to store the bits of the filter
  //   the second type is the type of the objects we store in the filter
  BloomFilter<int , string> int_string_filter(bitcount, 2, as, bs); //--we are using this one in the sample main below. 
                                                              //--but we will test your code with other types
	 
  //element can have different types so you need to also use the type of the element 
  //as a template type
  string element; //-- note that we are using a string filter
  //char element;
  //int element;

  //Interactive test - checks if index computations are correct
  bool cont = true;
  while (cont) {
    cout << endl;
    cout << "*************************************************"<<endl
	 << "***** 0 - ADD ELEMENT                        ****"<<endl
	 << "***** 1 - TEST ELEMENT                       ****"<<endl	
	 << "***** 2 - PRINT FILTER                       ****"<<endl
	 << "***** 3 - EXIT                               ****"<<endl
	 << "*************************************************"<<endl;
    cout << endl;
    int option;
    bool existInFilter;
    cout << "Pick an option from above (int number from 0 to 3): ";
    cin >> option;
    switch (option) 
      {
      case 0:
	cout << "Enter the element you want to add to the bloom filter: ";
	cin >> element;
	int_string_filter.addElement(element);
	break;
      case 1:
	cout << "Enter the element you want to test if it exists in bloom filter: ";
	cin >> element;
	existInFilter = int_string_filter.checkExist(element);
	if(existInFilter) {
	  cout << "The element is in the filter with high probability" << endl;
	} else {
	  cout << "The element is not in the filter" << endl;
	}
	break;
      case 2:
	int_string_filter.printFilter();
	break;
      case 3:
	cout << "Interactive test exiting ... " << endl << endl << endl;
	cont = false;
	break;
      default:
	cout << "Invalid option!!! Try again" << endl;
      }//switch
  }//while (true)

  //Noninteractive test - checks if the accuracy is OK for elements chosen from 2^20 elements from 2^24 possible elements
  const int n = (1 << 13); //number of elements inserted to the filter 
  const int M = (1 << 17); //number of possible elements
  bitcount = 1 << 12; 
  
  cout << "Non-interactive test with parameters: " << endl << 
    "\tBF size: " << bitcount << " " << endl << 
    "\tNo possible items: " << M << " " << endl << 
    "\tNo items inserted: " << n << endl;

  BloomFilter<int, int> int_int_filter(bitcount, 2, as, bs);
  BloomFilter<short, double> short_double_filter(bitcount, 2, as, bs);

  //basic random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, M);

  //create n random integers 
  int ints[n];
  for(int i = 0; i < n; i++) {
    ints[i] = dis(gen);
  }

  //insert the elements to the filters
  for(int i = 0; i < n; i++) {
    int_int_filter.addElement(ints[i]);
    double d_elm = ((double)ints[i]) / M;
    short_double_filter.addElement(d_elm);
  }

  //check the accuracy: first create an array of size M to check which elements are inserted
  bool marks[M];
  for(int i = 0; i < M; i++) {
    marks[i] = false;
  }  
  for(int i = 0; i < n; i++) {
    marks[ints[i]] = true;
  }
  //now mark is true if and only if the elements are insterted to the filters

  //check the accuracy of the int_int_filter
  int falsePositive = 0, truePositive = 0, falseNegative = 0, trueNegative = 0;
  for(int i = 0; i < M; i++) {
    if(marks[i]) { //the correct answer is positive
      if(int_int_filter.checkExist(i)) {
	truePositive++;
      } else {
	falseNegative++; //this must be always 0 at the end
      } 
    } else { //the correct answer is negative
      if(!int_int_filter.checkExist(i)) {
	falsePositive++;
      }else {
	trueNegative++; //all negatives are true negatives
      }
    }
  }
  cout << endl;
  cout << "Stats for the int_int_filter " << endl 
       << "\tNo true positive: " << truePositive << endl 
       << "\tNo true negative: " << trueNegative << endl  
       << "\tNo false positive: " << falsePositive << endl 
       << "\tNo false negative: " << falseNegative << endl 
       << "\tTrue positive rate: " << ((double)truePositive) / (truePositive + falseNegative) << endl 
       << "\tTrue negative rate: " << ((double)trueNegative) / (trueNegative + falsePositive) << endl
       << "\tPositive answer correctness probability: " << ((double)truePositive) / (truePositive + falsePositive) << endl 
       << "\tNegative answer correctness probability: " << ((double)trueNegative) / (trueNegative + falseNegative) << endl;

  //check the accuracy of the short_double_ilter
  falsePositive = 0, truePositive = 0, falseNegative = 0, trueNegative = 0;
  for(int i = 0; i < M; i++) {
    double d_elm = ((double)i) / M;
    if(marks[i]) { //the correct answer is positive
      if(short_double_filter.checkExist(d_elm)) {
	truePositive++;
      } else {
	falseNegative++;
      }
    } else { //the correct answer is negative
      if(!short_double_filter.checkExist(d_elm)) {
	falsePositive++;
      } else {
	trueNegative++;
      }
    }
  }
  cout << endl;
  cout << "Stats for the short_double_ilter " << endl 
       << "\tNo true positive: " << truePositive << endl 
       << "\tNo true negative: " << trueNegative << endl  
       << "\tNo false positive: " << falsePositive << endl 
       << "\tNo false negative: " << falseNegative << endl 
       << "\tTrue positive rate: " << ((double)truePositive) / (truePositive + falseNegative) << endl 
       << "\tTrue negative rate: " << ((double)trueNegative) / (trueNegative + falsePositive) << endl
       << "\tPositive answer correctness probability: " << ((double)truePositive) / (truePositive + falsePositive) << endl 
       << "\tNegative answer correctness probability: " << ((double)trueNegative) / (trueNegative + falseNegative) << endl;

  cin.ignore();
  cin.get();
  return 0;
}
