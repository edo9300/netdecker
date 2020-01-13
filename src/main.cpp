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
 
#include <fstream>
#include <string>
#include <vector>

#include "replay.h"
#include "utils.h"

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

void ParseReplay(const path_string & path) {
	std::ifstream replay_file(path, std::ifstream::binary);
	if(!replay_file.is_open()) {
		return;
	}
	std::vector<uint8_t> contents((std::istreambuf_iterator<char>(replay_file)),
								  std::istreambuf_iterator<char>());
	replay_file.close();
	Replay replay(contents);
	auto players = replay.GetPlayerNames();
	if(players.empty()) {
		return;
	}
	auto decks = replay.GetPlayerDecks();
	if(players.size() > decks.size()) {
		return;
	}
	path_string filename = Utils::GetFileName(path);
	for(int i = 0; i < decks.size(); i++) {
#define SAVE(name) SaveDeck(filename + I18N("_") + I18N("player") + tostring(i) + I18N("_") + name + I18N(".ydk"), decks[i].main_deck, decks[i].extra_deck)
		if(!SAVE(convert_string(players[i]))) {
			SAVE(I18N("")); // if the player name is not allowed in a file
		}
#undef SAVE
	}
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
		path_string file = argv[j];
		if(Utils::IsFolder(file)) {
			auto filename = Utils::GetFileName(file);
			if(Utils::MakeDirectory(filename + I18N("_extracted"))) {
				std::vector<path_string> files;
				Utils::IterateFolder(file, [&files](path_string name, bool isdir) {
					if(!isdir) {
						files.push_back(name);
					}
				});
				auto prev_dir = Utils::GetDirectory();
				Utils::ChangeDirectory(filename + I18N("_extracted"));
				for(auto& replay : files) {
					ParseReplay(file + SEPARATOR + replay);
				}
				Utils::ChangeDirectory(prev_dir);
			}
		} else {
			ParseReplay(file);
		}
	}
	return EXIT_SUCCESS;
}
