/* The GFOS port's start: the stack, the version string and PROGDIR:User
 * (preferences, saved games, finished missions), made before the game
 * looks for them.
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#include <sys/stat.h>

/* A Shell gives 4 KB of stack; SDL 2 and the game's simulation want
 * more. libnix's startup sets this much up. */
unsigned long __stack = 512 * 1024;
static const char version[] __attribute__((used)) =
    "$VER: HostileTakeover 1.6 (16.11.2026)";

static void __attribute__((constructor)) amiga_user_drawer(void)
{
	mkdir("PROGDIR:User", 0755);
}
