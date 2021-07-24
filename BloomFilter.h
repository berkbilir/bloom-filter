#ifndef _BLOOMFILTER_H
#define _BLOOMFILTER_H

//Header guards to protect from infinte loops 

#include <iostream>
#include <string>

//BERK BILIR 23621

using namespace std;

template<class filterType, class elementType>
class BloomFilter
{
	public: 
	//Public Functions to use in bloom.cpp
		BloomFilter(int, int, int* , int* );
		void addElement(elementType);
		bool checkExist(elementType);
		void printFilter();
		void resetFilter(); //Resets the filter by initliasing every element to 0
		int bitPosition(int&); //Return element related to the bit
		int hash(string, int, int* , int* );
		
	private:
	//Private variables
		filterType* elementArr; //To store elements we need dyn arrays
		int totalBits, k;
		double bitSize;
		int * as, * bs;		
};

#endif