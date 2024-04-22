/* LZOStream\LZOCommand.h -- command parser

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
#include "LZOHeader.h"
#include <vector>
#include <sstream>
#include <iomanip>

constexpr auto TitleVersion{_T("LZOStream v1.0")};

class LZOCommand
{
public:
    const uint32_t BufferSize{1024 * 1024};
    using Bytes = std::vector<byte>;

    enum class Command
    {
        None,
        Compress,
        Decompress,
        Info
    };
    enum class Option
    {
        None,
        Input,
        Output,
        Format,
        Block
    };

    LZOCommand()
    {
        if (lzo_init() != LZO_E_OK)
        {
            Error(std::errc::no_such_device);
        }
    }
    LZOCommand(const std::vector<LPCTSTR>& arguments)
    {
        if (lzo_init() == LZO_E_OK)
        {
            Process(arguments);
        }
        else
        {
            Error(std::errc::no_such_device);
        }
    }
    operator int() const
    {
        return _error;
    }

    int Process(const std::vector<LPCTSTR>& arguments)
    {
        int error{Parse(arguments)};

        if (error)
        {
            return error;
        }

        if (_debugger)
        {
            while (!IsDebuggerPresent())
            {
                Sleep(100);
            }
        }

        if (_command == Command::Compress)
        {
            return Compress();
        }
        if (_command == Command::Decompress)
        {
            return Decompress();
        }
        if (_command == Command::Info)
        {
            return Info();
        }

        return Help();
    }

    int Compress()
    {
        const auto input{Input()};

        if (input.empty())
        {
            return _error;
        }

        if (_format == LZOFormat::Id::None)
        {
            _format = LZOFormat::Id::Default;
        }

        try
        {
            const auto info{LZOFormat::FormatInfo(_format)};

            if (!info || !info->FunctionCompress)
            {
                return Error(std::errc::not_supported);
            }

            lzo_uint compressedSize{input.size() + input.size() / 16 + 64 + 3};

            if (_headerLess)
            {
                Bytes compressed(compressedSize);
                Bytes work(info->MemoryCompress);
                int   result{};

                try
                {
                    result = info->FunctionCompress(
                        input.data(), input.size(), compressed.data(), &compressedSize, work.data());
                }
                catch (std::exception&)
                {
                    result = -1;
                }

                if (result == LZO_E_OK && (_limitLess || compressedSize < input.size()))
                {
                    compressed.resize(compressedSize);

                    return Output(compressed);
                }

                return Error(std::errc::bad_address);
            }

            Bytes compressed(LZOHeader::Size(compressedSize));
            auto  header{LZOHeader::Header(compressed.data(), compressed.size())};
            Bytes work(info->MemoryCompress);
            int   result{};

            try
            {
                result =
                    info->FunctionCompress(input.data(), input.size(), header->Data(), &compressedSize, work.data());
            }
            catch (std::exception&)
            {
                result = -1;
            }

            if (result == LZO_E_OK && (_limitLess || compressedSize < input.size()))
            {
                const auto sourceHash{lzo_adler32(0, header->Data(), compressedSize)};
                const auto destinationHash{lzo_adler32(0, input.data(), input.size())};

                header->Initialize(_format, compressedSize, input.size(), sourceHash, destinationHash);
                compressed.resize(LZOHeader::Size(compressedSize));

                return Output(compressed);
            }

            lzo_uint   size{input.size()};
            const auto sourceDestinationHash{lzo_adler32(0, input.data(), size)};

            memcpy_s(header->Data(), compressedSize, input.data(), size);

            header->Initialize(LZOFormat::Id::None, size, size, sourceDestinationHash, sourceDestinationHash);
            compressed.resize(LZOHeader::Size(size));

            return Output(compressed);
        }
        catch (std::exception&)
        {
            return Error(std::errc::not_enough_memory);
        }

        return Error({});
    }

    int Decompress()
    {
        const auto input{Input()};

        if (input.empty())
        {
            return _error;
        }

        try
        {
            if (_headerLess)
            {
                const auto info{LZOFormat::FormatInfo(_format)};

                if (!info || !info->FunctionDecompress)
                {
                    return Error(std::errc::not_supported);
                }

                lzo_uint decompressedSize{_block};
                Bytes    decompressed(decompressedSize);
                Bytes    work(info->MemoryDecompress);
                int      result{};

                try
                {
                    result = info->FunctionDecompress(
                        input.data(), input.size(), &decompressed[0], &decompressedSize, work.data());
                }
                catch (std::exception&)
                {
                    result = -1;
                }

                if (result == LZO_E_OK)
                {
                    decompressed.resize(decompressedSize);

                    return Output(decompressed);
                }

                return Error(std::errc::illegal_byte_sequence);
            }

            const auto header{LZOHeader::Header(input.data(), input.size(), true)};

            if (header)
            {
                if (header->FormatId == LZOFormat::Id::None)
                {
                    return Output(input, LZOHeader::Size(0));
                }

                const auto info{LZOFormat::FormatInfo(header->FormatId)};

                if (!info || !info->FunctionDecompress)
                {
                    return Error(std::errc::not_supported);
                }
                if (header->SourceHash != 0 && header->SourceHash != lzo_adler32(0, header->Data(), header->SourceSize))
                {
                    return Error(std::errc::illegal_byte_sequence);
                }

                lzo_uint decompressedSize{header->DestinationSize};
                Bytes    decompressed(decompressedSize);
                Bytes    work(info->MemoryDecompress);
                int      result{};

                try
                {
                    result = info->FunctionDecompress(
                        header->Data(), header->SourceSize, &decompressed[0], &decompressedSize, work.data());
                }
                catch (std::exception&)
                {
                    result = -1;
                }

                if (result == LZO_E_OK)
                {
                    if (header->DestinationHash != 0 &&
                        header->DestinationHash != lzo_adler32(0, decompressed.data(), decompressedSize))
                    {
                        return Error(std::errc::illegal_byte_sequence);
                    }

                    decompressed.resize(decompressedSize);

                    return Output(decompressed);
                }

                return Error(std::errc::bad_address);
            }
        }
        catch (std::exception&)
        {
            return Error(std::errc::not_enough_memory);
        }

        return Error({});
    }

    int Info()
    {
        const auto input{Input()};

        if (input.empty())
        {
            return _error;
        }

        auto header{LZOHeader::Header(input.data(), input.size())};

        if (!header)
        {
            Output("Header (not available)");

            return Error(std::errc::illegal_byte_sequence);
        }

        std::stringstream stream;
        auto              valid{header->Valid()};
        auto              info{LZOFormat::FormatInfo(header->FormatId)};

        stream << "[0x00] HeaderId        :" << Hex(header->HeaderId) << Ok(header->HeaderId == LZOHeaderId)
               << std::endl;
        stream << "[0x04] FormatId        :" << Hex(header->FormatId) << " " << ((info) ? info->Name : "Unknown")
               << std::endl;
        stream << "[0x08] SourceSize      :" << Hex(header->SourceSize) << " " << header->SourceSize
               << Ok(input.size() >= LZOHeader::Size(header->SourceSize)) << std::endl;
        stream << "[0x0c] DestinationSize :" << Hex(header->DestinationSize) << " " << header->DestinationSize
               << std::endl;
        stream << "[0x10] SourceHash      :" << Hex(header->SourceHash)
               << Ok(input.size() >= LZOHeader::Size(header->SourceSize) &&
                      header->SourceHash == lzo_adler32(0, header->Data(), header->SourceSize))
               << std::endl;
        stream << "[0x14] DestinationHash :" << Hex(header->DestinationHash) << std::endl;
        stream << "[0x18] HeaderHash      :" << Hex(header->HeaderHash) << Ok(valid) << std::endl;
        stream << "[0x1c] ...              " << Hex(input.size()) << " " << input.size() << std::endl;

        return Output(stream.str());
    }

    int Help()
    {
        std::tstringstream stream;

        stream << _T(R"(Usage: LZOStream <command> [<option>...] [> output] [< input]

<Commands>
    c|compress              Compress    (-i -o -f -h -l)
    d|decompress            Decompress  (-i -o -f -h -b)
    i|info                  Info        (-i -o)

<Options> 
    -i|--input <file>       Input file
    -o|--output <file>      Output file
    -f|--format <method>    Compression method (compress/ decompress headerless)
    -h|--headerless         Headerless output (compress)
    -l|--limitless          No limitation (compress: data maybe larger)
    -b|--block <size>       Block size (decompress: headerless)

<Methods>
    Lzo1,  Lzo1_99
    Lzo1a, Lzo1a_99
    Lzo1b, Lzo1b_1 ... Lzo1b_9, Lzo1b_99, Lzo1b_999
    Lzo1c, Lzo1c_1 ... Lzo1c_9, Lzo1c_99, Lzo1c_999
    Lzo1f, Lzo1f_1, Lzo1f_999
    Lzo1x, Lzo1x_1, Lzo1x_1_11, Lzo1x_1_12, Lzo1x_1_15, Lzo1x_999
    Lzo1y, Lzo1y_1, Lzo1y_999
    Lzo1z, Lzo1z_999
    Lzo2a, Lzo2a_999
)");
        Message(stream.str().data());

        return Error({});
    }

    int Parse(const std::vector<LPCTSTR>& arguments)
    {
        Option option{};

        for (const auto& argument : arguments)
        {
            if (option == Option::Input)
            {
                _input = argument;
                option = {};
            }
            else if (option == Option::Output)
            {
                _output = argument;
                option  = {};
            }
            else if (option == Option::Format)
            {
                _format = LZOFormat::FormatId(argument);

                if (_format == LZOFormat::Id::None)
                {
                    Message(_T("Unknown format"), argument);

                    return Error(std::errc::invalid_argument);
                }
                option = {};
            }
            else if (option == Option::Block)
            {
                if (argument && isdigit((byte)*argument))
                {
                    _block = _tstol(argument);
                }
                else
                {
                    Message(_T("Unknown size"), argument);

                    return Error(std::errc::invalid_argument);
                }
                option = {};
            }
            else if (Equals(argument, {_T("c"), _T("compress")}))
            {
                _command = Command::Compress;
            }
            else if (Equals(argument, {_T("d"), _T("decompress")}))
            {
                _command = Command::Decompress;
            }
            else if (Equals(argument, {_T("i"), _T("info")}))
            {
                _command = Command::Info;
            }
            else if (Equals(argument, {_T("-i"), _T("--input")}))
            {
                option = Option::Input;
            }
            else if (Equals(argument, {_T("-o"), _T("--output")}))
            {
                option = Option::Output;
            }
            else if (Equals(argument, {_T("-f"), _T("--format")}))
            {
                option = Option::Format;
            }
            else if (Equals(argument, {_T("-b"), _T("--block"), _T("--blocksize")}))
            {
                option = Option::Block;
            }
            else if (Equals(argument, {_T("-h"), _T("--headerless")}))
            {
                _headerLess = true;
            }
            else if (Equals(argument, {_T("-l"), _T("--limitless")}))
            {
                _limitLess = true;
            }
            else if (Equals(argument, {_T("-d"), _T("--debug")}))
            {
                _debugger = true;
            }
            else
            {
                Message(_T("Unknown"), argument);

                return Error(std::errc::illegal_byte_sequence);
            }
        }

        return {};
    }

    Bytes Input()
    {
        Bytes bytes;

        if (_input.empty())
        {
            HANDLE handle{GetStdHandle(STD_INPUT_HANDLE)};

            if (handle == INVALID_HANDLE_VALUE)
            {
                Message(_T("Error reading input"));
                Error(std::errc::no_such_device);

                return {};
            }

            try
            {
                Bytes buffer(BufferSize);
                DWORD read{};

                while (ReadFile(handle, buffer.data(), (DWORD)buffer.size(), &read, nullptr) && read)
                {
                    bytes.insert(bytes.end(), buffer.begin(), buffer.begin() + read);
                }
            }
            catch (std::exception&)
            {
                Message(_T("Error reading input"));
                Error(std::errc::not_enough_memory);

                return {};
            }
        }
        else
        {
            HANDLE handle{
                CreateFile(_input.data(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr)};

            if (handle == INVALID_HANDLE_VALUE)
            {
                Message(_T("Error opening "), _input.data());
                Error(std::errc::no_such_file_or_directory);

                return {};
            }

            LARGE_INTEGER size;

            if (GetFileSizeEx(handle, &size) && !size.HighPart)
            {
                DWORD read{};

                try
                {
                    bytes.resize(size.LowPart);

                    if (!ReadFile(handle, &bytes[0], size.LowPart, &read, nullptr))
                    {
                        bytes.clear();
                    }
                }
                catch (std::exception&)
                {
                    Message(_T("Error reading "), _input.data());
                    Error(std::errc::not_enough_memory);
                }
            }
            else
            {
                Message(_T("Error sizing "), _input.data());
                Error(std::errc::bad_address);
            }

            CloseHandle(handle);
        }

        return bytes;
    }

    int Output(const Bytes& bytes, const size_t offset = {})
    {
        if (_output.empty())
        {
            HANDLE handle{GetStdHandle(STD_OUTPUT_HANDLE)};

            if (handle == INVALID_HANDLE_VALUE)
            {
                Message(_T("Error writing output"));

                return Error(std::errc::no_such_device);
            }

            DWORD written{};

            if (!WriteFile(handle, bytes.data() + offset, bytes.size() - offset, &written, nullptr))
            {
                Message(_T("Error writing output"));

                return Error(std::errc::bad_address);
            }
        }
        else
        {
            HANDLE handle{
                CreateFile(_output.data(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr)};

            if (handle == INVALID_HANDLE_VALUE)
            {
                Message(_T("Error creating "), _output.data());

                return Error(std::errc::no_such_file_or_directory);
            }

            DWORD written{};

            if (!WriteFile(handle, bytes.data() + offset, bytes.size() - offset, &written, nullptr))
            {
                CloseHandle(handle);

                Message(_T("Error writing "), _output.data());

                return Error(std::errc::bad_address);
            }

            CloseHandle(handle);
        }

        return Error({});
    }

    int Output(const std::string& string)
    {
        const auto bytes{Bytes(string.data(), string.data() + string.size())};

        return Output(bytes);
    }

    int Output(const uint32_t& value)
    {
        return Output(std::to_string(value));
    }

    int Error(const std::errc& error)
    {
        return _error = (int)error;
    }

    static bool Equals(const std::tstring& string, const std::vector<LPCTSTR>& arguments)
    {
        if (string.empty())
        {
            return false;
        }

        for (const auto& argument : arguments)
        {
            if (_tcsicmp(string.data(), argument) == 0)
            {
                return true;
            }
        }

        return false;
    }

    static void Message(LPCTSTR message = {}, LPCTSTR argument = {})
    {
        std::tcout << TitleVersion << std::endl << std::endl;

        if (message)
        {
            std::tcout << message;

            if (argument)
            {
                std::tcout << argument;
            }
            std::tcout << std::endl;
        }
    }

    template <typename T>
    static std::string Hex(T value)
    {
        std::stringstream stream;

        stream << " 0x" << std::setfill('0') << std::setw(8) << std::hex << (uint32_t)value;

        return stream.str();
    }
    static std::string Ok(bool ok)
    {
        return (ok) ? " (ok)" : "";
    }

private:
    Command       _command{};
    std::tstring  _input;
    std::tstring  _output;
    LZOFormat::Id _format{LZOFormat::Id::None};
    bool          _headerLess{};
    bool          _limitLess{};
    bool          _debugger{};
    uint32_t      _block{};
    int           _error{};
};
