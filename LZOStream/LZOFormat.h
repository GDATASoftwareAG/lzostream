/* LZOStream\LZOFormat.h -- lzo formats

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
#include <map>

static constexpr uint32_t MakeId(const char* string)
{
    auto id{(uint32_t)-1};

    for (auto i = 0; string[i]; ++i)
    {
        id ^= ((byte)string[i]) << ((i & 3) << 3);
    }

    return id;
}

class LZOFormat
{
public:
    enum class Id : uint32_t
    {
        None       = MakeId("None"),
        Lzo1       = MakeId("Lzo1"),
        Lzo1_99    = MakeId("Lzo1_99"),
        Lzo1a      = MakeId("Lzo1a"),
        Lzo1a_99   = MakeId("Lzo1a_99"),
        Lzo1b      = MakeId("Lzo1b"),
        Lzo1b_1    = MakeId("Lzo1b_1"),
        Lzo1b_2    = MakeId("Lzo1b_2"),
        Lzo1b_3    = MakeId("Lzo1b_3"),
        Lzo1b_4    = MakeId("Lzo1b_4"),
        Lzo1b_5    = MakeId("Lzo1b_5"),
        Lzo1b_6    = MakeId("Lzo1b_6"),
        Lzo1b_7    = MakeId("Lzo1b_7"),
        Lzo1b_8    = MakeId("Lzo1b_8"),
        Lzo1b_9    = MakeId("Lzo1b_9"),
        Lzo1b_99   = MakeId("Lzo1b_99"),
        Lzo1b_999  = MakeId("Lzo1b_999"),
        Lzo1c      = MakeId("Lzo1c"),
        Lzo1c_1    = MakeId("Lzo1c_1"),
        Lzo1c_2    = MakeId("Lzo1c_2"),
        Lzo1c_3    = MakeId("Lzo1c_3"),
        Lzo1c_4    = MakeId("Lzo1c_4"),
        Lzo1c_5    = MakeId("Lzo1c_5"),
        Lzo1c_6    = MakeId("Lzo1c_6"),
        Lzo1c_7    = MakeId("Lzo1c_7"),
        Lzo1c_8    = MakeId("Lzo1c_8"),
        Lzo1c_9    = MakeId("Lzo1c_9"),
        Lzo1c_99   = MakeId("Lzo1c_99"),
        Lzo1c_999  = MakeId("Lzo1c_999"),
        Lzo1f      = MakeId("Lzo1f"),
        Lzo1f_1    = MakeId("Lzo1f_1"),
        Lzo1f_999  = MakeId("Lzo1f_999"),
        Lzo1x      = MakeId("Lzo1x"),
        Lzo1x_1    = MakeId("Lzo1x_1"),
        Lzo1x_1_11 = MakeId("Lzo1x_1_11"),
        Lzo1x_1_12 = MakeId("Lzo1x_1_12"),
        Lzo1x_1_15 = MakeId("Lzo1x_1_15"),
        Lzo1x_999  = MakeId("Lzo1x_999"),
        Lzo1y      = MakeId("Lzo1y"),
        Lzo1y_1    = MakeId("Lzo1y_1"),
        Lzo1y_999  = MakeId("Lzo1y_999"),
        Lzo1z      = MakeId("Lzo1z"),
        Lzo1z_999  = MakeId("Lzo1z_999"),
        Lzo2a      = MakeId("Lzo2a"),
        Lzo2a_999  = MakeId("Lzo2a_999"),
        Default    = Lzo1x_999
    };

    struct LessNoCase
    {
        bool operator()(LPCTSTR left, LPCTSTR right) const
        {
            return _tcsicmp(left, right) < 0;
        }
    };

    struct Info
    {
        const char*      Name{};
        lzo_compress_t   FunctionCompress{};
        lzo_decompress_t FunctionDecompress{};
        uint32_t         MemoryCompress{};
        uint32_t         MemoryDecompress{};
    };

    static const std::map<LPCTSTR, Id, LessNoCase>& FormatIds()
    {
        static const std::map<LPCTSTR, Id, LessNoCase> formatIds{{_T("Lzo1"), Id::Lzo1}, {_T("Lzo1_99"), Id::Lzo1_99},
            {_T("Lzo1a"), Id::Lzo1a}, {_T("Lzo1a_99"), Id::Lzo1a_99}, {_T("Lzo1b"), Id::Lzo1b},
            {_T("Lzo1b_1"), Id::Lzo1b_1}, {_T("Lzo1b_2"), Id::Lzo1b_2}, {_T("Lzo1b_3"), Id::Lzo1b_3},
            {_T("Lzo1b_4"), Id::Lzo1b_4}, {_T("Lzo1b_5"), Id::Lzo1b_5}, {_T("Lzo1b_6"), Id::Lzo1b_6},
            {_T("Lzo1b_7"), Id::Lzo1b_7}, {_T("Lzo1b_8"), Id::Lzo1b_8}, {_T("Lzo1b_9"), Id::Lzo1b_9},
            {_T("Lzo1b_99"), Id::Lzo1b_99}, {_T("Lzo1b_999"), Id::Lzo1b_999}, {_T("Lzo1c"), Id::Lzo1c},
            {_T("Lzo1c_1"), Id::Lzo1c_1}, {_T("Lzo1c_2"), Id::Lzo1c_2}, {_T("Lzo1c_3"), Id::Lzo1c_3},
            {_T("Lzo1c_4"), Id::Lzo1c_4}, {_T("Lzo1c_5"), Id::Lzo1c_5}, {_T("Lzo1c_6"), Id::Lzo1c_6},
            {_T("Lzo1c_7"), Id::Lzo1c_7}, {_T("Lzo1c_8"), Id::Lzo1c_8}, {_T("Lzo1c_9"), Id::Lzo1c_9},
            {_T("Lzo1c_99"), Id::Lzo1c_99}, {_T("Lzo1c_999"), Id::Lzo1c_999}, {_T("Lzo1f"), Id::Lzo1f},
            {_T("Lzo1f_1"), Id::Lzo1f_1}, {_T("Lzo1f_999"), Id::Lzo1f_999}, {_T("Lzo1x"), Id::Lzo1x},
            {_T("Lzo1x_1"), Id::Lzo1x_1}, {_T("Lzo1x_1_11"), Id::Lzo1x_1_11}, {_T("Lzo1x_1_12"), Id::Lzo1x_1_12},
            {_T("Lzo1x_1_15"), Id::Lzo1x_1_15}, {_T("Lzo1x_999"), Id::Lzo1x_999}, {_T("Lzo1y"), Id::Lzo1y},
            {_T("Lzo1y_1"), Id::Lzo1y_1}, {_T("Lzo1y_999"), Id::Lzo1y_999}, {_T("Lzo1z"), Id::Lzo1z},
            {_T("Lzo1z_999"), Id::Lzo1z_999}, {_T("Lzo2a"), Id::Lzo2a}, {_T("Lzo2a_999"), Id::Lzo2a_999}};

        return formatIds;
    }

    static const std::map<Id, Info>& FormatInfos()
    {
        static const std::map<Id, Info> formatInfos{{Id::None, {"None", nullptr, nullptr, 0, 0}},
            {Id::Lzo1, {"Lzo1", lzo1_compress, lzo1_decompress, LZO1_MEM_COMPRESS, LZO1_MEM_DECOMPRESS}},
            {Id::Lzo1_99, {"Lzo1_99", lzo1_99_compress, lzo1_decompress, LZO1_99_MEM_COMPRESS, LZO1_MEM_DECOMPRESS}},
            {Id::Lzo1a, {"Lzo1a", lzo1a_compress, lzo1a_decompress, LZO1A_MEM_COMPRESS, LZO1A_MEM_DECOMPRESS}},
            {Id::Lzo1a_99,
                {"Lzo1a_99", lzo1a_99_compress, lzo1a_decompress, LZO1A_99_MEM_COMPRESS, LZO1A_MEM_DECOMPRESS}},
            {Id::Lzo1b, {"Lzo1b", lzo1b_999_compress, lzo1b_decompress, LZO1B_999_MEM_COMPRESS, LZO1B_MEM_DECOMPRESS}},
            {Id::Lzo1b_1, {"Lzo1b_1", lzo1b_1_compress, lzo1b_decompress, LZO1B_MEM_COMPRESS, LZO1B_MEM_DECOMPRESS}},
            {Id::Lzo1b_2, {"Lzo1b_2", lzo1b_2_compress, lzo1b_decompress, LZO1B_MEM_COMPRESS, LZO1B_MEM_DECOMPRESS}},
            {Id::Lzo1b_3, {"Lzo1b_3", lzo1b_3_compress, lzo1b_decompress, LZO1B_MEM_COMPRESS, LZO1B_MEM_DECOMPRESS}},
            {Id::Lzo1b_4, {"Lzo1b_4", lzo1b_4_compress, lzo1b_decompress, LZO1B_MEM_COMPRESS, LZO1B_MEM_DECOMPRESS}},
            {Id::Lzo1b_5, {"Lzo1b_5", lzo1b_5_compress, lzo1b_decompress, LZO1B_MEM_COMPRESS, LZO1B_MEM_DECOMPRESS}},
            {Id::Lzo1b_6, {"Lzo1b_6", lzo1b_6_compress, lzo1b_decompress, LZO1B_MEM_COMPRESS, LZO1B_MEM_DECOMPRESS}},
            {Id::Lzo1b_7, {"Lzo1b_7", lzo1b_7_compress, lzo1b_decompress, LZO1B_MEM_COMPRESS, LZO1B_MEM_DECOMPRESS}},
            {Id::Lzo1b_8, {"Lzo1b_8", lzo1b_8_compress, lzo1b_decompress, LZO1B_MEM_COMPRESS, LZO1B_MEM_DECOMPRESS}},
            {Id::Lzo1b_9, {"Lzo1b_9", lzo1b_9_compress, lzo1b_decompress, LZO1B_MEM_COMPRESS, LZO1B_MEM_DECOMPRESS}},
            {Id::Lzo1b_99,
                {"Lzo1b_99", lzo1b_99_compress, lzo1b_decompress, LZO1B_99_MEM_COMPRESS, LZO1B_MEM_DECOMPRESS}},
            {Id::Lzo1b_999,
                {"Lzo1b_999", lzo1b_999_compress, lzo1b_decompress, LZO1B_999_MEM_COMPRESS, LZO1B_MEM_DECOMPRESS}},
            {Id::Lzo1c, {"Lzo1c", lzo1c_999_compress, lzo1c_decompress, LZO1C_999_MEM_COMPRESS, LZO1C_MEM_DECOMPRESS}},
            {Id::Lzo1c_1, {"Lzo1c_1", lzo1c_1_compress, lzo1c_decompress, LZO1C_MEM_COMPRESS, LZO1C_MEM_DECOMPRESS}},
            {Id::Lzo1c_2, {"Lzo1c_2", lzo1c_2_compress, lzo1c_decompress, LZO1C_MEM_COMPRESS, LZO1C_MEM_DECOMPRESS}},
            {Id::Lzo1c_3, {"Lzo1c_3", lzo1c_3_compress, lzo1c_decompress, LZO1C_MEM_COMPRESS, LZO1C_MEM_DECOMPRESS}},
            {Id::Lzo1c_4, {"Lzo1c_4", lzo1c_4_compress, lzo1c_decompress, LZO1C_MEM_COMPRESS, LZO1C_MEM_DECOMPRESS}},
            {Id::Lzo1c_5, {"Lzo1c_5", lzo1c_5_compress, lzo1c_decompress, LZO1C_MEM_COMPRESS, LZO1C_MEM_DECOMPRESS}},
            {Id::Lzo1c_6, {"Lzo1c_6", lzo1c_6_compress, lzo1c_decompress, LZO1C_MEM_COMPRESS, LZO1C_MEM_DECOMPRESS}},
            {Id::Lzo1c_7, {"Lzo1c_7", lzo1c_7_compress, lzo1c_decompress, LZO1C_MEM_COMPRESS, LZO1C_MEM_DECOMPRESS}},
            {Id::Lzo1c_8, {"Lzo1c_8", lzo1c_8_compress, lzo1c_decompress, LZO1C_MEM_COMPRESS, LZO1C_MEM_DECOMPRESS}},
            {Id::Lzo1c_9, {"Lzo1c_9", lzo1c_9_compress, lzo1c_decompress, LZO1C_MEM_COMPRESS, LZO1C_MEM_DECOMPRESS}},
            {Id::Lzo1c_99,
                {"Lzo1c_99", lzo1c_99_compress, lzo1c_decompress, LZO1C_99_MEM_COMPRESS, LZO1C_MEM_DECOMPRESS}},
            {Id::Lzo1c_999,
                {"Lzo1c_999", lzo1c_999_compress, lzo1c_decompress, LZO1C_999_MEM_COMPRESS, LZO1C_MEM_DECOMPRESS}},
            {Id::Lzo1f, {"Lzo1f", lzo1f_999_compress, lzo1f_decompress, LZO1F_999_MEM_COMPRESS, LZO1F_MEM_DECOMPRESS}},
            {Id::Lzo1f_1, {"Lzo1f_1", lzo1f_1_compress, lzo1f_decompress, LZO1F_MEM_COMPRESS, LZO1F_MEM_DECOMPRESS}},
            {Id::Lzo1f_999,
                {"Lzo1f_999", lzo1f_999_compress, lzo1f_decompress, LZO1F_999_MEM_COMPRESS, LZO1F_MEM_DECOMPRESS}},
            {Id::Lzo1x, {"Lzo1x", lzo1x_999_compress, lzo1x_decompress, LZO1X_999_MEM_COMPRESS, LZO1X_MEM_DECOMPRESS}},
            {Id::Lzo1x_1, {"Lzo1x_1", lzo1x_1_compress, lzo1x_decompress, LZO1X_1_MEM_COMPRESS, LZO1X_MEM_DECOMPRESS}},
            {Id::Lzo1x_1_11,
                {"Lzo1x_1_11", lzo1x_1_11_compress, lzo1x_decompress, LZO1X_1_11_MEM_COMPRESS, LZO1X_MEM_DECOMPRESS}},
            {Id::Lzo1x_1_12,
                {"Lzo1x_1_12", lzo1x_1_12_compress, lzo1x_decompress, LZO1X_1_12_MEM_COMPRESS, LZO1X_MEM_DECOMPRESS}},
            {Id::Lzo1x_1_15,
                {"Lzo1x_1_15", lzo1x_1_15_compress, lzo1x_decompress, LZO1X_1_15_MEM_COMPRESS, LZO1X_MEM_DECOMPRESS}},
            {Id::Lzo1x_999,
                {"Lzo1x_999", lzo1x_999_compress, lzo1x_decompress, LZO1X_999_MEM_COMPRESS, LZO1X_MEM_DECOMPRESS}},
            {Id::Lzo1y, {"Lzo1y", lzo1y_999_compress, lzo1y_decompress, LZO1Y_999_MEM_COMPRESS, LZO1Y_MEM_DECOMPRESS}},
            {Id::Lzo1y_1, {"Lzo1y_1", lzo1y_1_compress, lzo1y_decompress, LZO1Y_MEM_COMPRESS, LZO1Y_MEM_DECOMPRESS}},
            {Id::Lzo1y_999,
                {"Lzo1y_999", lzo1y_999_compress, lzo1y_decompress, LZO1Y_999_MEM_COMPRESS, LZO1Y_MEM_DECOMPRESS}},
            {Id::Lzo1z, {"Lzo1z", lzo1z_999_compress, lzo1z_decompress, LZO1Z_999_MEM_COMPRESS, LZO1Z_MEM_DECOMPRESS}},
            {Id::Lzo1z_999,
                {"Lzo1z_999", lzo1z_999_compress, lzo1z_decompress, LZO1Z_999_MEM_COMPRESS, LZO1Z_MEM_DECOMPRESS}},
            {Id::Lzo2a, {"Lzo2a", lzo2a_999_compress, lzo2a_decompress, LZO2A_999_MEM_COMPRESS, LZO2A_MEM_DECOMPRESS}},
            {Id::Lzo2a_999,
                {"Lzo2a_999", lzo2a_999_compress, lzo2a_decompress, LZO2A_999_MEM_COMPRESS, LZO2A_MEM_DECOMPRESS}}};

        return formatInfos;
    }

    static Id FormatId(LPCTSTR format)
    {
        if (!format)
        {
            return {};
        }

        const auto found{FormatIds().find(format)};

        return (found != FormatIds().end()) ? found->second : Id::None;
    }

    static const Info* FormatInfo(const Id formatId)
    {
        const auto found{FormatInfos().find(formatId)};

        return (found != FormatInfos().end()) ? &found->second : nullptr;
    }
};
