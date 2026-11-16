// Hostile Takeover for GFOS: HTTP requests without a network stack.
//
// The SDL build fetches the mission pack index, mission packs and the
// multiplayer server list with libcurl on a helper thread. GFOS has no
// libcurl in the stove kit yet, so every request fails the way a failed
// connection does: kidmError is posted to the game thread, and the game
// shows its own "could not connect" message. Single-player and the
// missions in htdata832.pdb need no network.
//
// MIT, Copyright (c) 2026 AmiCradle Team.

#include "game/sdl/sdlhttprequest.h"
#include "game/sdl/hosthelpers.h"
#include "base/thread.h"

namespace wi {

SdlHttpRequest::SdlHttpRequest(HttpResponseHandler *handler) : handler_(handler) {
    curl_handle_ = NULL;
    chunk_.memory = NULL;
    chunkHeader_.memory = NULL;
}

SdlHttpRequest::~SdlHttpRequest() {
}

void SdlHttpRequest::Submit() {
    ErrorParams *pparams = new ErrorParams;
    strncpyz(pparams->szError, "This GFOS build has no network support",
            sizeof(pparams->szError));
    thread_.Post(kidmError, this, pparams);
}

void SdlHttpRequest::Release() {
    thread_.Clear(this);
}

void SdlHttpRequest::OnMessage(base::Message *pmsg) {
    switch (pmsg->id) {
    case kidmError:
        {
            ErrorParams *pparams = (ErrorParams *)pmsg->data;
            handler_->OnError(this, pparams->szError);
            delete pparams;
        }
        break;
    }
}

} // namespace wi
