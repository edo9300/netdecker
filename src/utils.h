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

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <functional>

#ifdef UNICODE

using path_char = wchar_t;
#define I18N(x) L##x
#define convert_string(x) x
#define tostring(x) std::to_wstring(x)

#define write_console std::wcout
#define SEPARATOR I18N("\\")

#else

#include <codecvt>
#include <locale>

using path_char = char;

#define I18N(x) x

#define convert_string(x) Utils::wstring_to_utf8(x)
#define tostring(x) std::to_string(x)

#define write_console std::cout
#define SEPARATOR "/"

#endif

using path_string = std::basic_string<path_char>;

namespace Utils {
	bool IsFolder(const path_string& path);
	bool MakeDirectory(const path_string& path);
	bool ChangeDirectory(const path_string& path);
	path_string GetDirectory();
	path_string GetFileName(const path_string& file);
	void IterateFolder(const path_string& path, const std::function<void(path_string, bool)>& cb);
#ifndef UNICODE
	std::string wstring_to_utf8(const std::wstring &str);
#endif
}

#endif //UTILS_H
