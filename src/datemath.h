/*
 Datetools provide a method of manipulating and working dates and times.
 Copyright (C) 2013 Chris Caron <lead2gold@gmail.com>
 
 This file is part of Datetools.  Datetools is free software; you can
 redistribute it and/or modify it under the terms of the GNU General Public
 License as published by the Free Software Foundation; either version 2 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/
#ifndef _DATE_MATH_H_
#define _DATE_MATH_H_
#include <sstream>


#define  AUTHOR       "Chris Caron <lead2gold@gmail.com>"
#define  ONEMIN       (60)
#define  ONEHOUR      (60*60)
#define  ONEDAY       (60*60*24)

using namespace std;

/// ProgramDetails displays help information
void ProgramDetails();

/// FindAndReplace searches a string for occurances of a substring and
/// replace those found with another string
///
/// \param[in] sourceString The input string to search and modify
/// \param[in] findString The string to search for
/// \param[in] replaceString The string to replace findString with
///
/// \return true, if at least one occurance of findString was found
/// false, if:
/// - no occurances of findString were found
/// - or, if findString == replaceString
/// - or, if findString == ""

template <class T>
inline string to_string (const T& t)
{
   std::stringstream ss;
   ss << t;
   return ss.str();
}

bool FindAndReplace(
      string &sourceString,
      const string &findString,
      const string &replaceString);

bool FindAndReplace(string &sourceString,
      const string &findString,
      const int &replaceNum,
      int width = 1 ,
      char fill = '0' );
#endif
