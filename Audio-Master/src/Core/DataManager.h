#ifndef INC_DATA_MANAGER_H
#define INC_DATA_MANAGER_H

namespace AudioMaster
{
	constexpr const char* DATA_DIR = "data\\";

	/// <summary>
	/// Creates a new directory at the specified path.
	/// Returns <c>true</c> if successful. Otherwise <c>false</c> is returned.
	/// </summary>
	/// <param name="path">Path to the new directory</param>
	/// <returns><c>true</c> if successful. Otherwise <c>false</c></returns>
	/// <remarks>This uses the Windows <c>CreateDirectory</c> so is not portable yet.</remarks>
	bool CreateDir(const char* path);
	/// <summary>
	/// Checks if a file/directory exists.
	/// Returns <c>true</c> if it exists. Otherwise <c>false</c> is returned
	/// </summary>
	/// <param name="path">Path to the file/directory</param>
	/// <returns><c>true</c> if successful. Otherwise <c>false</c></returns>
	bool Exists(const char* path);
	/// <summary>
	/// Gets the path to the directory that holds the main application executable (module).
	/// </summary>
	/// <returns>The path of the module</returns>
	/// <remarks>This uses the Windows file system so is not portable yet.</remarks>
	const char* GetModuleDir();
}

#endif