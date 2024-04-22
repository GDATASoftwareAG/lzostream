/* LZOStream\LZOHeader.h -- LZOStream header definition/ implementation

   This file is part of the LZOStream application for compressing/ decompressing files or streams.

   Copyright (C) 2024 G DATA CyberDefense AG
   All Rights Reserved.

   The LZOStream application is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   The LZOStream application is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the LZOStream application; see the file License.txt.
   If not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

   G DATA CyberDefense AG
   <source@gdata.de>
   https://www.gdata.de/
*/

#pragma once
#include "LZOFormat.h"

constexpr uint32_t LZOHeaderId{'L' | ('Z' << 8) | ('O' << 16) | (28 << 24)};

class LZOHeader
{
public:
    void Initialize(const LZOFormat::Id formatId, const uint32_t sourceSize, const uint32_t destinationSize,
        const uint32_t sourceHash = {}, const uint32_t destinationHash = {})
    {
        HeaderId        = LZOHeaderId;
        FormatId        = formatId;
        SourceSize      = sourceSize;
        DestinationSize = destinationSize;
        SourceHash      = sourceHash;
        DestinationHash = destinationHash;
        HeaderHash      = HeaderCrc32(this, sizeof(*this));
    }

    byte* Data() const
    {
        return (byte*)(this + 1);
    }

    bool Valid() const
    {
        return HeaderHash == HeaderCrc32(this, sizeof(*this));
    }

    static size_t Size(const size_t additional = 0)
    {
        return sizeof(LZOHeader) + additional;
    }

    static LZOHeader* Header(const void* header, const size_t size, const bool check = false)
    {
        if (!header || size < sizeof(LZOHeader))
        {
            return {};
        }
        if (check && ((LZOHeader*)header)->HeaderHash != HeaderCrc32(header, size))
        {
            return {};
        }

        return (LZOHeader*)header;
    }

    static uint32_t HeaderCrc32(const void* header, const size_t size)
    {
        if (!header || size < sizeof(LZOHeader))
        {
            return {};
        }

        return lzo_crc32(0, (const lzo_bytep)header, sizeof(LZOHeader) - sizeof(uint32_t));
    }

    uint32_t      HeaderId{LZOHeaderId};
    LZOFormat::Id FormatId{};
    uint32_t      SourceSize{};
    uint32_t      DestinationSize{};
    uint32_t      SourceHash{};
    uint32_t      DestinationHash{};
    uint32_t      HeaderHash{};
};
