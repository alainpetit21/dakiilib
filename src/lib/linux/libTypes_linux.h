#ifdef _UNICODE
	typedef const wchar_t*	cstring;
	typedef wchar_t*		string;
	typedef wchar_t			string_char;
#else
	typedef const char*		cstring;
	typedef char*			string;
	typedef char			string_char;
#endif	// _UNICODE

