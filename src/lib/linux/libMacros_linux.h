
#undef _T
#ifndef _UNICODE
  #define _T
#else
  #define _T	L
#endif


#define TRACE1(s)				{printf(s);}
#define TRACE2(s, p1)			{printf(s, p1);}
#define TRACE3(s, p1, p2)		{printf(s, p1, p2);}
#define TRACE4(s, p1, p2, p3)	{printf(s, p1, p2, p3);}


#undef	ERROR_MESSAGE
#define ERROR_MESSAGE(s)			{puts(s);}

#undef	ASSERT2
#ifdef _DEBUG
#define ASSERT2(a, s)			{if(!(a)){printf(s);}}
#else
	#define ASSERT2(a, s)			{}
#endif	// DEBUG

#undef	ASSERT3
#ifdef _DEBUG
#define ASSERT3(a, s, p1)			{if(!(a)){printf(s, p1);}}
#else
	#define ASSERT3(a, s, p1)			{}
#endif	// DEBUG

#undef	VERIFY2
#ifdef _DEBUG
	#define VERIFY2(a, s)			{if(!(a)){printf(s);}}
#else
	#define VERIFY2(a, s)			a;
#endif	// DEBUG

#undef	VERIFY3
#ifdef _DEBUG
	#define VERIFY3(a, s, p1)			{if(!(a)){printf(s, p1);}}
#else
	#define VERIFY3(a, s, p1)			a;
#endif	// DEBUG

#undef	WARNING2
#ifdef _DEBUG
#define WARNING2(a, s)			{if(!(a)){printf(s);}}
#else
	#define WARNING2(a, s)			{}
#endif	// DEBUG

#undef	TRAP_MSG
#define TRAP_MSG()

#define ENTRY_POINT(gameName)															\
extern int LibMain(const char *p_strGame);												\
int																						\
main(int argc, char* argvp[])															\
{																						\
	g_TheApp= newGameApp();																\
	return LibMain(gameName);															\
}

#undef	PROFILER_INC
#undef	PROFILER_DEC
#undef	PROFILER_INIT
#undef	PROFILER_BEFORE
#undef	PROFILER_AFTER
#define PROFILER_INC
#define PROFILER_DEC

#define PROFILER_INIT

#define PROFILER_BEFORE

#define PROFILER_AFTER(string, returnLine)

static inline int
min(int a, int b)
{
	return (a < b)? a : b;
}

static inline int
max(int a, int b)
{
	return (a > b)? a : b;
}
