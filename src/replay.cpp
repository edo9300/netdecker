#include "replay.h"
#include <algorithm>
#include <fstream>
#include "lzma/LzmaLib.h"

Replay::Replay(): yrp(nullptr), data_position(0){
}
Replay::~Replay() {
}
bool Replay::OpenReplayFromBuffer(std::vector<uint8_t> contents) {
	memcpy(&pheader, contents.data(), sizeof(pheader));
	if(pheader.id != REPLAY_YRP1 && pheader.id != REPLAY_YRPX) {
		return false;
	}
	contents.erase(contents.begin(), contents.begin() + sizeof(pheader));
	comp_size = contents.size();
	if(pheader.flag & REPLAY_COMPRESSED) {
		replay_size = pheader.datasize;
		replay_data.resize(replay_size);
		if(LzmaUncompress(replay_data.data(), &replay_size, contents.data(), &comp_size, pheader.props, 5) != SZ_OK)
			return false;
	} else {
		replay_data = contents;
		replay_size = comp_size;
	}
	data_position = 0;
	can_read = true;
	ParseNames();
	ParseParams();
	if(pheader.id == REPLAY_YRP1) {
		ParseDecks();
	} else {
		ParseStream();
	}
	return true;
}
bool Replay::ReadNextPacket(ReplayPacket* packet) {
	if(!can_read)
		return false;
	unsigned char message = (unsigned char)Read<int8_t>();
	if(!can_read)
		return false;
	packet->message = message;
	int len = Read<int32_t>();
	if(!can_read || len == -1)
		return false;
	packet->data.resize(len);
	ReadData((char*)packet->data.data(), packet->data.size());
	return true;
}
void Replay::ParseStream() {
	if(pheader.id != REPLAY_YRPX)
		return;
	ReplayPacket p;
	while(ReadNextPacket(&p)) {
		if(p.message == 231 && !yrp) {
			yrp = std::make_unique<Replay>();
			yrp->OpenReplayFromBuffer(p.data);
			continue;
		}
	}
}
std::vector<std::wstring> Replay::GetPlayerNames() {
	return players;
}
ReplayDeckList Replay::GetPlayerDecks() {
	if(pheader.id == REPLAY_YRPX && yrp)
		return yrp->decks;
	return decks;
}
void Replay::ParseNames() {
	players.clear();
	if(pheader.flag & REPLAY_SINGLE_MODE) {
		wchar_t namebuf[20];
		ReadName(namebuf);
		players.push_back(namebuf);
		ReadName(namebuf);
		players.push_back(namebuf);
		home_count = 1;
		opposing_count = 1;
		return;
	}
	auto f = [this](size_t& count) {
		if(pheader.flag & REPLAY_NEWREPLAY)
			count = Read<size_t>();
		else if(pheader.flag & REPLAY_TAG)
			count = 2;
		else if(pheader.flag & REPLAY_RELAY)
			count = 3;
		else
			count = 1;
		for(int i = 0; i < count; i++) {
			wchar_t namebuf[20];
			ReadName(namebuf);
			players.push_back(namebuf);
		}
	};
	f(home_count);
	f(opposing_count);
}
void Replay::ParseParams() {
	params = { 0 };
	if(pheader.id == REPLAY_YRP1) {
		params.start_lp = Read<int32_t>();
		params.start_hand = Read<int32_t>();
		params.draw_count = Read<int32_t>();
	}
	params.duel_flags = Read<int32_t>();
	if(pheader.flag & REPLAY_SINGLE_MODE && pheader.id == REPLAY_YRP1) {
		size_t slen = Read<uint16_t>();
		/*scriptname.resize(slen);
		ReadData(&scriptname[0], slen);*/
	}
}
void Replay::ParseDecks() {
	decks.clear();
	if(pheader.id != REPLAY_YRP1 || pheader.flag & REPLAY_SINGLE_MODE)
		return;
	for(int i = 0; i < home_count + opposing_count; i++) {
		ReplayDeck tmp;
		int main = Read<int32_t>();
		for(int i = 0; i < main; ++i)
			tmp.main_deck.push_back(Read<int32_t>());
		std::vector<int> extra_deck;
		int extra = Read<int32_t>();
		for(int i = 0; i < extra; ++i)
			tmp.extra_deck.push_back(Read<int32_t>());
		decks.push_back(tmp);
	}
}
bool Replay::ReadName(wchar_t* data) {
	if(!can_read)
		return false;
	unsigned short buffer[20];
	if(!ReadData(buffer, 40))
		return false;
	memcpy((char*)data, (char*)buffer, 40);
	data[19] = 0;
	return true;
}
int Replay::GetPlayersCount(int side) {
	if(side == 0)
		return home_count;
	return opposing_count;
}
bool Replay::ReadData(void* data, unsigned int length) {
	if(!can_read)
		return false;
	if((replay_data.size() - data_position) < length) {
		can_read = false;
		return false;
	}
	if(length)
		memcpy(data, &replay_data[data_position], length);
	data_position += length;
	return true;
}
template<typename T>
T Replay::Read() {
	T ret = 0;
	if(!ReadData(&ret, sizeof(T)))
		return -1;
	return ret;
}
