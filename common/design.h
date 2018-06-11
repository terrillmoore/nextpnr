/*
 *  nextpnr -- Next Generation Place and Route
 *
 *  Copyright (C) 2018  Clifford Wolf <clifford@clifford.at>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifndef DESIGN_H
#define DESIGN_H

#include <assert.h>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// replace with proper IdString later
typedef std::string IdString;

struct GraphicElement
{
    // This will control colour, and there should be separate
    // visibility controls in some cases also
    enum
    {
        // Wires entirely inside tiles, e.g. between switchbox and bels
        G_LOCAL_WIRES,
        // Standard inter-tile routing
        G_GENERAL_WIRES,
        // Special inter-tile wires, e.g. carry chains
        G_DEDICATED_WIRES,
        G_BEL_OUTLINE,
        G_SWITCHBOX_OUTLINE,
        G_TILE_OUTLINE,
        G_BEL_PINS,
        G_SWITCHBOX_PINS,
        G_BEL_MISC,
        G_TILE_MISC,
    } style;

    enum
    {
        G_LINE,
        G_BOX,
        G_CIRCLE,
        G_LABEL
    } type;

    float x1, y1, x2, y2, z;
    std::string text;
};

#include "chip.h"

struct CellInfo;

struct PortRef
{
    CellInfo *cell = nullptr;
    IdString port;
};

struct NetInfo
{
    IdString name;
    PortRef driver;
    std::vector<PortRef> users;
    std::unordered_map<IdString, std::string> attrs;

    // wire -> uphill_pip
    std::unordered_map<WireId, PipId> wires;
};

enum PortType
{
    PORT_IN = 0,
    PORT_OUT = 1,
    PORT_INOUT = 2
};

struct PortInfo
{
    IdString name;
    NetInfo *net;
    PortType type;
};

struct CellInfo
{
    IdString name, type;
    std::unordered_map<IdString, PortInfo> ports;
    std::unordered_map<IdString, std::string> attrs, params;

    BelId bel;
    // cell_port -> bel_pin
    std::unordered_map<IdString, IdString> pins;
};

struct Design
{
    struct Chip chip;

    Design(ChipArgs args) : chip(args)
    {
        // ...
    }

    std::unordered_map<IdString, NetInfo *> nets;
    std::unordered_map<IdString, CellInfo *> cells;
};

#endif
