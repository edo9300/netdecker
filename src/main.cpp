#include <fstream>
#include <string>
#include <vector>

#include "replay.h"

#if defined(UNICODE)

using path_char = wchar_t;
#define I18N(x) L##x
#define convert_string(x) x
#define tostring(x) std::to_wstring(x)

#else

#include <codecvt>
#include <locale>

using path_char = char;

#define I18N(x) x

std::string wstring_to_utf8(const std::wstring &str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> myconv;
    return myconv.to_bytes(str);
}

#define convert_string(x) wstring_to_utf8(x)
#define tostring(x) std::to_string(x)

#endif

using path_string = std::basic_string<path_char>;

bool SaveDeck(const path_string &name, std::vector<int> mainlist,
	      std::vector<int> extralist) {
    std::ofstream deckfile(name, std::ofstream::out);
    if (!deckfile.is_open()) {
		return false;
	}
    deckfile << "#main\n";
    for (auto card : mainlist) {
		deckfile << std::to_string(card) << "\n";
	}
    deckfile << "#extra\n";
    for (auto card : extralist) {
		deckfile << std::to_string(card) << "\n";
	}
    deckfile.close();
    return true;
}

path_string GetFilename(path_string path) {
	size_t dashpos = path.find_last_of(I18N("\\/"));
	if(dashpos == path_string::npos)
		dashpos = 0;
	else
		dashpos++;
	size_t dotpos = path.find_last_of(I18N("."));
	if(dotpos == path_string::npos)
		dotpos = path.size();
	path_string name = path.substr(dashpos, dotpos - dashpos);
	return name;
}

#if defined(UNICODE)
int wmain(int argc, wchar_t *argv[]) {
#else
int main(int argc, char *argv[]) {
#endif
    if (argc < 2) {
		return EXIT_FAILURE;
	}
	for(int j = 1; j < argc; j++) {
		std::ifstream replay_file(argv[j], std::ifstream::binary);
		if(!replay_file.is_open()) {
			return EXIT_FAILURE;
		}
		std::vector<uint8_t> contents((std::istreambuf_iterator<char>(replay_file)),
									  std::istreambuf_iterator<char>());
		replay_file.close();
		Replay replay(contents);
		auto players = replay.GetPlayerNames();
		if(players.empty()) {
			return EXIT_FAILURE;
		}
		auto decks = replay.GetPlayerDecks();
		if(players.size() > decks.size()) {
			return EXIT_FAILURE;
		}
		path_string filename = GetFilename(argv[j]);
		for(int i = 0; i < decks.size(); i++) {
			SaveDeck(filename + I18N("_") + I18N("player") + tostring(i) + I18N("_") + convert_string(players[i]) + I18N(".ydk"),
					 decks[i].main_deck, decks[i].extra_deck);
		}
	}
	return EXIT_SUCCESS;
}
