Hostile Takeover for GFOS
=========================

A port of Hostile Takeover, the open source release of Spiffcode's
real-time strategy game Warfare Incorporated, from
https://github.com/spiffcode/hostile-takeover at commit
363c8f7f02c25f90255b28a5e2eaea784b2a502f (game version 1.6), for GFOS:
AmigaOS 3.2 with GFOpenUp, on a 68040 or 68060 with an FPU, on
GFOpenGPU's SDL 2.

Build a base, mine galaxite, research and send tanks, infantry and
artillery against the computer in the game's missions and challenge
levels. The game was made for handhelds, so a 68k is home ground: its data
files are big-endian, like the Amiga.

Status: written for Cradle's os32-gcc16-open6 stove; not yet run in a lab.


What you need
-------------

- GFOS: AmigaOS 3.2 with GFOpenUp and its GFOpenGPU part, which installs
  LIBS:GFOpenGPU/SDL2.module.
- A 68040 or 68060 with an FPU (not an LC040 or EC040), an RTG screen
  (the game's window is 640 x 480), about 12 MB of free memory and 4 MB
  on disk.
- Nothing else: the missions, art and sounds are in the drawer
  (htdata832.pdb, htsfx.pdb).


How to play
-----------

On GFOS: GFOpenUpTool INSTALL Parts/HostileTakeover puts the game in
SYS:Games/HostileTakeover. From the archive: run Install-HostileTakeover
(or copy the HostileTakeover drawer where you want it). Then double-click
HostileTakeover. The mouse does everything: click a unit to select it,
drag a box round several, click the ground or an enemy to send them, and
use the buttons along the bottom to build and research. The game's own
Help explains each unit and building.

Preferences, saved games and finished missions are kept in the drawer's
User drawer; the Installer keeps it on an update and GFOpenUp never
touches it.

Online play (the multiplayer lobby and downloaded mission packs) needs
HTTP, which this build has no library for yet: those menus say they
cannot connect. Everything else is there.


Building
--------

    make -f amiga/Makefile.amigaos -j4 STOVE=/path/to/os32-gcc16-open6

or, in Cradle, stove_build with the os32-gcc16-open6 stove and target
package (package/build-amiga.sh), which also runs the host tests, makes
the GFOpenUp part (Parts/HostileTakeover) and packs
HostileTakeover-VERSION-68040.lha.


What was changed
----------------

Upstream's game code is used as it is, with the sources upstream's
game/sdl/linux/makefile builds. On top:

- amiga/amigahosthelpers.cpp (instead of game/sdl/linux/hosthelpers.cpp):
  the data files come from PROGDIR:, the player's files go to PROGDIR:User
  (prefs.bin, SaveGames, Completes, MissionPacks); the game draws into a
  640 x 480 surface (800 x 600 on Linux) to save conversion work on a 68k;
  the device id is a number kept in User/udid.
- amiga/amigahttprequest.cpp (instead of game/sdl/sdlhttprequest.cpp,
  which needs libcurl): every request ends in the game's own "could not
  connect" error. amiga/curl/curl.h stands in for libcurl's header.
- amiga/net/: base/socket.cpp, socketaddress.cpp and selectserver.cpp
  built on Roadshow's bsdsocket.library (opened at start when a TCP/IP
  stack runs; without one every call fails cleanly), and
  game/sdl/host.cpp with SDL_Delay() for HostSleep().
- game/sdl/htplatform.h, mpshared/misc.h: BigWord()/BigDword() do nothing
  on the 68k, which is big-endian like the data.
  tests/test_data_endian.py checks the .pdb files are big-endian.
- game/sdl/sdlpackfile.cpp, mpshared/packinfomanager.cpp: functions that
  return a pointer return NULL, not false (GCC 16 refuses that).
- game/Multiplayer.cpp, game/gameform.cpp: include game/multiplayer.h by
  its real name (the cross compiler's file system minds the case).
- amiga/amiga_start.c: a 512 KB stack, the $VER string, PROGDIR:User.
- amiga/icons: the drawer's GlowIcons, from The AmiCradle Team's own art
  (a tank under a radar dish).


Licence
-------

Hostile Takeover's code and data: BSD-2-Clause, Copyright (c) 2003-2014
Spiffcode, Inc. (LICENSE.txt). "Hostile Takeover" and "Warfare
Incorporated" are Spiffcode's trademarks. The Amiga files (amiga/,
package/, tools/, tests/test_part.sh, tests/test_data_endian.py): MIT,
Copyright (c) 2026 AmiCradle Team.

Designed, tested and delivered by The AmiCradle Team.
