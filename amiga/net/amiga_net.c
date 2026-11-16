/* bsdsocket.library for Hostile Takeover's network code: opened when the
 * game starts if a TCP/IP stack (Roadshow) runs, closed when it ends.
 * Without one SocketBase stays NULL and amiga_net.h fails every call.
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/filio.h>
#include <proto/exec.h>
#include <proto/bsdsocket.h>

struct Library *SocketBase;

/* The only fcntl() calls are F_GETFL and F_SETFL with O_NONBLOCK on
 * sockets: keep the flag per socket number and pass it on as FIONBIO. */
static unsigned char nonblock[256];

int amiga_net_fcntl(int s, int cmd, long arg)
{
	long on;

	if (SocketBase == NULL || s < 0 || s >= (int)sizeof(nonblock))
		return -1;
	if (cmd == F_GETFL)
		return nonblock[s] ? O_NONBLOCK : 0;
	if (cmd != F_SETFL)
		return -1;
	on = (arg & O_NONBLOCK) != 0;
	if (IoctlSocket(s, FIONBIO, (char *)&on) < 0)
		return -1;
	nonblock[s] = (unsigned char)on;
	return 0;
}

static void __attribute__((constructor)) amiga_net_open(void)
{
	SocketBase = OpenLibrary("bsdsocket.library", 4);
}

static void __attribute__((destructor)) amiga_net_close(void)
{
	if (SocketBase != NULL) {
		CloseLibrary(SocketBase);
		SocketBase = NULL;
	}
}
