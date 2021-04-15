#pragma once
#include "Core/Core.h"
#include "Core/VException.h"

#ifdef PLATFORM_WINDOWS
#include "Platform/Windows/Win.h"

namespace Engine 
{
	class WindowException : public VException
	{
	public:
		WindowException(int line, const char* file, HRESULT hr);
		virtual const char* what() const override;
		virtual const char* GetType() const override;
		static std::string TranslateErrorCode(HRESULT hr);
		HRESULT GetErrorCode() const;
		std::string GetErrorString() const;
	private:
		HRESULT hr;

	};
}

#define HWND_EXCEPT(hr) WindowException(__LINE__, __FILE__, hr)
#define HWND_LAST_EXCEPT() WindowException(__LINE__, __FILE__, GetLastError())

#endif // PLATFORM_WINDOWS
