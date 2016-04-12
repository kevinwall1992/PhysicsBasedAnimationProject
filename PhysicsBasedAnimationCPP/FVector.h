#ifndef __PBA_FVECTOR__
#define __PBA_FVECTOR__

template<class T, int d>
struct FVector
{
	T v[d];//need to test whether having const size better
	//int *link_count;

	/*FVector()
	{
		v= new T[d];
		link_count= new int(1);
	}

	FVector(const FVector<T, d> &source)
    {
        v = source.v;
		link_count= source.link_count;
		*link_count++;
    }

	~FVector()
	{
		*link_count--;
		if(link_count== 0)
		{
			delete v;
			delete link_count;
		}
	}*/

	FVector()
	{

	}



	/*void TakeValueOf(const FVector<T, d> &b)
	{
		for(int i= 0; i< d; i++)
			*this[i]= b[i];
	}*/

	/*FVector<T, d> & operator=(const FVector<T, d> &b) 
	{
		for(int i= 0; i< d; i++)
			(*this)[i]= b[i];

		return *this;
	}*/

	T & operator[](const int index)
	{
		return v[index];
	}

	const T & operator[](const int index) const
	{
		return v[index];
	}

	FVector<T, d> operator+(const FVector<T, d> &b) const
	{
		FVector<T, d> c;

		for(int i= 0; i< d; i++)
			c.v[i]= this->v[i]+ b.v[i];

		return c;
	}

	FVector<T, d> & operator+=(const FVector<T, d> &b)
	{
		for(int i= 0; i< d; i++)
			this->v[i]= (T)(this->v[i]+ b.v[i]);

		return *this;
	}

	FVector<T, d> operator-(const FVector<T, d> &b) const
	{
		FVector<T, d> c;

		for(int i= 0; i< d; i++)
			c.v[i]= this->v[i]- b.v[i];

		return c;
	}

	FVector<T, d> & operator-=(const FVector<T, d> &b)
	{
		for(int i= 0; i< d; i++)
			this->v[i]= (T)(this->v[i]- b.v[i]);

		return *this;
	}

	FVector<T, d> operator*(const FVector<T, d> &b) const
	{
		FVector<T, d> c;

		for(int i= 0; i< d; i++)
			c.v[i]= this->v[i]* b.v[i];

		return c;
	}

	template<class U>
	FVector<T, d> operator*(const U &s) const
	{
		FVector<T, d> c;

		for(int i= 0; i< d; i++)
			c.v[i]= (T)(this->v[i]* s);

		return c;
	}

	template<class U>
	FVector<T, d> & operator*=(const U &s)
	{
		for(int i= 0; i< d; i++)
			this->v[i]= (T)(this->v[i]* s);

		return *this;
	}

	FVector<T, d> operator/(const FVector<T, d> &b) const
	{
		FVector<T, d> c;

		for(int i= 0; i< d; i++)
			c.v[i]= this->v[i]/ b.v[i];

		return c;
	}

	template<class U>
	FVector<T, d> operator/(const U &s) const
	{
		FVector<T, d> c;

		for(int i= 0; i< d; i++)
			c.v[i]= (T)(this->v[i]/ s);

		return c;
	}

	template<class U>
	FVector<T, d> & operator/=(const U &s)
	{
		for(int i= 0; i< d; i++)
			this->v[i]= (T)(this->v[i]/ s);

		return *this;
	}

	float SquaredMagnitude()
	{
		float sum= 0;
		for(int i= 0; i< d; i++)
			sum+= (float)(this->v[i]* this->v[i]);

		return sum;
	}

	float Magnitude()
	{
		return sqrt(SquaredMagnitude());
	}

	float Distance(const FVector<T, d> &b)
	{
		return (*this- b).Magnitude();
	}

	float SquaredDistance(const FVector<T, d> &b)
	{
		return (*this- b).SquaredMagnitude();
	}

	FVector<T, d> Normalized()
	{
		return *this/ Magnitude();
	}

	void Normalize()
	{
		*this/= Magnitude();
		
	}

	FVector<T, d> Absolute()
	{
		FVector<T, d> c;

		for(int i= 0; i< d; i++)
			c.v[i]= (T)abs(this->v[i]);
	}

	void Absolutize()
	{
		for(int i= 0; i< d; i++)
			this->v[i]= (T)abs(this->v[i]);
	}

	void ZeroOut()
	{
		for(int i= 0; i< d; i++)
			this->v[i]= (T)0;
	}
};

typedef FVector<float, 2> FVector2f;
typedef FVector<int, 2> FVector2i;


FVector2f MakeFVector2f(float f0, float f1);
FVector2i MakeFVector2i(int i0, int i1);

#endif