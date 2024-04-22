/* LZOStreamTest\CompressTest.cpp -- Google test implementation

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

#include "stdafx.h"
#include "LZOStreamCall.h"

const std::string loremIpsum{
    R"(Lorem ipsum dolor sit amet, consectetur adipisici elit, sed eiusmod tempor incidunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquid ex ea commodi consequat. Quis aute iure reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint obcaecat cupiditat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat.
Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.
Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat facer possim assum. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat.
Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis.
At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, At accusam aliquyam diam diam dolore dolores duo eirmod eos erat, et nonumy sed tempor et et invidunt justo labore Stet clita ea et gubergren, kasd magna no rebum. sanctus sea sed takimata ut vero voluptua. est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat.
Consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. )"};

const std::vector<LPCTSTR> Formats{_T("Lzo1"), _T("Lzo1_99"), _T("Lzo1a"), _T("Lzo1a_99"), _T("Lzo1b"), _T("Lzo1b_1"),
    _T("Lzo1b_2"), _T("Lzo1b_3"), _T("Lzo1b_4"), _T("Lzo1b_5"), _T("Lzo1b_6"), _T("Lzo1b_7"), _T("Lzo1b_8"),
    _T("Lzo1b_9"), _T("Lzo1b_99"), _T("Lzo1b_999"), _T("Lzo1c"), _T("Lzo1c_1"), _T("Lzo1c_2"), _T("Lzo1c_3"),
    _T("Lzo1c_4"), _T("Lzo1c_5"), _T("Lzo1c_6"), _T("Lzo1c_7"), _T("Lzo1c_8"), _T("Lzo1c_9"), _T("Lzo1c_99"),
    _T("Lzo1c_999"), _T("Lzo1f"), _T("Lzo1f_1"), _T("Lzo1f_999"), _T("Lzo1x"), _T("Lzo1x_1"), _T("Lzo1x_1_11"),
    _T("Lzo1x_1_12"), _T("Lzo1x_1_15"), _T("Lzo1x_999"), _T("Lzo1y"), _T("Lzo1y_1"), _T("Lzo1y_999"), _T("Lzo1z"),
    _T("Lzo1z_999"), _T("Lzo2a"), _T("Lzo2a_999")};

bool WriteData(LPCTSTR file, const void* data, const uint32_t size)
{
    const auto handle{CreateFile(file, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr)};

    if (handle == INVALID_HANDLE_VALUE)
    {
        return {};
    }

    DWORD      written{};
    const auto result{WriteFile(handle, data, size, &written, nullptr)};

    CloseHandle(handle);

    return result;
}

std::string ReadString(LPCTSTR file)
{
    const auto handle{
        CreateFile(file, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr)};

    if (handle == INVALID_HANDLE_VALUE)
    {
        return {};
    }

    DWORD high;
    DWORD size{GetFileSize(handle, &high)};

    if (high)
    {
        return {};
    }

    try
    {
        std::string string(size, '\0');
        DWORD       read;

        if (ReadFile(handle, (LPVOID)string.data(), string.size(), &read, nullptr))
        {
            CloseHandle(handle);

            return string;
        }
    }
    catch (std::exception&)
    {
    }

    CloseHandle(handle);

    return {};
}

TEST(Compress, Basic)
{
    const auto lzoStream{_T(R"(LZOStream.exe)")};

    for (const auto& format : Formats)
    {
        const auto compressed{LZOStreamCompress(lzoStream, loremIpsum.data(), loremIpsum.size(), format)};
        const auto decompressed{LZOStreamDecompress(lzoStream, compressed.data(), compressed.size())};

        EXPECT_TRUE(loremIpsum.size() >= compressed.size());
        EXPECT_TRUE(loremIpsum.size() == decompressed.size());
        EXPECT_TRUE(memcmp(loremIpsum.data(), decompressed.data(), decompressed.size()) == 0);
    }
}

TEST(Compress, Headerless)
{
    const auto lzoStream{_T("LZOStream.exe")};

    for (const auto& format : Formats)
    {
        const auto compressed{LZOStreamCompress(lzoStream, loremIpsum.data(), loremIpsum.size(), format, true, true)};
        const auto decompressed{
            LZOStreamDecompress(lzoStream, compressed.data(), compressed.size(), true, format, loremIpsum.size())};

        EXPECT_TRUE(loremIpsum.size() >= compressed.size());
        EXPECT_TRUE(loremIpsum.size() == decompressed.size());
        EXPECT_TRUE(memcmp(loremIpsum.data(), decompressed.data(), decompressed.size()) == 0);
    }
}

TEST(Compress, Large)
{
    const auto        lzoStream{_T("LZOStream.exe")};
    std::vector<byte> large(64 * 1024 * 1024);

    memset(large.data(), 0, large.size());

    for (const auto& format : Formats)
    {
        const auto compressed{LZOStreamCompress(lzoStream, large.data(), large.size(), format, true, true)};
        const auto decompressed{
            LZOStreamDecompress(lzoStream, compressed.data(), compressed.size(), true, format, large.size())};

        EXPECT_TRUE(large.size() >= compressed.size());
        EXPECT_TRUE(large.size() == decompressed.size());
        EXPECT_TRUE(memcmp(large.data(), decompressed.data(), decompressed.size()) == 0);
    }
}

TEST(Compress, File)
{
    const auto lzoStream{_T("LZOStream.exe")};
    const auto appendix{std::to_tstring(GetCurrentProcessId()) + _T("_") + std::to_tstring(GetCurrentThreadId())};
    const auto inputFile{std::tstring(_T("Input_")) + appendix + _T(".txt")};

    EXPECT_TRUE(WriteData(inputFile.data(), loremIpsum.data(), loremIpsum.size()));

    for (const auto& format : Formats)
    {
        const auto compressedFile{std::tstring(_T("Compressed_")) + appendix + _T("(") + format + _T(").lzo")};
        const auto decompressedFile{std::tstring(_T("Decompressed_")) + appendix + _T("(") + format + _T(").txt")};

        EXPECT_TRUE(LZOStreamCompress(lzoStream, inputFile.data(), compressedFile.data(), format));
        EXPECT_TRUE(LZOStreamDecompress(lzoStream, compressedFile.data(), decompressedFile.data()));

        const auto decompressed{ReadString(decompressedFile.data())};

        DeleteFile(compressedFile.data());
        DeleteFile(decompressedFile.data());

        EXPECT_TRUE(loremIpsum == decompressed);
    }

    DeleteFile(inputFile.data());
}
