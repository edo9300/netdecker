#ifndef REPLAY_H
#define REPLAY_H

#include <ctime>
#include <vector>
#include <fstream>
#include <cstdint>
#include <memory>
#include <cstring>

#define REPLAY_COMPRESSED	0x1
#define REPLAY_TAG			0x2
#define REPLAY_DECODED		0x4
#define REPLAY_SINGLE_MODE	0x8
#define REPLAY_LUA64		0x10
#define REPLAY_NEWREPLAY	0x20
#define REPLAY_RELAY		0x40

#define REPLAY_YRP1			0x31707279
#define REPLAY_YRPX			0x58707279

struct ReplayHeader {
	unsigned int id;
	unsigned int version;
	unsigned int flag;
	unsigned int seed;
	unsigned int datasize;
	unsigned int hash;
	unsigned char props[8];
};


class ReplayPacket {
public:
	ReplayPacket() {}
	int message;
	std::vector<unsigned char> data;
};

struct ReplayDeck {
	std::vector<int> main_deck, extra_deck;
};

using ReplayDeckList = std::vector<ReplayDeck>;

class Replay {
public:
	Replay();
	Replay(const std::vector<uint8_t>& contents):data_position(0) {
		OpenReplayFromBuffer(contents);
	};
	~Replay();
	bool OpenReplayFromBuffer(std::vector<uint8_t> contents);
	std::vector<std::wstring> GetPlayerNames();
	ReplayDeckList GetPlayerDecks();
	int GetPlayersCount(int side);
	std::unique_ptr<Replay> yrp;
	ReplayHeader pheader;
	std::vector<uint8_t> replay_data;
	std::vector<uint8_t> comp_data;
	size_t replay_size;
	size_t comp_size;
	struct duel_parameters {
		int start_lp;
		int start_hand;
		int draw_count;
		int duel_flags;
	};
	duel_parameters params;
private:
	bool ReadData(void* data, unsigned int length);
	bool ReadNextPacket(ReplayPacket* packet);
	size_t data_position;
	bool can_read;
	template <typename  T>
	T Read();
	bool ReadName(wchar_t* data);
	void ParseNames();
	void ParseParams();
	void ParseDecks();
	void ParseStream();
	std::vector<std::wstring> players;
	size_t home_count;
	size_t opposing_count;
	ReplayDeckList decks;
};

#endif
