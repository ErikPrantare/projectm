/**
 * projectM -- Milkdrop-esque visualisation SDK
 * Copyright (C)2003-2007 projectM Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * See 'LICENSE.txt' included within this release
 *
 */
/**
 * $Id$
 *
 * $Log$
 */

#ifndef COMMON_HPP
#define COMMON_HPP
#include <cassert>
#include <cstdarg>
#include <locale>
#include <typeinfo>
#include <vector>

#ifdef _MSC_VER
#define strcasecmp(s, t) _strcmpi(s, t)
#endif

#if defined(_MSC_VER) && !defined(EYETUNE_WINRT)
#pragma warning(disable : 4244 4305 4996; once : 4018)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
typedef unsigned int uint;
#endif

#ifdef __APPLE__
#include <cstdio>
extern FILE* fmemopen(void* buf, size_t len, const char* pMode);
#endif /** MACOS */

#define DEFAULT_FONT_PATH "/home/carm/fonts/courier1.glf"
#define MAX_TOKEN_SIZE 512
#define MAX_PATH_SIZE 4096

#define STRING_BUFFER_SIZE 1024 * 150
#define STRING_LINE_SIZE 1024


#define MAX_DOUBLE_SIZE 10000000.0
#define MIN_DOUBLE_SIZE -10000000.0

#define MAX_INT_SIZE 10000000
#define MIN_INT_SIZE -10000000

/* default float initial value */
#define DEFAULT_DOUBLE_IV 0.0

/* default float lower bound */
#define DEFAULT_DOUBLE_LB MIN_DOUBLE_SIZE

/* default float upper bound */
#define DEFAULT_DOUBLE_UB MAX_DOUBLE_SIZE

/** Per-platform path separators */
#define WIN32_PATH_SEPARATOR '\\'
#define UNIX_PATH_SEPARATOR '/'
#ifdef WIN32
#define PATH_SEPARATOR WIN32_PATH_SEPARATOR
#else
#define PATH_SEPARATOR UNIX_PATH_SEPARATOR
#endif /** WIN32 */
#include <algorithm>
#include <string>

const unsigned int NUM_Q_VARIABLES(32);
const std::string PROJECTM_FILE_EXTENSION("prjm");
const std::string MILKDROP_FILE_EXTENSION("milk");
const std::string PROJECTM_MODULE_EXTENSION("so");


inline std::string parseExtension(const std::string& filename)
{

    const std::size_t start = filename.find_last_of('.');

    if (start == std::string::npos || start >= (filename.length() - 1))
        return "";
    std::string ext = filename.substr(start + 1, filename.length());
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    return ext;
}

inline std::string parseFilename(const std::string& filename)
{

    const std::size_t start = filename.find_last_of('/');

    if (start == std::string::npos || start >= (filename.length() - 1))
        return "";
    else
        return filename.substr(start + 1, filename.length());
}

inline double meanSquaredError(const double& x, const double& y)
{
    return (x - y) * (x - y);
}

inline auto CaseInsensitiveSubstringFind(std::string const& haystack, std::string const& needle, const std::locale& loc = std::locale()) -> size_t
{
    auto const it = std::search(
        haystack.cbegin(),
        haystack.cend(),
        needle.cbegin(),
        needle.cend(),
        [&loc](char ch1, char ch2) {
            return std::toupper(ch1, loc) == std::toupper(ch2, loc);
        });

    if (it != haystack.end())
    {
        return it - haystack.begin();
    }

    return std::string::npos;
}

enum PresetRatingType
{
    FIRST_RATING_TYPE = 0,
    HARD_CUT_RATING_TYPE = FIRST_RATING_TYPE,
    SOFT_CUT_RATING_TYPE,
    LAST_RATING_TYPE = SOFT_CUT_RATING_TYPE,
    TOTAL_RATING_TYPES = SOFT_CUT_RATING_TYPE + 1
};


typedef std::vector<int> RatingList;

#endif
