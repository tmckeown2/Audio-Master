#include "EffectsMenu.h"

// Only load the DLLs at runtime if Windows is being used as DLLs are different for different OS (linux uses shared object *.so files instead, etc.)
// This is from Charphacy's answer at:
// https://stackoverflow.com/questions/2989810/which-cross-platform-preprocessor-defines-win32-or-win32-or-win32
#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "SFX.h"
#endif

// Outputs memory leaks after the program has closed
#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif

namespace AudioMaster
{
    EffectsMenu::EffectsMenu()
    {
		std::string filePath = fs::current_path().generic_string() + "/plugins";

		// Only load the DLLs at runtime if Windows is being used as DLLs are different for different OS (linux uses shared object *.so files instead, etc.)
		// This is from Charphacy's answer at:
		// https://stackoverflow.com/questions/2989810/which-cross-platform-preprocessor-defines-win32-or-win32-or-win32
		#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)

		for (auto &file : fs::directory_iterator(filePath))
		{
			if (file.path().extension() == ".dll")
			{
				dllPaths.push_back(file.path());
				Logger::GetInstance()->Log("Found plugin: " + file.path().generic_string());
			}
		}

		typedef SFXHANDLE (__stdcall *GETSFXPROC)(void);
		HINSTANCE hInstLib;
		GETSFXPROC getSfxFunc;

		for (auto &dll : dllPaths)
		{
			// Try to load the DLL
			hInstLib = LoadLibraryW(dll.c_str());

			// Check that the library loaded successfully
			if (hInstLib != NULL)
			{
				// Try to find the address of the "GetSfx" function, without decoration
				getSfxFunc = (GETSFXPROC)GetProcAddress(hInstLib, "GetSfx");

				// If the undecorated function handle could not be found then try the decorated name
				if (getSfxFunc == NULL)
					getSfxFunc = (GETSFXPROC)GetProcAddress(hInstLib, "_GetSfx@0");

				// If the function was found and loaded correctly then attempt to get the Sfx object
				if (getSfxFunc != NULL)
				{
					SFXHANDLE sfx = getSfxFunc();

					// If the Sfx object is loaded correctly then attempt to get the name
					if (sfx != NULL)
					{
						// Try to get the name from the Sfx object and create a new menu item for it
						std::string name = sfx->GetName();

						Logger::GetInstance()->Log("Loaded plugin: " + name);

						// Map the new effect to its id so it can be used in an event.
						this->effects[name] = this->GetNextEffectID();
						this->Append(this->effects[name], name);

						// Release the Sfx object from memory
						sfx->Release();
					}
				}
			}

			// Close the DLL
			if (!FreeLibrary(hInstLib))
			{
				Logger::GetInstance()->Log("Effect library could not be freed");
			}
			else
			{
				Logger::GetInstance()->Log("Effect library successfully freed");
			}
		}

		#endif
    }

    EffectsMenu::~EffectsMenu()
    {
        Logger::GetInstance()->Log("Destroying Effects Menu");
    }

	int EffectsMenu::GetNextEffectID()
	{
		int nextId = 0;

		// Find the highest id for an effect currently
		for (auto &effect : this->effects)
		{
			if (effect.second > nextId)
				nextId = effect.second;
		}

		// Increment the id to get the new highest value
		nextId++;

		return nextId;
	}

	std::map<std::string, int> EffectsMenu::GetEffects()
	{
		return this->effects;
	}
	
	std::vector<fs::path> EffectsMenu::GetDLLs()
	{
		return this->dllPaths;
	}
}