/**
 *  YGOpen Netdecker
 *  Copyright (C) 2019 Edoardo Lolletti
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "utils.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace Utils {
	bool IsFolder(const path_string & path) {
#ifdef _WIN32
		return !!(GetFileAttributes(path.c_str()) & FILE_ATTRIBUTE_DIRECTORY);
#else
		struct stat s;
		return (stat(path.c_str(), &s) == 0) && !!(s.st_mode & S_IFDIR);
#endif
	}
	bool MakeDirectory(const path_string& path) {
#ifdef _WIN32
		return CreateDirectory(path.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError();
#else
		return !mkdir(&path[0], 0777) || errno == EEXIST;
#endif
	}
	bool ChangeDirectory(const path_string & path) {
#ifdef _WIN32
		return SetCurrentDirectory(path.c_str());
#else
		return chdir(path.c_str()) == 0;
#endif
	}
	path_string GetDirectory() {
#ifdef _WIN32
		TCHAR Buffer[MAX_PATH];
		if(GetCurrentDirectory(MAX_PATH, Buffer))
			return Buffer;
		return I18N("");
#else
		char Buffer[PATH_MAX];
		if(getcwd(Buffer, PATH_MAX) != NULL)
			return Buffer;
		return "";
#endif
	}
	path_string GetFileName(path_string file) {
		size_t dashpos = file.find_last_of(SEPARATOR);
		if(dashpos == path_string::npos)
			dashpos = 0;
		else
			dashpos++;
		size_t dotpos = file.find_last_of(I18N("."));
		if(dotpos == path_string::npos)
			dotpos = file.size();
		path_string name = file.substr(dashpos, dotpos - dashpos);
		return name;
	}
	void IterateFolder(const path_string& path, const std::function<void(path_string, bool)>& cb) {
#ifdef _WIN32
		WIN32_FIND_DATA fdataw;
		HANDLE fh = FindFirstFile((path + SEPARATOR + I18N("*.*")).c_str(), &fdataw);
		if(fh != INVALID_HANDLE_VALUE) {
			do {
				cb(fdataw.cFileName, !!(fdataw.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
			} while(FindNextFile(fh, &fdataw));
			FindClose(fh);
		}
#else
		DIR * dir;
		struct dirent * dirp = nullptr;
		if((dir = opendir(path.c_str())) != nullptr) {
			struct stat fileStat;
			while((dirp = readdir(dir)) != nullptr) {
				stat((path + SEPARATOR + dirp->d_name).c_str(), &fileStat);
				cb(dirp->d_name, !!S_ISDIR(fileStat.st_mode));
			}
			closedir(dir);
		}
#endif
	}
	#ifndef UNICODE
	std::string wstring_to_utf8(const std::wstring &str) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> myconv;
		return myconv.to_bytes(str);
	}
	#endif
}
