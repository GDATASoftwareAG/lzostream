# lzostream
Lzostream is a windows command line tool that uses [Oberhumer LZO](http://www.oberhumer.com/opensource/lzo/) to compress and decompress files and streams.

It was written with Visual Studio in C++ for Windows and can easy be customized for other platforms.

# Usage
Run lzostream.exe in a command line without any arguments for a short help
```
LZOStream v1.0

Usage: LZOStream <command> [<option>...] [> output] [< input]

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

```
### Command c|compress
Compresses files or streams ('input.txt' is compressed to 'output.lzo')
```
lzostream c -i input.txt -o output.lzo
```
You can also stream data ('dir' output is compressed to 'dir.lzo')
```
dir | lzostream c -o dir.lzo
```
### Command d|decompress
Decompresses files or streams
```
lzostream d -i input.lzo -o output.txt
```
Streaming is also possible ('dir.lzo' is decompressed to 'dir.txt')
```
lzostream d > dir.txt < dir.lzo
```
### Command i|info
Displays header information. An lzostream header is written before the compressed data.
```
[0x00] HeaderId        : 0x1c4f5a4c (ok)
[0x04] FormatId        : 0xf7a9daf2 Lzo1x_999
[0x08] SourceSize      : 0x0000021b 539 (ok)
[0x0c] DestinationSize : 0x00000446 1094
[0x10] SourceHash      : 0x77908678 (ok)
[0x14] DestinationHash : 0x26c3f2a9
[0x18] HeaderHash      : 0x926eb563 (ok)
[0x1c] ...               0x00000237 567
```
* **HeaderId** is the magic number for the 'Lzostream header'
* **FormatId** is the magic number for the compression method
* **SourceSize** is the number of bytes for compressed data
* **DestinationSize** the number of bytes for uncompressed data
* **SourceHash** is the adler32 hash for the compressed data
* **DestinationHash** is the adler32 hash for the uncompressed data
* **HeaderHash** is the crc32 hash for this header (HeaderHash itself excluded)
### Option -i|--input \<file\>
Specifies the input file. File names with space should be enclosed in quotation marks.
### Option -o|--output \<file\>
Specifies the output file. File names with space should be enclosed in quotation marks.
### Option -f|--format \<method\>
Specifies the compression method. Default compression method is Lzo1x_999.
As the used method is part of the lzostream header the correct method is used for decompression.
In headerless mode you have to specify the used method.
### Option -h|--headerless
In headerless mode, no lzostream header is written during compression.
You must specify the compression method and the block size (target size) when decompressing.
### Option -l|--limitless
In limitless mode, compression does not check whether the compressed data has become larger.
Even ineffective compression method is then used.
### Option -b|--block \<size\>
In headerless decompression you have to specify the size of the decompressed data.
## Methods
More information on the possible compression methods can be found at [Oberhumer LZO](http://www.oberhumer.com/opensource/lzo/).
## License
The source code in this repository is distributed under the terms of [GNU General Public License v2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
A copy of the GPL license text is part of the repository.
## Acknowledgements
We would like to thank Markus Oberhumer [Oberhumer LZO](http://www.oberhumer.com/opensource/lzo/) who made this work possible.
Lzo was integrated with the Nuget package [lzo-msvc-x86](https://www.nuget.org/packages/lzo-msvc-x86).
