// Hostile Takeover for GFOS: where the game keeps its files, and the host
// calls the Linux build makes through game/sdl/linux/hosthelpers.cpp.
//
// Everything lives in the game's drawer: htdata832.pdb and htsfx.pdb next
// to the program, and the player's own files (preferences, saved games,
// finished missions, downloaded mission packs) in PROGDIR:User, so a
// reinstall or a GFOpenUp update never touches them.
//
// MIT, Copyright (c) 2026 AmiCradle Team.

#include "game/sdl/linux/linuxchatcontroller.h"
#include "game/sdl/hosthelpers.h"
#include "game/sdl/sdlhttpservice.h"
#include "base/thread.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace wi {

static char gszMainDataDir[] = "PROGDIR:";
static char gszHomeDir[] = "PROGDIR:User";
static char gszTempDir[] = "PROGDIR:User/tmp";
static char gszMissionPacksDir[] = "PROGDIR:User/MissionPacks";
static char gszMissionPackInfosDir[] = "PROGDIR:User/MissionPackInfos";
static char gszSaveGamesDir[] = "PROGDIR:User/SaveGames";
static char gszCompletesDir[] = "PROGDIR:User/Completes/";
static char gszPrefsFilename[] = "PROGDIR:User/prefs.bin";
HttpService *gphttp;
IChatController *gpchat;

bool HostHelpers::Init() {
    mkdir(gszHomeDir, 0755);
    mkdir(gszTempDir, 0755);
    mkdir(gszMissionPacksDir, 0755);
    mkdir(gszMissionPackInfosDir, 0755);
    mkdir(gszSaveGamesDir, 0755);
    mkdir("PROGDIR:User/Completes", 0755);

    gphttp = (HttpService *)new SdlHttpService();
    gpchat = NULL;
    return true;
}

void HostHelpers::Cleanup() {
    delete gphttp;
}

const char *HostHelpers::GetMainDataDir() { return gszMainDataDir; }
const char *HostHelpers::GetTempDir() { return gszTempDir; }
const char *HostHelpers::GetMissionPacksDir() { return gszMissionPacksDir; }
const char *HostHelpers::GetMissionPackInfosDir() { return gszMissionPackInfosDir; }
const char *HostHelpers::GetSaveGamesDir() { return gszSaveGamesDir; }
const char *HostHelpers::GetCompletesDir() { return gszCompletesDir; }
const char *HostHelpers::GetPrefsFilename() { return gszPrefsFilename; }

void HostHelpers::OpenUrl(const char *pszUrl) {
    Log("OpenUrl: %s", pszUrl);
}

void HostHelpers::Log(const char *pszFormat, va_list va) {
#ifdef DEBUG_LOGGING
    char sz[512];
    vsnprintf(sz, sizeof(sz), pszFormat, va);
    printf("%s\n", sz);
#endif
}

void HostHelpers::Log(const char *pszFormat, ...) {
    va_list va;
    va_start(va, pszFormat);
    Log(pszFormat, va);
    va_end(va);
}

void HostHelpers::MessageBox(const char *pszFormat, va_list va) {
    char sz[512];
    vsnprintf(sz, sizeof(sz), pszFormat, va);
    printf("%s\n", sz);
}

void HostHelpers::Break() {
    Log("BREAK!!");
}

// 640x480, 8 bits: what the game draws into each frame before the SDL
// texture upload. The Linux build uses 800x600; 640x480 is a third less
// to convert and upload on a 68040 and still shows the whole UI.
void HostHelpers::GetSurfaceProperties(SurfaceProperties *pprops) {
    pprops->cxWidth = 640;
    pprops->cyHeight = 480;
    pprops->cbxPitch = 1;
    pprops->cbyPitch = pprops->cxWidth;
    pprops->ffFormat = wi::kfDirect8;
}

bool HostHelpers::IsExiting() {
    return false;
}

// The device id only names this machine to the multiplayer server. It is
// made once and kept in PROGDIR:User/udid so it stays the same.
const char *HostHelpers::GetUdid() {
    static char gszUdid[20];
    if (gszUdid[0] != 0)
        return gszUdid;
    FILE *pf = fopen("PROGDIR:User/udid", "r");
    if (pf != NULL) {
        if (fgets(gszUdid, sizeof(gszUdid), pf) == NULL)
            gszUdid[0] = 0;
        fclose(pf);
        char *pch = gszUdid;
        while (*pch >= '0' && *pch <= '9')
            pch++;
        *pch = 0;
        if (pch - gszUdid == 19)
            return gszUdid;
    }
    srand((unsigned)time(NULL));
    for (int i = 0; i < 19; i++)
        gszUdid[i] = '0' + rand() % 10;
    gszUdid[19] = 0;
    pf = fopen("PROGDIR:User/udid", "w");
    if (pf != NULL) {
        fprintf(pf, "%s\n", gszUdid);
        fclose(pf);
    }
    return gszUdid;
}

void HostHelpers::InitiateAsk(const char *title, int max, const char *def,
        int keyboard, bool secure) {
    Log("HostHelpers::InitiateAsk not implemented yet");
}

void HostHelpers::GetAskString(char *psz, int cb) {
    Log("HostHelpers::GetAskString not implemented yet");
}

IChatController *HostHelpers::GetChatController() {
    if (gpchat == NULL)
        gpchat = new wi::LinuxChatController();
    return gpchat;
}

void HostHelpers::InitiateWebView(const char *title, const char *url) {
    Log("InitiateWebView: %s", url);
}

const char *HostHelpers::GetPlatformString() {
    return "GFOS";
}

} // namespace wi
