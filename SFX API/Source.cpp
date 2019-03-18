// Include SFX.h
// Load DLL using Windows
// Try and use the GetSfx - might have to export the decorated name and use that
// See if multiple can be read

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include "SFX.h"

//namespace fs = std::filesystem;			  // C++17
namespace fs = std::experimental::filesystem; // C++14

using namespace std;

int main()
{
	vector<fs::path> dllPaths;
	string filePath = "D:/Uni/3rd Year Project/SFXAPI/SFX API/SFX From Folder/Plugins/";

	for (const auto &entry : fs::directory_iterator(filePath))
	{
		auto path = entry.path();

		string extension = path.extension().generic_string();

		if (extension == ".dll")
		{
			dllPaths.push_back(path);
		}
	}

	cout << "Number of DLLs in \"Plugins\" folder: " << dllPaths.size() << endl;

	// SFXHANDLE __stdcall GetSfx(void)
	typedef SFXHANDLE(__stdcall *GETSFXPROC)(void);
	HINSTANCE hInstLib;
	GETSFXPROC procGet;
	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;

	for (auto &dll : dllPaths)
	{
		// Try to load the DLL
		hInstLib = LoadLibrary(TEXT("D:\\Uni\\3rd Year Project\\SFXAPI\\SFX API\\SFX From Folder\\Plugins\\SFX API.dll"));

		// Try to run the GetSfx function
		if (hInstLib != NULL)
		{
			procGet = (GETSFXPROC)GetProcAddress(hInstLib, "GetSfx");

			if (procGet == NULL)
				procGet = (GETSFXPROC)GetProcAddress(hInstLib, "_GetSfx@0");

			if (procGet != NULL)
			{
				fRunTimeLinkSuccess = TRUE;
				SFXHANDLE sfx = procGet();

				// Try to run the GetName function on the Sfx object
				string name = sfx->GetName();

				// Try to run the DoSfx function on the Sfx object
				//sfx->DoSFX(...);

				// Release the Sfx object
				sfx->Release();
			}

			// Free the DLL module
			fFreeResult = FreeLibrary(hInstLib);
		}

		if (!fRunTimeLinkSuccess)
			cout << "DLL \"" << dll.c_str() << "\" could not be loaded!" << endl;
	}
}