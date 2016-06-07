#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	template <typename T>
	void safe_delete(T& pointer)
	{
		if(NULL != pointer)
			delete pointer;
		pointer = NULL;
	}

	template <typename T>
	void safe_delete_array(T& pointer)
	{
		if(NULL != pointer)
			delete [] pointer;
		pointer = NULL;
	}

	template <typename T>
	unsigned long safe_release(T& pointer)
	{
		unsigned long lRefCnt = 0;

		if(NULL != pointer)
		{
			lRefCnt = pointer->Release();
			if(0 == lRefCnt)
				pointer = NULL;
		}
		return lRefCnt;
	}
}

#endif // Engine_Function_h__
