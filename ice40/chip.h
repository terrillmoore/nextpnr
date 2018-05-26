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

#include "design.h"

#ifndef CHIP_H
#define CHIP_H

struct BelId
{
	int32_t index = -1;

	bool nil() const {
		return index < 0;
	}
};

struct WireId
{
	int32_t index = -1;

	bool nil() const {
		return index < 0;
	}
};

namespace std
{
	template<> struct hash<BelId>
        {
		std::size_t operator()(const BelId &bel) const noexcept
		{
			return bel.index;
		}
	};

	template<> struct hash<WireId>
        {
		std::size_t operator()(const WireId &wire) const noexcept
		{
			return wire.index;
		}
	};
}

struct BelIterator
{
	BelId *ptr = nullptr;

	void operator++() { ptr++; }
	bool operator!=(const BelIterator &other) const { return ptr != other.ptr; }
	BelId operator*() const { return *ptr; }
};

struct BelRange
{
	BelIterator b, e;
	BelIterator begin() const { return b; }
	BelIterator end() const { return e; }
};

struct WireIterator
{
	WireId *ptr = nullptr;

	void operator++() { ptr++; }
	bool operator!=(const WireIterator &other) const { return ptr != other.ptr; }
	WireId operator*() const { return *ptr; }
};

struct WireRange
{
	WireIterator b, e;
	WireIterator begin() const { return b; }
	WireIterator end() const { return e; }
};

struct WireDelay
{
	WireId wire;
	float delay;
};

struct WireDelayIterator
{
	WireDelay *ptr = nullptr;

	void operator++() { ptr++; }
	bool operator!=(const WireDelayIterator &other) const { return ptr != other.ptr; }
	WireDelay operator*() const { return *ptr; }
};

struct WireDelayRange
{
	WireDelayIterator b, e;
	WireDelayIterator begin() const { return b; }
	WireDelayIterator end() const { return e; }
};

struct BelPin
{
	BelId bel;
	IdString pin;
};

struct BelPinIterator
{
	BelPin *ptr = nullptr;

	void operator++() { ptr++; }
	bool operator!=(const BelPinIterator &other) const { return ptr != other.ptr; }
	BelPin operator*() const { return *ptr; }
};

struct BelPinRange
{
	BelPinIterator b, e;
	BelPinIterator begin() const { return b; }
	BelPinIterator end() const { return e; }
};

struct GuiLine
{
	float x1, y1, x2, y2;
};

struct ChipArgs
{
	enum {
		NONE,
		LP384,
		LP1K,
		LP8K,
		HX1K,
		HX8K,
		UP5K
	} type = NONE;
};

struct BelInfo
{
	const char *name;
};

struct WireDelayPOD
{
	int32_t wire_index;
	float delay;
};

struct BelPortPOD
{
	int32_t bel_index;
	int port_index;
};

struct WireInfo
{
	const char *name;
	int num_uphill, num_downhill, num_bidir;
	WireDelayPOD *wires_uphill, *wires_downhill, *wires_bidir;

	int num_bels_downhill;
	BelPortPOD bel_uphill;
	BelPortPOD *bels_downhill;
};

struct Chip
{
	int num_bels, num_wires;
	BelInfo *bel_data;
	WireInfo *wire_data;

	// ...

	Chip(ChipArgs args);

	void setBelActive(BelId bel, bool active);
	bool getBelActive(BelId bel);

	BelId getBelByName(IdString name) const;
	WireId getWireByName(IdString name) const;
	IdString getBelName(BelId bel) const;
	IdString getWireName(WireId wire) const;

	BelRange getBels() const;
	BelRange getBelsByType(IdString type) const;
	IdString getBelType(BelId bel) const;

	void getBelPosition(BelId bel, float &x, float &y) const;
	void getWirePosition(WireId wire, float &x, float &y) const;
	vector<GuiLine> getBelGuiLines(BelId bel) const;
	vector<GuiLine> getWireGuiLines(WireId wire) const;

	WireRange getWires() const;
	WireDelayRange getWiresUphill(WireId wire) const;
	WireDelayRange getWiresDownhill(WireId wire) const;
	WireDelayRange getWiresBidir(WireId wire) const;
	WireDelayRange getWireAliases(WireId wire) const;

	// the following will only operate on / return "active" BELs
	// multiple active uphill BELs for a wire will cause a runtime error
	WireId getWireBelPin(BelId bel, IdString pin) const;
	BelPin getBelPinUphill(WireId wire) const;
	BelPinRange getBelPinsDownhill(WireId wire) const;
};

#endif