/* LZOStreamTest\LZOStreamCall.h -- LZOStream application wrapper

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

inline std::vector<byte> LZOStreamCall(LPCTSTR lzoStreamExe, LPCTSTR arguments, const void* input, size_t size)
{
    constexpr auto      bufferSize{1024 * 1024};
    SECURITY_ATTRIBUTES attributes{sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};
    HANDLE              inputRead{};
    HANDLE              inputWrite{};
    HANDLE              outputRead{};
    HANDLE              outputWrite{};

    if (!CreatePipe(&outputRead, &outputWrite, &attributes, 0) ||
        !SetHandleInformation(outputRead, HANDLE_FLAG_INHERIT, 0) ||
        !CreatePipe(&inputRead, &inputWrite, &attributes, 0) ||
        !SetHandleInformation(inputWrite, HANDLE_FLAG_INHERIT, 0))
    {
        CloseHandle(outputRead);
        CloseHandle(outputWrite);
        CloseHandle(inputRead);
        CloseHandle(inputWrite);

        return {};
    }

    const auto          commandLine{std::tstring(lzoStreamExe) + _T(" ") + arguments};
    PROCESS_INFORMATION processInfo{};
    STARTUPINFO         startupInfo{};

    startupInfo.cb         = sizeof(STARTUPINFO);
    startupInfo.hStdError  = outputWrite;
    startupInfo.hStdOutput = outputWrite;
    startupInfo.hStdInput  = inputRead;
    startupInfo.dwFlags |= STARTF_USESTDHANDLES;

    const auto        process{CreateProcess(
        nullptr, (LPTSTR)commandLine.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startupInfo, &processInfo)};
    std::vector<byte> output;

    if (process)
    {
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);

        if (input && size)
        {
            DWORD written{};

            WriteFile(inputWrite, input, size, &written, nullptr);
        }
    }

    CloseHandle(inputWrite);
    CloseHandle(outputWrite);
    CloseHandle(inputRead);

    if (process)
    {
        std::vector<byte> buffer(bufferSize);
        DWORD             read{};

        while (ReadFile(outputRead, buffer.data(), (DWORD)buffer.size(), &read, nullptr))
        {
            output.insert(output.end(), buffer.begin(), buffer.begin() + read);
        }
    }

    CloseHandle(outputRead);

    return output;
}

inline std::vector<byte> LZOStreamCompress(LPCTSTR lzoStreamExe, const void* input, size_t size,
    LPCTSTR format = _T("Lzo1x_999"), bool headerless = {}, bool limitless = {})
{
    std::tstring commandLine{_T("c")};

    if (format)
    {
        commandLine += _T(" -f ");
        commandLine += format;
    }
    if (headerless)
    {
        commandLine += _T(" -h");
    }
    if (limitless)
    {
        commandLine += _T(" -l");
    }

    return LZOStreamCall(lzoStreamExe, commandLine.data(), input, size);
}

inline std::vector<byte> LZOStreamDecompress(LPCTSTR lzoStreamExe, const void* input, size_t size, bool headerless = {},
    LPCTSTR format = {}, uint32_t blockSize = {})
{
    std::tstring commandLine{_T("d")};

    if (headerless)
    {
        commandLine += _T(" -h");
    }
    if (format)
    {
        commandLine += _T(" -f ");
        commandLine += format;
    }
    if (blockSize)
    {
        commandLine += _T(" -b ");
        commandLine += std::to_tstring(blockSize);
    }

    return LZOStreamCall(lzoStreamExe, commandLine.data(), input, size);
}

inline bool LZOStreamCall(LPCTSTR lzoStreamExe, LPCTSTR arguments)
{
    const auto          commandLine{std::tstring(lzoStreamExe) + _T(" ") + arguments};
    PROCESS_INFORMATION processInfo{};
    STARTUPINFO         startupInfo{};
    bool                result{};

    startupInfo.cb = sizeof(STARTUPINFO);

    if (CreateProcess(nullptr, (LPTSTR)commandLine.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startupInfo,
            &processInfo))
    {
        if (WaitForSingleObject(processInfo.hProcess, 10000) == WAIT_TIMEOUT)
        {
            TerminateProcess(processInfo.hProcess, -1);
        }
        else
        {
            result = true;
        }
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }

    return result;
}

inline bool LZOStreamCompress(LPCTSTR lzoStreamExe, LPCTSTR input, LPCTSTR output, LPCTSTR format = _T("Lzo1x_999"))
{
    std::tstring commandLine{_T("c")};

    commandLine += _T(" -i \"");
    commandLine += input;
    commandLine += _T("\" -o \"");
    commandLine += output;
    commandLine += _T("\"");

    if (format)
    {
        commandLine += _T(" -f ");
        commandLine += format;
    }

    return LZOStreamCall(lzoStreamExe, commandLine.data());
}

inline bool LZOStreamDecompress(LPCTSTR lzoStreamExe, LPCTSTR input, LPCTSTR output)
{
    std::tstring commandLine{_T("d")};

    commandLine += _T(" -i \"");
    commandLine += input;
    commandLine += _T("\" -o \"");
    commandLine += output;
    commandLine += _T("\"");

    return LZOStreamCall(lzoStreamExe, commandLine.data());
}
