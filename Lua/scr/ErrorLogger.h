#pragma once
#include <Windows.h>
#include <comdef.h>
#include <string>
#include <algorithm>

#define COM_ERROR_IF_FAILED( hr, msg ) if( FAILED( hr ) ) throw COMException( hr, msg, __FILE__, __FUNCTION__, __LINE__ )
 std::wstring StringToWide(std::string str);
 std::string GetFileExtension(const std::string& filename);

class COMException
{
public:
	COMException(HRESULT hr, const std::string& msg, const std::string& file, const std::string& function, int line)
	{
		_com_error error(hr);
		whatmsg = L"Msg: " + StringToWide(std::string(msg)) + L"\n";
		whatmsg += error.ErrorMessage();
		whatmsg += L"\nFile: " + StringToWide(file);
		whatmsg += L"\nFunction: " + StringToWide(function);
		whatmsg += L"\nLine: " + StringToWide(std::to_string(line));
	}

	const wchar_t* what() const
	{
		return whatmsg.c_str();
	}
private:
	std::wstring whatmsg;
};

class ErrorLogger
{
public:
	static void Log(std::string message);
	static void Log(HRESULT hr, std::string message);
	static void Log(HRESULT hr, std::wstring message);
	static void Log(COMException& exception);
};
