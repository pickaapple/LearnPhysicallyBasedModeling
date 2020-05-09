#pragma once
template<class T>
class auto_release
{
public:
	explicit auto_release(T* ptr = nullptr) noexcept
		:_Myptr(ptr) {}
	~auto_release()
	{
		if (_Myptr) _Myptr->Release();
	}
	T& operator*() const noexcept
	{
		return *_Myptr;
	}

	T** operator&() noexcept
	{
		return &_Myptr;
	}

	T* operator->() const noexcept
	{
		return _Myptr;
	}

	T* get() noexcept
	{
		return _Myptr;
	}
	T* release() noexcept
	{
		T* temp = _Myptr;
		_Myptr = nullptr;
		return temp;
	}
	void reset(T* ptr)
	{
		if (_Myptr != ptr) {
			if (_Myptr) _Myptr->Release();
			_Myptr = ptr;
		}
	}
private:
	T* _Myptr;
};

template<class T>
void ScaleVector(T* vector, size_t length, T scale)
{
	for (size_t i = 0; i < length; ++i)
		*(vector++) *= scale;
}

template<class T>
void AddVector(T* src1, T* src2, T* dst, size_t length)
{
	for (size_t i = 0; i < length; ++i)
		*(dst++) = *(src1++) + *(src2++);
}


extern float FLOAT_EPSILON;

bool NotEqual(float a, float b);
bool Equal(float a, float b);
bool LT(float a, float b);
bool LET(float a, float b);
bool MT(float a, float b);
bool MET(float a, float b);
