#include <iostream>   
#include <string>
#include <sstream>

#include "BloomFilter.h"

//BERK BILIR 23621

using namespace std;

//To use in several member functions, decleration at top to use
template<class elementType>
string to_str(const elementType& obj) {
	 ostringstream oss;
	 oss << obj;
	 return oss.str();
}

template<class filterType, class elementType>
BloomFilter<filterType, elementType>::BloomFilter(int totalBits, int k, int* as, int* bs){

	//initliaze elements
	this -> totalBits = totalBits;
	this -> k = k;
	this -> as = as;
	this -> bs = bs;
	this -> bitSize = 8 * sizeof(filterType); //Keep track of bitsize

	//We need to find how many elements we will use in the array by dividing input to size of filterType and rounding it up
	elementArr = new filterType [ ceil(totalBits / bitSize) ];

	resetFilter();
}
	
template<class filterType, class elementType>
void BloomFilter<filterType, elementType>::resetFilter(){
	//Loop and set each element to zero
	for (int i = 0; i < ceil( totalBits / bitSize ); i++)
	{
		elementArr [i] = 0; 
	}
}

template<class filterType, class elementType>
int BloomFilter<filterType, elementType>::hash(string x_str, int index, int* as, int* bs) {
	 unsigned int hash = 0;
	 for (int i = 0; i < x_str.length(); i++) {
	 hash ^= (x_str[i] * 899809363);
	 }
	 hash = (as[index] * hash) + bs[index];
	 hash = hash % totalBits; //getsize() replaced by real size
	return hash;
}

template<class filterType, class elementType>
int BloomFilter<filterType, elementType>::bitPosition(int& bitPosition){

	int elementFound = bitPosition / bitSize;
	//Find the elements position by seeing its bit
	return elementFound;

}

template<class filterType, class elementType>
void BloomFilter<filterType, elementType>::addElement(elementType element){
		//Find and modify the filter
		int bitSize = 8 * sizeof(filterType);
		for (int i = 0; i < k; i++){
				int elementPos = hash(to_str(element)  , i, as, bs);
				int arrayPos = bitPosition(elementPos);
				elementPos = elementPos % int(bitSize); //For elements that have indexes more than 0
				//Change the position by finding the index and modyfing it
				* (elementArr + arrayPos) |= 1ULL << elementPos;
	}
}

template<class filterType, class elementType>
bool BloomFilter<filterType, elementType>::checkExist(elementType element){
	
	bool check = true;
	//Find and check the filter
		for (int i = 0; i < k; i++){
			//Get the element to check
			int elementPos =  hash(to_str(element)  , i, as, bs);
			int arrayPos = bitPosition(elementPos);
			elementPos = elementPos % int(bitSize); //For elements that have indexes more than 0
			//substr since position are 1 indexed
			if (!((*(elementArr + arrayPos)>> elementPos) & 1ULL )) //if nth bit set or not
			{
				 // if the bit is 0
				check = false;
				return check;
			}
		}
	return check;
}

template<class filterType, class elementType>
void BloomFilter<filterType, elementType>::printFilter(){
	int bitCounter = totalBits;
	//We need to loop through every element of filter while we have not yet met the bit count
	for (int i = 0; i < ceil(totalBits / bitSize); i++)
	{
		for (int j = 0; j < bitSize; j++)
		{ 
			if(bitCounter > 0)
		     {  
				 cout << (( 1ULL & *(elementArr + i) >> j) ); //Print the bit out
				 bitCounter --; 
			}
		}
	}
}