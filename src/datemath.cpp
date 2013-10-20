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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/program_options.hpp>
#include "Date.h"
#include "datemath.h"

namespace po = boost::program_options;
using namespace std;

// locale’s abbreviated weekday name
const char dayAbbreva[7][4] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
const int Sun2Mon[7] = {7,1,2,3,4,5,6};
const char dayOfWeek[7][10] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
const char monAbbrev[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Aug","Sep","Oct","Nov","Dec"};
const char monOfYear[12][10] = {"January","February","March","April","May","June","August","September","October","November","December"};

//--------------------------------------------------
//
//  Function: ProgramDetails
//
//------------------------------------------------
void ProgramDetails()
{
   cerr << "Datemath v" << PACKAGE_VERSION  << " "
        << "Copyright (C) 2013 " << AUTHOR << endl << endl
        << "Dateblock comes with ABSOLUTELY NO WARRANTY" << endl
        << "This is free software, and you are welcome to redistribute it" << endl
        << "under certain conditions; see man pages for details." << endl << endl

        << "This tool can be used to preform math using the current date/time." << endl
        << "Syntax: "
        << "datemath [options] [-f DateTimeFormat]" << endl << endl;
}

//--------------------------------------------------
//
//  Function: FindAndReplace
//
//------------------------------------------------
bool FindAndReplace(string &sourceString, const string &findString, const string &replaceString)
{
   string::size_type findStringLoc;
   bool foundAtLeastOne;

   if (findString == "" || findString == replaceString)
   {
      return false;
   }

   foundAtLeastOne = false;
   findStringLoc = 0;

   while ((findStringLoc = sourceString.find(findString, findStringLoc)) != string::npos)
   {
      sourceString.replace(findStringLoc, findString.length(), replaceString);
      findStringLoc += replaceString.length();

      foundAtLeastOne = true;
   }

   return(foundAtLeastOne);
}
//--------------------------------------------------
//
//  Function: FindAndReplace
//
//------------------------------------------------
bool FindAndReplace(string &sourceString, const string &findString, const int &replaceNum, int width , char fill)
{
   string replaceString;
   string::size_type findStringLoc;
   std::string s;
   std::stringstream out;
   out.width(width);
   out.fill(fill);
   out << replaceNum;
   replaceString = out.str();

   bool foundAtLeastOne;

   if (findString == "" || findString == replaceString)
   {
      return false;
   }

   foundAtLeastOne = false;
   findStringLoc = 0;

   while ((findStringLoc = sourceString.find(findString, findStringLoc)) != string::npos)
   {
      sourceString.replace(findStringLoc, findString.length(), replaceString);
      findStringLoc += replaceString.length();

      foundAtLeastOne = true;
   }

   return(foundAtLeastOne);
}

int main(int argc, char **argv)
{
   Date curTime;

   // Input Parameters
   string  strFormat = "%Y-%m-%d %H:%M:%S";
   long    lSecOffset=0;
   long    lMinOffset=0;
   long    lHourOffset=0;
   long    lDayOffset=0;
   long    lMonthOffset=0;
   long    lYearOffset=0;


   // Declare the supported options.
   po::options_description poAllOptions("Options");
   po::variables_map poVariablesMap;
   poAllOptions.add_options()
       ("help,h", "Show this help screen.")
       ("seconds,s", po::value<long>(), "Specify the offset (+/-) in seconds.")
       ("minutes,n", po::value<long>(), "Specify the offset (+/-) in minutes.")
       ("hours,o", po::value<long>(), "Specify the offset (+/-) in hours.")
       ("days,d", po::value<long>(), "Specify the offset (+/-) in days.")
       ("months,m", po::value<long>(), "Specify the offset (+/-) in months.")
       ("years,y", po::value<long>(), "Specify the offset (+/-) in years.")
       ("format,f", po::value<string>(), "Specify the desired output format (see $>man date). The default is: %Y-%m-%d %H:%M:%S")
   ;


   // Parse all the options and ensure the critical ones have been defined.
   // It's up to the application mainline to ensure application-specific
   // arguments have been specified on the command-line.
   try
   {
      po::store(po::parse_command_line(argc, argv, poAllOptions), poVariablesMap);
   }
   catch(exception& e)
   {
      cerr << "Error parsing command line: " << e.what() << endl;
      cerr << "Exiting." << endl;
      return -1;
   }
   po::notify(poVariablesMap);
   if (poVariablesMap.count("help"))
   {
      ProgramDetails();
      cerr << poAllOptions << endl;
      return 0;
   }

   if (! ( (poVariablesMap.count("seconds")) ||
         (poVariablesMap.count("minutes")) ||
         (poVariablesMap.count("hours")) ||
         (poVariablesMap.count("months")) ||
         (poVariablesMap.count("days")) ||
         (poVariablesMap.count("years"))
         )
      )
   {
      ProgramDetails();
      cerr << poAllOptions << endl;
      return 1;
   }

   // Store Seconds
   if ((poVariablesMap.count("seconds")))
   {
      lSecOffset = poVariablesMap["seconds"].as<long>();
   }
   // Store Minutes
   if ((poVariablesMap.count("minutes")))
   {
      lMinOffset = poVariablesMap["minutes"].as<long>();
   }
   // Store Hours
   if ((poVariablesMap.count("hours")))
   {
      lHourOffset = poVariablesMap["hours"].as<long>();
   }
   // Store Days
   if ((poVariablesMap.count("days")))
   {
      lDayOffset = poVariablesMap["days"].as<long>();
   }
   // Store Months
   if ((poVariablesMap.count("months")))
   {
      lMonthOffset = poVariablesMap["months"].as<long>();
   }
   // Store Years
   if ((poVariablesMap.count("years")))
   {
      lYearOffset = poVariablesMap["years"].as<long>();
   }

   // Store Format
   if ((poVariablesMap.count("format")))
   {
      strFormat = poVariablesMap["format"].as<string>();
   }

   curTime.AddYear(lYearOffset);
   curTime.AddMonth(lMonthOffset);
   curTime.AddDOM(lDayOffset);
   curTime.AddHour(lHourOffset);
   curTime.AddMin(lMinOffset);
   curTime.AddSec(lSecOffset);

   printf("%s\n",curTime.Str(strFormat).c_str());
   return 0;
}
