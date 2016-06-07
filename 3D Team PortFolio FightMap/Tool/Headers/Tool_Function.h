#ifndef Tool_Function_h__
#define Tool_Function_h__


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
void safe_release(T& pointer)
{
	if(NULL != pointer)
		pointer->Release();
	pointer = NULL;
}


#endif // Tool_Function_h__


