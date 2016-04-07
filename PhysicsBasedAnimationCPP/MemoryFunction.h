#ifndef __PBA_MEMORY_FUNCTION__
#define __PBA_MEMORY_FUNCTION__

#include "FVector.h"

#include <cstdarg>

template<int d, int resolution>
struct Dumb 
{
	static int FlattenIndex(int (&index)[d])
	{
		using shorter_t = int(&)[d-1];

		//int index_[d- 1];
		//memcpy(index_, index, sizeof(int)* (d- 1));
		int foo= index[d- 1]* (int)pow(resolution, d- 1);
		shorter_t bleep= reinterpret_cast<shorter_t>(index);
		int bar= Dumb<d-1, resolution>::FlattenIndex(bleep);
		return foo+ bar;
	}
};

template<int resolution>
struct Dumb<0, resolution>
{
	static int FlattenIndex(...)
	{
		return 0;
	}
};

template<int d, int resolution>
int FlattenIndex(int (&index)[d])//adding specialization for low orders might be good
{
	return Dumb<d, resolution>::FlattenIndex(index);
}

template<int d, int resolution>//nice if we could make this recursive
void UnflattenIndex(int flattened_index, int (&index)[d])
{
	for(int i= 0; i< d; i++)
		index[i]= (flattened_index/ (int)pow(resolution, i))% resolution;
}


const float delta= 0.000001f;

template<typename T, int d, int resolution>
class LookupTable
{
	FVector<float, d> low;
	FVector<float, d> high;
	FVector<float, d> range;

	T *table;//test out having this be float pointers. Takes 2x memory but then we don't have to compute values unless needed

	FVector<float, d> GenerateInputVector(int flattened_index)
	{
		int index[d];
		UnflattenIndex<d, resolution>(flattened_index, index);

		FVector<float, d> input_vector;
		for(int i= 0; i< d; i++)
			input_vector[i]= index[i]+ 0.5f;
		input_vector= ((input_vector/ resolution)* range)+ low;

		return input_vector;
	}

public:
	typedef T OracleFunction(float[d]);

	LookupTable(OracleFunction Oracle, FVector<float, d> low, FVector<float, d> high)
	{
		this->low= low;
		this->high= high;
		this->range= high- low;
		for(int i= 0; i< d; i++)
			if(range[i]< delta)
				range[i]= delta;


		int max_index[d];
		std::fill_n(max_index, d, resolution- 1);
		int flattened_max_index= FlattenIndex<d, resolution>(max_index);

		table= new T[flattened_max_index+ 1];
		for(int i= 0; i< flattened_max_index; i++)
			table[i]= Oracle(GenerateInputVector(i).v);
	}

	~LookupTable()
	{
		if(table!= nullptr)
			delete table;
	}

	T Lookup(FVector<float, d> input_vector)
	{
		input_vector= ((input_vector- low)/ range)* (resolution- (1+ delta));//maybe move this up into the loop

		int index[d];
		for(int i= 0; i< d; i++)
			index[i]= (int)input_vector[i];

		return table[FlattenIndex<d, resolution>(index)];
	}
	
};

//template<typename F, int d>
//LookupFunction(F f, LookupDecription<d> lookup_description, ...);

#endif