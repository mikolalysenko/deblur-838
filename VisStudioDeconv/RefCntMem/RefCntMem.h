#ifndef Ref_Cnt_Mem_h
#define Ref_Cnt_Mem_h

template <typename T>
class RefCntMem            // reference-counted memory object class
{

//	friend class RefCntMem;

private:

	struct CountedPtr			// counted pointer
	{
		T *_data;				// allocated data
		int _count;				// reference count
		CountedPtr() { _data = 0; _count = 0; }
	};
	CountedPtr *_cptr;

    void DecrementCount(void)  // decrement the reference count and delete if done
	{		
		if (_cptr)
		{
			_cptr->_count--;
			if (_cptr->_count == 0)
			{
				delete[] _cptr->_data; 
				delete _cptr;
				_cptr = 0;
			}
		}
	}
    void IncrementCount(void)  // increment the reference count
	{
		if (_cptr) _cptr->_count++;
	}
    
public:
	RefCntMem(void)                   // default constructor
	{ 
		_cptr = 0; 
	}

	RefCntMem(size_t n)
	{
		_cptr = 0;
		ReAllocate(n);
	}

    RefCntMem(const RefCntMem <T>& rcm)   // copy constructor
	{	
		_cptr = 0;    
		(*this) = rcm;  
	}

    ~RefCntMem(void)				  // destructor
	{
		DecrementCount();
		_cptr = 0;      
	}

    RefCntMem& operator=(const RefCntMem<T>& rcm)  // assignment
	{
		DecrementCount();   
		_cptr = rcm._cptr;
		IncrementCount();
		return *this;
	}

    void ResetCount(T *data)        // start count new memory
	{
		DecrementCount();
		if (data)
		{
			_cptr = new CountedPtr;
			_cptr->_data = data;
			_cptr->_count = 1;			
		}
		else
			_cptr = 0; 		
	}

	void ReAllocate(size_t n)
	{
		ResetCount(new T [n]);
	}

	T* Memory(void)							// pointer to allocated memory
	{
		return (_cptr) ? _cptr->_data : 0;
	}
	
	operator const T* () const 
	{
		return Memory();
	}

	operator T* ()  
	{
		return Memory();
	}
};

#endif // Ref_Cnt_Mem_h
