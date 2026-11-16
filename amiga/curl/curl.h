/* Hostile Takeover for GFOS: the Amiga build has no libcurl yet.
 * game/sdl/sdlhttprequest.h only names CURL as a pointer member; the
 * Amiga request (amiga/amigahttprequest.cpp) never uses it.
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#ifndef AMIGA_CURL_STUB_H
#define AMIGA_CURL_STUB_H
typedef void CURL;
#define CURL_ERROR_SIZE 256
#endif
