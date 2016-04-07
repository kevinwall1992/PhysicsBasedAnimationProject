#ifndef __PBA_MEMORY_FUNCTION__
#define __PBA_MEMORY_FUNCTION__

#include "FVector.h"

#include <cstdarg>

template<int d>
int GetProduct(int (&a)[d])
{
	int product= a[0];
	for(int i= 1; i< d; i++)
		product*= d;

	return product;
}

template<>
int GetProduct<1>(int (&a)[1])
{
	return a[0];
}

template<>
int GetProduct<2>(int (&a)[2])
{
	return a[0]* a[1];
}

template<>
int GetProduct<3>(int (&a)[3])
{
	return a[0]* a[1]* a[2];
}

template<>
int GetProduct<4>(int (&a)[4])
{
	return a[0]* a[1]* a[2]* a[3];
}

template<int d>
int GetProduct(int (&resolution)[d], int count)//count must be > 0
{
	int product= resolution[0];
	for(int i= 1; i< count; i++)
		product*= d;

	return product;
}


template<int d>
struct FlattenIndexStruct 
{
	using shorter_t = int(&)[d-1];

	static int FlattenIndex(int (&index)[d], int (&resolution)[d])
	{
		return index[d- 1]* GetProduct(resolution)+ FlattenIndexStruct<d-1, resolution>::FlattenIndex(reinterpret_cast<shorter_t>(index), reinterpret_cast<shorter_t>(resolution));
	}
};

template<>
struct FlattenIndexStruct<2>
{
	static int FlattenIndex(int (&index)[2], int (&resolution)[2])
	{
		return index[1]* resolution[0]+ index[0];
	}
};

template<>
struct FlattenIndexStruct<0>
{
	static int FlattenIndex(...)
	{
		return 0;
	}
};

template<int d>
int FlattenIndex(int (&index)[d], int (&resolution)[d])//adding specialization for low orders might be good
{
	return FlattenIndexStruct<d>::FlattenIndex(index, resolution);
}

template<int d>//nice if we could make this recursive
void UnflattenIndex(int flattened_index, int (&index)[d], int (&resolution)[d])
{
	index[0]= flattened_index% resolution[0];

	for(int i= 1; i< d; i++)
		index[i]= (flattened_index/ GetProduct(resolution, i))% resolution[i];
}


const float delta= 0.000001f;

template<typename T, int d>
class LookupTable
{
	FVector<float, d> low;
	FVector<float, d> high;
	FVector<float, d> range;

	FVector<int, d> resolution;
	FVector<float, d> float_resolution;
	FVector<float, d> max_vector;

	T *table;//test out having this be float pointers. Takes 2x memory but then we don't have to compute values unless needed

	FVector<float, d> GenerateInputVector(int flattened_index)
	{
		int index[d];
		UnflattenIndex<d>(flattened_index, index, resolution.v);

		FVector<float, d> input_vector;
		for(int i= 0; i< d; i++)
			input_vector[i]= index[i]+ 0.5f;
		input_vector= ((input_vector/ float_resolution)* range)+ low;

		return input_vector;
	}

public:
	typedef T OracleFunction(float[d]);

	LookupTable(OracleFunction Oracle, FVector<float, d> low, FVector<float, d> high, FVector<int, d> resolution)
	{
		this->low= low;
		this->high= high;
		this->range= high- low;
		for(int i= 0; i< d; i++)
			if(range[i]< delta)
				range[i]= delta;
		this->resolution= resolution;
		for(int i= 0; i< d; i++)
			this->float_resolution[i]= (float)resolution[i];


		int max_index[d];
		for(int i= 0; i< d; i++)
		{
			max_index[i]= resolution[i]- 1;
			max_vector.v[i]= max_index[i]- 0.000001f;
		}

		int flattened_max_index= FlattenIndex<d>(max_index, this->resolution.v);

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
		input_vector= ((input_vector- low)/ range)* max_vector;

		int index[d];
		for(int i= 0; i< d; i++)
			index[i]= (int)input_vector[i];

		return table[FlattenIndex<d>(index, resolution.v)];
	}
	
};

//template<typename F, int d>
//LookupFunction(F f, LookupDecription<d> lookup_description, ...);

#endif