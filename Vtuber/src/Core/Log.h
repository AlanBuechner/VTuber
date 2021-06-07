#pragma once
#include "Core.h"
#include <sstream>

#ifdef PLATFORM_WINDOWS

#include "Platform/Windows/Win.h"

#define DBOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

#endif // PLATFORM_WINDOWS