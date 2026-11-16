/* Hostile Takeover for GFOS: base/'s BSD socket code on Roadshow's
 * bsdsocket.library (amiga/net/*.cpp include this, then the upstream
 * file, so the system headers are already in and only the upstream
 * code sees these names).
 *
 * C++ only (base/ is C++).
 * - socket calls go through bsdsocket.library's inlines; SocketBase is
 *   opened at start (amiga/net/amiga_net.c) and may be NULL when no TCP/IP
 *   stack runs: then every call fails like a closed network, and the game
 *   says it cannot connect. Single-player never gets here.
 * - close() and ioctl() on a socket are CloseSocket() and IoctlSocket();
 *   fcntl(O_NONBLOCK) is FIONBIO; select() is WaitSelect(); errno is the
 *   socket library's Errno().
 *
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#ifndef AMIGA_NET_H
#define AMIGA_NET_H

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <proto/bsdsocket.h>
/* exec's inline names would hit base/'s Wait(), Signal() and Remove(). */
#undef Wait
#undef Signal
#undef Remove

#ifdef __cplusplus
extern "C" {
#endif
extern struct Library *SocketBase;
int amiga_net_fcntl(int s, int cmd, long arg);
#ifdef __cplusplus
}
#endif

#define AMIGA_NET_UP (SocketBase != NULL)

/* Functions, not function-like macros: upstream calls some as ::socket(). */
static inline int amiga_net_errno(void) { return AMIGA_NET_UP ? (int)Errno() : ENETDOWN; }
static inline int amiga_net_close(int s) { return AMIGA_NET_UP ? (int)CloseSocket(s) : -1; }
static inline int amiga_net_ioctl(int s, unsigned long r, void *a)
{ return AMIGA_NET_UP ? (int)IoctlSocket(s, r, (char *)a) : -1; }
static inline int amiga_net_select(int n, fd_set *r, fd_set *w, fd_set *e, struct timeval *t)
{ return AMIGA_NET_UP ? (int)WaitSelect(n, r, w, e, t, NULL) : -1; }
static inline int amiga_net_socket(int d, int t, int p)
{ return AMIGA_NET_UP ? (int)socket(d, t, p) : -1; }
static inline struct hostent *amiga_net_gethostbyname(const char *n)
{ return AMIGA_NET_UP ? gethostbyname((STRPTR)n) : (struct hostent *)NULL; }
static inline int amiga_net_gethostname(char *n, int l)
{ return AMIGA_NET_UP ? (int)gethostname((STRPTR)n, l) : -1; }
static inline int amiga_net_inet_aton(const char *c, struct in_addr *a)
{ return AMIGA_NET_UP ? (int)inet_aton((STRPTR)c, a) : 0; }
static inline int amiga_net_fcntl_va(int s, int c, long a = 0) { return amiga_net_fcntl(s, c, a); }

#undef errno
#define errno amiga_net_errno()
#undef close
#define close amiga_net_close
#undef ioctl
#define ioctl amiga_net_ioctl
#undef fcntl
#define fcntl amiga_net_fcntl_va
#undef select
#define select amiga_net_select
#undef socket
#define socket amiga_net_socket
#undef gethostbyname
#define gethostbyname amiga_net_gethostbyname
#undef gethostname
#define gethostname amiga_net_gethostname
#undef inet_aton
#define inet_aton amiga_net_inet_aton

#endif
