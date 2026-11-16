#!/usr/bin/env python3
"""The GFOS build reads htdata832.pdb and htsfx.pdb with BigWord/BigDword
as identity (game/sdl/htplatform.h, mpshared/misc.h under __amigaos__).
That is right only if the files are big-endian. This checks it on the host:
each is a Palm database whose record count and record offsets make sense
read big-endian (ascending, inside the file) and not read little-endian.
MIT, Copyright (c) 2026 AmiCradle Team."""
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def offsets(data, fmt):
    count = struct.unpack(fmt + "H", data[76:78])[0]
    offs = [struct.unpack(fmt + "I", data[78 + 8 * i:82 + 8 * i])[0] for i in range(count)]
    return count, offs


def sane(data, count, offs):
    return (count > 0 and 78 + 8 * count <= len(data)
            and all(78 + 8 * count <= o < len(data) for o in offs)
            and offs == sorted(offs))


fails = 0
for name in ("htdata832.pdb", "htsfx.pdb"):
    data = (ROOT / "game" / name).read_bytes()
    be = offsets(data, ">")
    le = offsets(data, "<")
    ok_be, ok_le = sane(data, *be), sane(data, *le)
    print(f"{name}: {be[0]} records, big-endian {'ok' if ok_be else 'BAD'}, "
          f"little-endian {'ok' if ok_le else 'rejected'}")
    if not ok_be or ok_le:
        fails += 1
sys.exit(1 if fails else 0)
