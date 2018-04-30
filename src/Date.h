/*
 Datetools provide a method of manipulating and working dates and times.
 Copyright (C) 2013-2018 Chris Caron <lead2gold@gmail.com>

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
#ifndef _DATE_H
#define _DATE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <time.h>
#include <string>
#include <sstream>
#include <set>
#include <vector>
#include <iostream> // temporary

using namespace std;

class Date
{
   public:
     // Some Min/Max Restrictions
     static const int T_SEC_MIN;
     static const int T_SEC_MAX;
     static const int T_MIN_MIN;
     static const int T_MIN_MAX;
     static const int T_HOUR_MIN;
     static const int T_HOUR_MAX;
     static const int T_MONTH_MIN;
     static const int T_MONTH_MAX;
     static const int T_YEAR_MIN;
     static const int T_YEAR_MAX;
     static const int T_DOM_MIN;
     static const int T_DOM_MAX;
     static const int T_DOW_MIN;
     static const int T_DOW_MAX;
     static const int T_DRIFT_MIN;
     static const int T_DRIFT_MAX;
     static const int T_NO_ENTRY;

     // ISC Cron Fields (5 of them)
     static const int ISC_CRON_FIELD_COUNT;

     // Dateblock Cron Fields (7 of them at this time) with the extension of
     // both seconds and drift)
     static const int DBL_CRON_FIELD_COUNT;

     Date();
     Date(int year,
          int month = 1,
          int day = 1,
          int hour = 0,
          int minute = 0,
          int second = 0);
     Date(const time_t localTime);
     Date(const struct tm& tmPtr);
     Date(const Date& dateObjIn);
     ~Date();

     const bool operator==(const Date&) const;
     const bool operator<(const Date&) const;
     const bool operator<=(const Date&) const;
     const bool operator>(const Date&) const;
     const bool operator>=(const Date&) const;
     const bool operator!=(const Date& right) const;

     // Assignments
     const Date & operator=(const Date&);
     const Date & operator=(const time_t &);
     const Date & operator=(const struct tm &);
     void Set(); // get Now
     void Set(int year,
              int month = 1,
              int day = 1,
              int hour = 0,
              int minute = 0,
              int second = 0);
     void Set(const Date&);
     void Set(const time_t &);
     void Set(const struct tm &);

     const bool AddSec(const int seconds = 1);
     const bool AddMin(const int minutes = 1);
     const bool AddHour(const int hours = 1);
     const bool AddDOM(const int days = 1);
     const bool AddMonth(const int months = 1);
     const bool AddYear(const int years = 1);

     const bool SetSec(const int seconds = 0);
     const bool SetMin(const int minutes = 0);
     const bool SetHour(const int hours = 0);
     const bool SetDOM(const int days = 1);
     const bool SetMonth(const int months = 1);
     const bool SetYear(const int years);

     const time_t Time()const;
     const struct tm& Tm()const;

     // returns a string based on passed in format
     const string Str(const string& format="%Y-%m-%d %H:%M:%S") const;

     const int Sec() const; // returns 0-59
     const int Min() const; // returns 0-59
     const int Hour() const; // returns 0-23
     const int DOM() const; // returns 1-31
     const int Month() const; // returns 1-12 Jan=1, Dec=12
     const int Year() const; // returns 1980 and +
     const int DOW() const; // returns 0-6

     const int MaxDOMsThisMonth() const; // returns 1-31
     const int MaxDOMsNextMonth() const; // returns 1-31
     const int MaxDOMsPrevMonth() const; // returns 1-31

     // A more advanced cron that calculates cron syntax such as:
     // 0-4,6      (which would handle 0,1,2,3,4,6)
     // 1,2,6,7,8  (which would handle 1,2,6,7,8)
     // */10       (which would handle 'every increment of 10',
     //             in the minute column; this would be similar
     //             to writing: 0,10,20,30,40,50)
     //
     // Valid ranges are no different then whats specified
     // above.

     // CronValid Tests Cron Strings and returns true
     // if they are parseable.
     //
     // sSecOffset, sMinOffset, and sHourOffset can all be set
     // to '*' as their default value;  They are not defined
     // as defaults below to remove any ambiguity with the other
     // CronValid() function
     static bool CronValid(const string& sSecOffset,
                           const string& sMinOffset,
                           const string& sHourOffset,
                           const string& sDomOffset   = "*",
                           const string& sMonthOffset = "*",
                           const string& sDowOffset   = "*",
                           const string& sDriftOffset = "*");

     static bool CronValid(const string& sCronStr="*  *  *  *  *  *  *",
                            bool isISC=false);

     // Cron
     //  -1 means not set.
     //  Valid Ranges are as follows:
     //     Second (0-59) Default=0
     //     Minute (0-59)
     //     Hour (0-23)
     //     DOM of Month (1-31)
     //     Month (1-11) {Jan=1,...,Dec=12}
     //     DOM of Week (0-6) {Sun=0,...,Sat=6}
     //     Drift Offset identifies how many seconds to adjust
     //     the final results by.
     //  Returns a Date Object of the next moment in time
     //  that matches specified cron entry
     //  By default lSecoffset is always set to 0 if 'nothing'
     //    else was specified.
     const Date Cron(int lSecOffset   = T_NO_ENTRY,
                     int lMinOffset   = T_NO_ENTRY,
                     int lHourOffset  = T_NO_ENTRY,
                     int lDomOffset   = T_NO_ENTRY,
                     int lMonthOffset = T_NO_ENTRY,
                     int lDowOffset   = T_NO_ENTRY,
                     int lDriftOffset = T_NO_ENTRY) const;

     // sSecOffset, sMinOffset, and sHourOffset can all be set
     // to '*' as their default value;  They are not defined
     // as defaults below to remove any ambiguity with the other
     // Cron() function
     const Date Cron(const string& sSecOffset,
                     const string& sMinOffset,
                     const string& sHourOffset,
                     const string& sDomOffset   = "*",
                     const string& sMonthOffset = "*",
                     const string& sDowOffset   = "*",
                     const string& sDriftOffset = "*") const;

     // Support the ISC Cron Formatting (same as above, but without
     // the seconds) (UNIX/Linux Crontab equivalent)
     //
     // day of week (0 - 6) (Sunday=0) -----------------------+
     // month (1 - 12) ------------------------------------+  |
     // day of month (1 - 31) --------------------------+  |  |
     // hour (0 - 23) -------------------------------+  |  |  |
     // min (0 - 59) -----------------------------+  |  |  |  |
     //                                           |  |  |  |  |
     //                                           -  -  -  -  -
     //                                   String: *  *  *  *  *
     //
     // Standard Formatting
     // drift time --------------------------------------------+
     // day of week (0 - 6) (Sunday=0) ---------------------+  |
     // month (1 - 12) ----------------------------------+  |  |
     // day of month (1 - 31) ------------------------+  |  |  |
     // hour (0 - 23) -----------------------------+  |  |  |  |
     // min (0 - 59) ---------------------------+  |  |  |  |  |
     // seconds(0 - 59) ---------------------+  |  |  |  |  |  |
     //                                      |  |  |  |  |  |  |
     //                                      -  -  -  -  -  -  -
     //                              String: *  *  *  *  *  *  *
     const Date Cron(const string& sCronStr="*  *  *  *  *  *  *",
                     bool isISC = false) const;

   private:
     struct tm m_tmObj;
     time_t m_time;

     // Used in conjunction of advanced
     // cron calculations
     enum CronComboOffset {
         COMBO_SEC = 0,
         COMBO_MIN,
         COMBO_HOUR,
         COMBO_DOM,
         COMBO_MONTH,
         COMBO_DOW,
         COMBO_DRIFT,
         COMBO_COUNT
      };

     // Internal Tool
     bool FindAndReplace(string &sourceString, const string &findString,
                const string &replaceString) const;
     bool FindAndReplace(string &sourceString, const string &findString,
                const int &replaceNum,
                int width = 1 ,
                char fill = '0') const;

     size_t Tokenize(const string& strIn, char delimiter,
                     vector<string> &tokensOut) const;

     bool ParseCronString(const string& strIn, set<int> &comboListOut,
                            int minVal, int maxVal) const;
};

inline const time_t Date::Time() const
{
   return m_time;
}

inline const struct tm& Date::Tm() const
{
   return m_tmObj;
}
inline const int Date::Sec() const
{
   return m_tmObj.tm_sec;
}
inline const int Date::Min() const
{
   return m_tmObj.tm_min;
}
inline const int Date::Hour() const
{
   return m_tmObj.tm_hour;
}
inline const int Date::DOM() const
{
   return m_tmObj.tm_mday;
}
inline const int Date::Month() const
{
   return m_tmObj.tm_mon + 1;
}
inline const int Date::DOW() const
{
   return m_tmObj.tm_wday;
}
inline const int Date::Year() const
{
   return m_tmObj.tm_year + 1900;
}
#endif
