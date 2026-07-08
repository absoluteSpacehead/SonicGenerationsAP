namespace Utils
{
	template<class T>
	T* Offset(LPCWSTR lpModuleName, uintptr_t offset)
	{
		return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(GetModuleHandle(lpModuleName)) + offset);
	}
}