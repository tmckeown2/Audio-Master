#ifndef INC_ISOUNDWRAPPER_H
#define INC_ISOUNDWRAPPER_H

#include "..\Utils\Sound.h"

namespace AudioMaster
{
	class ISoundWrapper
	{
	public:
		virtual Sound* Import(const char* filename) = 0;
		virtual void   Export(const char* filename, Sound* data) = 0;
	};
}

#endif