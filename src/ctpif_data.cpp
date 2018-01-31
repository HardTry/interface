// check memory leak
#ifdef _WINDOWS
#ifdef _DEBUG_
#ifndef DBG_NEW
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#endif
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif // _DEBUG_
#endif //_WINDOWS

#include "ctpif_data.h"

