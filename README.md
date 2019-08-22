# The YGOpen Netdecker [![Build Status](https://travis-ci.org/edo9300/netdecker.svg?branch=master)](https://travis-ci.org/edo9300/netdecker)

Free/libre and open-source utility to extract YGOPro deck files from replays.

## Usage
Download binaries from [releases](https://github.com/edo9300/netdecker/releases).

Windows: `ygopen-netdecker.exe <replayname|folder> <...>`

macOS/Linux: `./ygopen-netdecker <replayname|folder> <...>`

This outputs the decks for both players into the working directory as `replayname_playerX_playername.ydk`.

On Windows, you can also drag replays and folders on top of the exe to convert them.

## Building
The YGOpen Netdecker builds with [CMake](https://cmake.org/download/) 3.12 or later and depends only on the public-domain LZMA library, 
a stripped-down version of which has been included in the repository.

```bash
mkdir -p build && cd build
cmake ..
cmake --build . --parallel 2
```

## License

Copyright (C) 2019 Edoardo Lolletti

The YGOpen Netdecker is distributed under the GNU Affero General Public License, version 3 or later.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
See [`LICENSE`](https://github.com/edo9300/netdecker/blob/master/LICENSE) for more details.

## Credits

Source code derived from [EDOPro](https://github.com/edo9300/ygopro), commit `4c098ac4df0a97e46b577f159612e9074960b44e`.
The YGOpen client is under active development [here](https://github.com/DyXel/ygopen-gui/).
