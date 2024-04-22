/* LZOStream\stdafx.cpp -- precompiled header definition

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

#include <atlbase.h>
#include "lzo/lzoconf.h"
#include "lzo/lzo1.h"
#include "lzo/lzo1a.h"
#include "lzo/lzo1b.h"
#include "lzo/lzo1c.h"
#include "lzo/lzo1f.h"
#include "lzo/lzo1x.h"
#include "lzo/lzo1y.h"
#include "lzo/lzo1z.h"
#include "lzo/lzo2a.h"
#pragma comment(lib, "lzo2.lib")
#include <ostream>
#include <string>
#include <iostream>

namespace std
{
#ifdef _UNICODE
constexpr wostream& tcout = wcout;
#else
constexpr ostream& tcout = cout;
#endif
using tstringstream = basic_stringstream<TCHAR, char_traits<TCHAR>, allocator<TCHAR>>;
using tstring       = basic_string<TCHAR>;
inline tstring to_tstring(int _Val)
{
    return _Integral_to_string<TCHAR>(_Val);
}
}
