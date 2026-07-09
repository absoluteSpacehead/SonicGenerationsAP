namespace Utils
{
	template<class T>
	T* Offset(LPCWSTR lpModuleName, uint32_t offset)
	{
		return reinterpret_cast<T*>(reinterpret_cast<uint32_t>(GetModuleHandle(lpModuleName)) + offset);
	}
}