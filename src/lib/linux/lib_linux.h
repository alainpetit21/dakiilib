#include <memory.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include <XMLLoader.h>

extern bool System_Run(void);
extern bool System_IsActive(void);
extern void System_Quit(void);

#ifdef DLL_EXPORT
	#define EXTERN extern
	#define EXPORT
#elif defined(DLL_IMPORT)
	#define EXTERN extern
#else
	#define EXTERN extern
	#define EXPORT
#endif
