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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <time.h>
#include <string.h>
#include <limits.h>
#include <set>
#include <vector>
#include <ctype.h>
#include "Date.h"

#include <boost/tokenizer.hpp>

// Boost Character Tokenizer Simplified
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

using namespace std;

namespace {
   // locale’s abbreviated weekday name
   const char dayAbbreva[7][4] =
       {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
   const int Sun2Mon[7] =
      {7,1,2,3,4,5,6};
   const char dayOfWeek[7][10] =
      {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
   const char monAbbrev[12][4] =
      {"Jan","Feb","Mar","Apr","May","Jun","Aug","Sep","Oct","Nov","Dec"};
   const char monOfYear[12][10] =
      {"January","February","March","April","May","June","August","September","October","November","December"};
}

// Limits
const int Date::T_SEC_MIN=0;
const int Date::T_SEC_MAX=59;
const int Date::T_MIN_MIN=0;
const int Date::T_MIN_MAX=59;
const int Date::T_HOUR_MIN=0;
const int Date::T_HOUR_MAX=23;
const int Date::T_DOM_MIN=1;
const int Date::T_DOM_MAX=31;
const int Date::T_MONTH_MIN=1;
const int Date::T_MONTH_MAX=12;
const int Date::T_YEAR_MIN=0;
const int Date::T_YEAR_MAX=INT_MAX;
const int Date::T_DOW_MIN=0;
const int Date::T_DOW_MAX=6;

// Drift Defaults
const unsigned long Date::T_NO_DRIFT=0;
// Presently our AddSec() function only handles integers as input and therefore
// we need to set the max to the maximum values of a 'signed integer' AddSec()
// will always take a signed value since we allow the decrementing of time by
// passing in a negative number
const unsigned long Date::T_DRIFT_MAX=INT_MAX;

//------------------------------------------------
//
//  Function: Constructor (now)
//
//------------------------------------------------
Date::Date()
{
   Set();
}

//------------------------------------------------
//
//  Function: Constructor (accepts YYYY, MM, DD, hh, mm, ss)
//
//------------------------------------------------
Date::Date(int year, int mon, int day,
           int hour, int min, int sec)
{
   Set(year, mon, day, hour, min, sec);
}

//------------------------------------------------
//
//  Function: Constructor (uses Date object)
//
//------------------------------------------------
Date::Date(const Date & dObj)
{
   Set(dObj);
}

//------------------------------------------------
//
//  Function: Constructor (uses time_t type)
//
//------------------------------------------------
Date::Date(const time_t localTime)
{
   Set(localTime);
}

//------------------------------------------------
//
//  Function: Constructor (uses struct tm object)
//
//------------------------------------------------
Date::Date(const struct tm& tmPtr)
{
   Set(tmPtr);
}

//------------------------------------------------
//
//  Function: Deconstructor
//
//------------------------------------------------
Date::~Date()
{
}

//------------------------------------------------
//
//  Function: Assignment '=' (uses Date object)
//
//------------------------------------------------
const Date & Date::operator=(const Date& right)
{
   memcpy(&m_tmObj,&right.m_tmObj,sizeof(struct tm));
   m_time = right.m_time;
   return (*this);
}

//------------------------------------------------
//
//  Function: Assignment '=' (uses time_t type)
//
//------------------------------------------------
const Date & Date::operator=(const time_t &right)
{
   m_time = right;
   struct tm* tmPtr=localtime(&m_time);
   memcpy(&m_tmObj,tmPtr,sizeof(struct tm));
   return (*this);
}

//------------------------------------------------
//
//  Function: Assignment '=' (uses struct tm object)
//
//------------------------------------------------
const Date & Date::operator=(const struct tm &right)
{
   memcpy(&m_tmObj,&right,sizeof(struct tm));
   m_time = mktime(&m_tmObj);
   return (*this);
}

//------------------------------------------------
//
//  Function: Set (now)
//
//------------------------------------------------
void Date::Set()
{
   m_time=time((time_t *) NULL);
   struct tm* tmPtr=localtime(&m_time);
   memcpy(&m_tmObj,tmPtr,sizeof(struct tm));
}

//------------------------------------------------
//
//  Function: Set (accepts YYYY, MM, DD, hh, mm, ss)
//
//------------------------------------------------
void Date::Set(int year, int mon, int day,
               int hour, int min, int sec)
{
   m_time=time((time_t *) NULL);
   struct tm* tmPtr=localtime(&m_time);
   memcpy(&m_tmObj,tmPtr,sizeof(struct tm));

   // Now Set Defaults
   SetYear(year);
   SetMonth(mon);
   SetDOM(day);
   SetHour(hour);
   SetMin(min);
   SetSec(sec);
}

//------------------------------------------------
//
//  Function: Set (uses Date object)
//
//------------------------------------------------
void Date::Set(const Date& dObj)
{
   (*this) = dObj;
}

//------------------------------------------------
//
//  Function: Set (uses time_t type)
//  where tVal is the EPOCH Time (number of seconds
//  since 1970-01-01 00:00:01)
//
//------------------------------------------------
void Date::Set(const time_t &tVal)
{
   m_time = tVal;
   struct tm* tmPtr=localtime(&m_time);
   memcpy(&m_tmObj,tmPtr,sizeof(struct tm));
}

//------------------------------------------------
//
//  Function: Set (uses struct tm object)
//
//------------------------------------------------
void Date::Set(const struct tm &tmObj)
{
   memcpy(&m_tmObj,&tmObj,sizeof(struct tm));
   m_time = mktime(&m_tmObj);
}

//------------------------------------------------
//
//  Function: Operator ==
//
//------------------------------------------------
const bool Date::operator==(const Date& right) const
{
   return (m_time == right.m_time);
}

//------------------------------------------------
//
//  Function: Operator <
//
//------------------------------------------------
const bool Date::operator<(const Date& right) const
{
   return (m_time < right.m_time);
}

//------------------------------------------------
//
//  Function: Operator <=
//
//------------------------------------------------
const bool Date::operator<=(const Date& right) const
{
   return (m_time <= right.m_time);
}

//------------------------------------------------
//
//  Function: Operator >
//
//------------------------------------------------
const bool Date::operator>(const Date& right) const
{
   return (m_time > right.m_time);
}

//------------------------------------------------
//
//  Function: Operator >=
//
//------------------------------------------------
const bool Date::operator>=(const Date& right) const
{
   return (m_time >= right.m_time);
}

//------------------------------------------------
//
//  Function: MaxDOMsThisMonth
//
//------------------------------------------------
const int Date::MaxDOMsThisMonth() const
{
   struct tm* tmPtr;
   struct tm tmTime;
   time_t tTmp;
   memcpy(&tmTime,&m_tmObj,sizeof(struct tm));
   if(tmTime.tm_mon ==11)
   {
      tmTime.tm_mon=0;
      tmTime.tm_year++;
   }
   else
   {
      tmTime.tm_mon++;
   }
   tmTime.tm_mday=1;
   tTmp=mktime(&tmTime);
   tTmp-=86400; // Take off 1 DOM
   tmPtr = localtime(&tTmp);
   return tmPtr->tm_mday;
}

//------------------------------------------------
//
//  Function: MaxDOMsNextMonth
//
//------------------------------------------------
const int Date::MaxDOMsNextMonth() const
{
   struct tm* tmPtr;
   struct tm tmTime;
   time_t tTmp;
   memcpy(&tmTime,&m_tmObj,sizeof(struct tm));
   if(tmTime.tm_mon ==10)
   {
      tmTime.tm_mon=0;
      tmTime.tm_year+=2;
   }
   else
   {
      tmTime.tm_mon+=2;
   }
   tmTime.tm_mday=1;
   tTmp=mktime(&tmTime);
   tTmp-=86400; // Take off 1 DOM
   tmPtr = localtime(&tTmp);
   return tmPtr->tm_mday;
}

//------------------------------------------------
//
//  Function: MaxDOMsPrevMonth
//
//------------------------------------------------
const int Date::MaxDOMsPrevMonth() const
{
   struct tm* tmPtr;
   struct tm tmTime;
   time_t tTmp;
   memcpy(&tmTime,&m_tmObj,sizeof(struct tm));
   if(tmTime.tm_mon ==0)
   {
      tmTime.tm_mon=11;
      tmTime.tm_year--;
   }
   else
   {
      tmTime.tm_mon--;
   }
   tmTime.tm_mday=1;

   tTmp=mktime(&tmTime);
   tTmp-=86400; // Take off 1 DOM
   tmPtr = localtime(&tTmp);
   return tmPtr->tm_mday;
}


//--------------------------------------------------
//
//  Function: AddSec
//
//------------------------------------------------
const bool Date::AddSec(const int seconds)
{
   // Aquire Time Now
   struct tm *tmPtr;
   time_t tTmp=mktime(&m_tmObj)+seconds;
   tmPtr = localtime(&tTmp);
   if (tmPtr == NULL)
   {
      return false;
   }

   m_time=tTmp;
   memcpy(&m_tmObj,tmPtr,sizeof(struct tm));
   return true;
}

//--------------------------------------------------
//
//  Function: AddMin
//
//------------------------------------------------
const bool Date::AddMin(const int minutes)
{
   // Aquire Time Now
   struct tm *tmPtr;
   time_t tTmp=mktime(&m_tmObj)+(minutes*60);
   tmPtr = localtime(&tTmp);
   if (tmPtr == NULL)
   {
      return false;
   }

   m_time=tTmp;
   memcpy(&m_tmObj,tmPtr,sizeof(struct tm));
   return true;
}

//--------------------------------------------------
//
//  Function: AddHour
//
//------------------------------------------------
const bool Date::AddHour(const int hours)
{
   // Aquire Time Now
   struct tm *tmPtr;
   time_t tTmp=mktime(&m_tmObj)+(hours*3600);
   tmPtr = localtime(&tTmp);
   if (tmPtr == NULL)
   {
      return false;
   }

   // Store new time_t type
   m_time = tTmp;
   // Store struct tm type
   memcpy(&m_tmObj,tmPtr,sizeof(struct tm));
   return true;
}

//--------------------------------------------------
//
//  Function: AddDOM
//
//------------------------------------------------
const bool Date::AddDOM(const int days)
{
   // Aquire Time Now
   struct tm *tmPtr;
   time_t tTmp=mktime(&m_tmObj)+(days*86400);
   tmPtr = localtime(&tTmp);
   if (tmPtr == NULL )
   {
      return false;
   }

   m_time=tTmp;
   memcpy(&m_tmObj,tmPtr,sizeof(struct tm));
   return true;
}

//--------------------------------------------------
//
//  Function: AddMonth
//
//------------------------------------------------
const bool Date::AddMonth(const int months)
{
   struct tm tmTime;

   memcpy(&tmTime,&m_tmObj,sizeof(struct tm));
   // Temporarily Store the Current Day
   int tmpDays = tmTime.tm_mday;

   tmTime.tm_mday = T_DOM_MIN; // Store Min Day (temporary)

   int mCnt = months;
   if(months > 0)
   {
      // Add Months
      while(mCnt > 0)
      {
         if(tmTime.tm_mon == 11)
         {
            tmTime.tm_mon = 0;
            tmTime.tm_year++;
         }
         else
         {
            tmTime.tm_mon++;
         }
         mCnt--;
      }
   }
   else
   {
      // Subtract Months
      while(mCnt < 0)
      {
         if(tmTime.tm_mon ==0)
         {
            tmTime.tm_mon=11;
            tmTime.tm_year--;
         }
         else
         {
            tmTime.tm_mon--;
         }
         mCnt++;
      }
   }

   m_time=mktime(&tmTime);
   memcpy(&m_tmObj,&tmTime,sizeof(struct tm));
   return SetDOM(tmpDays);
}

//--------------------------------------------------
//
//  Function: AddYear
//
//------------------------------------------------
const bool Date::AddYear(const int years)
{
   struct tm tmTime;
   memcpy(&tmTime,&m_tmObj,sizeof(struct tm));
   tmTime.tm_year+=years;
   m_time=mktime(&tmTime);
   memcpy(&m_tmObj,&tmTime,sizeof(struct tm));
   return true;
}

//------------------------------------------------
//
//  Function: SetSec
//
//------------------------------------------------
const bool Date::SetSec(const int seconds)
{
   bool ok = false;
   if(seconds >= T_SEC_MIN && seconds <= T_SEC_MAX)
   {
      m_tmObj.tm_sec = seconds;
      m_time=mktime(&m_tmObj);
      ok=true;
   }
   return ok;
}

//------------------------------------------------
//
//  Function: SetMin
//
//------------------------------------------------
const bool Date::SetMin(const int minutes)
{
   bool ok = false;
   if(minutes >= T_MIN_MIN && minutes <= T_MIN_MAX)
   {
      m_tmObj.tm_min = minutes;
      m_time=mktime(&m_tmObj);
      ok=true;
   }
   return ok;
}

//------------------------------------------------
//
//  Function: SetHour
//
//------------------------------------------------
const bool Date::SetHour(const int hours)
{
   bool ok = false;
   if(hours >= T_HOUR_MIN && hours <= T_HOUR_MAX)
   {
      m_tmObj.tm_hour = hours;
      m_time=mktime(&m_tmObj);
      ok=true;
   }
   return ok;
}

//------------------------------------------------
//
//  Function: SetDOM
//
//------------------------------------------------
const bool Date::SetDOM(const int days)
{
   bool ok = false;
   if(days >= T_DOM_MIN && days <= T_DOM_MAX)
   {
      // Apply Correction If Nessisary
      int MaxDays=MaxDOMsThisMonth();
      if(days > MaxDays)
      {
         m_tmObj.tm_mday = MaxDays;
      }
      else
      {
         m_tmObj.tm_mday = days;
      }
      m_time=mktime(&m_tmObj);
      ok=true;
   }
   return ok;
}

//------------------------------------------------
//
//  Function: SetMonth
//
//------------------------------------------------
const bool Date::SetMonth(const int months)
{
   bool ok = false;
   if(months >= T_MONTH_MIN && months <= T_MONTH_MAX)
   {
      m_tmObj.tm_mon = months - 1;
      m_time=mktime(&m_tmObj);
      ok=true;
   }
   return ok;
}

//------------------------------------------------
//
//  Function: SetYear
//
//------------------------------------------------
const bool Date::SetYear(const int years)
{
   bool ok = false;
   if(years >= T_YEAR_MIN && years <= T_YEAR_MAX)
   {
      m_tmObj.tm_year = years - 1900;
      m_time=mktime(&m_tmObj);
      ok=true;
   }
   return ok;
}

//------------------------------------------------
//
//  Function: Str
//
//------------------------------------------------
const string Date::Str(const string& strFormatIn) const
{
   string strFormat=strFormatIn;

   // Syntax Swapping
   // ---------------
   // Support %c     locale’s date and time (e.g., Thu Mar  3 23:05:25 2005)
   FindAndReplace(strFormat,"%c","%a %b %d %H:%M:%S %Y");
   // Support %D     date; same as %m/%d/%y
   FindAndReplace(strFormat,"%D","%m/%d/%y");
   // Support %r     locale’s 12-hour clock time (e.g., 11:11:04 PM)
   FindAndReplace(strFormat,"%r","%H:%M:%S %p");
   // Support %R     24-hour hour and minute; same as %H:%M
   FindAndReplace(strFormat,"%R","%H:%M");
   // Support %T     time; same as %H:%M:%S
   FindAndReplace(strFormat,"%T","%H:%M:%S");
   // Support %t     a tab
   FindAndReplace(strFormat,"%t","\t");
   // Support %n     a new line
   FindAndReplace(strFormat,"%n","\n");
   // Support %h     a new line
   FindAndReplace(strFormat,"%h","\n");
   // Support %x     locale’s date representation (e.g., 12/31/99)
   FindAndReplace(strFormat,"%x","%M/%d/%y");
   // Support %X     locale’s time representation (e.g., 23:13:48)
   FindAndReplace(strFormat,"%X","%H/%M/%S");

   // Build Year into String (to ease handling)
   string strYear;
   stringstream ss;
   ss << (m_tmObj.tm_year+1900);
   strYear= ss.str();


   // Date Swapping
   // ---------------
   // Support %a     locale’s abbreviated weekday name (e.g., Sun)
   FindAndReplace(strFormat,"%a",dayAbbreva[m_tmObj.tm_wday]);
   // Support %A     locale’s full weekday name (e.g., Sunday)
   FindAndReplace(strFormat,"%A",dayOfWeek[m_tmObj.tm_wday]);
   // Support %b     locale’s abbreviated month name (e.g., Jan)
   FindAndReplace(strFormat,"%b",monAbbrev[m_tmObj.tm_mon]);
   // Support %B     locale’s full month name (e.g., January)
   FindAndReplace(strFormat,"%B",monOfYear[m_tmObj.tm_mon]);
   // Support %C     century; like %Y, except omit last two digits (e.g., 21)
   FindAndReplace(strFormat,"%C",strYear.substr(0,2));
   // Support %d     day of month (e.g, 01)
   FindAndReplace(strFormat,"%d",m_tmObj.tm_mday,2);
   // Support %H     hour (00..23)
   FindAndReplace(strFormat,"%H",m_tmObj.tm_hour,2);
   // Support %I     hour (01..12)
   FindAndReplace(strFormat,"%I",(m_tmObj.tm_hour>11)?((m_tmObj.tm_hour+1)/12):(m_tmObj.tm_hour+1),2);
   // Support %j     day of year (001..366)
   FindAndReplace(strFormat,"%j",(m_tmObj.tm_yday)/12,3);
   // Support %k     hour ( 0..23)
   FindAndReplace(strFormat,"%k",m_tmObj.tm_hour,2,' ');
   // Support %l     hour ( 1..12)
   FindAndReplace(strFormat,"%l",(m_tmObj.tm_hour>11)?((m_tmObj.tm_hour+1)/12):(m_tmObj.tm_hour+1),2,' ');
   // Support %m     month (00..59)
   FindAndReplace(strFormat,"%m",(m_tmObj.tm_mon+1),2);
   // Support %M     minute (00..59)
   FindAndReplace(strFormat,"%M",(m_tmObj.tm_min),2);
   // Support %p     locale’s equivalent of either AM or PM; blank if not known
   FindAndReplace(strFormat,"%p",((m_tmObj.tm_hour>11)?"PM":"AM"));
   // Support %P     like %p, but lower case
   FindAndReplace(strFormat,"%P",((m_tmObj.tm_hour>11)?"pm":"pm"));
   // Support %S     second (00..60)
   FindAndReplace(strFormat,"%S",(m_tmObj.tm_sec),2);
   // Support %s     seconds since 1970-01-01 00:00:00 UTC
   FindAndReplace(strFormat,"%s",m_time,2);
   // Support %u     day of week (1..7); 1 is Monday
   FindAndReplace(strFormat,"%u",Sun2Mon[m_tmObj.tm_wday],2);
   // Support %U     week number of year, with Sunday as first day of week (00..53)
   FindAndReplace(strFormat,"%U",m_tmObj.tm_yday%7,2);
   // Support %w     day of week (0..6); 0 is Sunday;
   FindAndReplace(strFormat,"%w",m_tmObj.tm_wday);
   // Support %y     last two digits of year (00..99)
   FindAndReplace(strFormat,"%y",strYear.substr(2,2));
   // Support %Y     year
   FindAndReplace(strFormat,"%Y",(m_tmObj.tm_year)+1900,4);

   return strFormat;
}

//------------------------------------------------
//
//  Function: Cron (Basic)
//
//------------------------------------------------
const Date Date::Cron( int lSecOffset,
                       int lMinOffset,
                       int lHourOffset,
                       int lDomOffset,
                       int lMonthOffset,
                       int lDowOffset,
                       unsigned long lDriftOffset) const
{
   Date dObjFinish(*this);
   Date dObjRef(*this);

   #ifdef CRON_ASSERT_LOOP_LIMIT
      // Only allow looping for a specific time before failing
      unsigned cron_limit = CRON_ASSERT_LOOP_LIMIT;
   #endif

   #ifdef DEBUG
   cerr << "DEBUG Date::Cron("
       << lSecOffset
       << "," << lMinOffset
       << "," << lHourOffset
       << "," << lDomOffset
       << "," << lMonthOffset
       << "," << lDowOffset
       << "," << lDriftOffset << ");" << endl;
   #endif

   bool bSetSec=(lSecOffset >= Date::T_SEC_MIN &&
                 lSecOffset <= Date::T_SEC_MAX);
   bool bSetMin=(lMinOffset >= Date::T_MIN_MIN &&
                 lMinOffset <= Date::T_MIN_MAX);
   bool bSetHour=(lHourOffset >= Date::T_HOUR_MIN &&
                  lHourOffset <= Date::T_HOUR_MAX);
   bool bSetDom=(lDomOffset >= Date::T_DOM_MIN &&
                 lDomOffset <= Date::T_DOM_MAX);
   bool bSetMonth=(lMonthOffset >= Date::T_MONTH_MIN &&
                   lMonthOffset <= Date::T_MONTH_MAX);
   bool bSetDow=(lDowOffset >= Date::T_DOW_MIN &&
                 lDowOffset <= Date::T_DOW_MAX);

   if (!bSetSec)
        lSecOffset=Date::T_SEC_MIN;     // Force Default
   if (!bSetMin)
        lMinOffset=Date::T_MIN_MIN;     // Force Default
   if (!bSetHour)
        lHourOffset=Date::T_HOUR_MIN;   // Force Default
   if (!bSetDom)
        lDomOffset=Date::T_DOM_MIN;     // Force Default
   if (!bSetMonth)
        lMonthOffset=Date::T_MONTH_MIN; // Force Default
   if (!bSetDow)
        lDowOffset=Date::T_DOW_MIN;     // Force Default

   if (!( bSetMin || bSetHour || bSetDom || bSetMonth || bSetDow ))
   {
      // This function requires that at least something has been set
      // so at worst case scenario (and to avoid an infinite loop
      // we assume that seconds have been set
      bSetSec=true;
   }

   // A Drift introduces a small problem, it adds time to the final
   // result. We need to factor this into our calculation a head of time.
   // If a drift has been specified, we have to assume the current time
   // is actually in the past by the same amount.
   if (lDriftOffset != T_NO_DRIFT)
   {
      dObjRef.AddSec(-lDriftOffset);
      dObjFinish.AddSec(-lDriftOffset);
   }
   #ifdef DEBUG
   cerr << "DEBUG Date::Cron"
        << " SetSec: " << bSetSec
        << ", SetMin: " << bSetMin
        << ", SetHour: " << bSetHour
        << ", SetDom: " << bSetDom
        << ", SetMonth: " << bSetMonth
        << ", SetDow: " << bSetDow
        << ", Drift: " << lDriftOffset
        << endl;
   #endif
   // Forever Loop (unless cron is matched)
   while(1)
   {
   #ifdef CRON_ASSERT_LOOP_LIMIT
      if(cron_limit == 0){
         cerr << "FATAL:"
              << " CRON_ASSERT_LOOP_LIMIT ("
              << CRON_ASSERT_LOOP_LIMIT
              << ") reached."
              << " CRON='"
              << lSecOffset
              << " " << lMinOffset
              << " " << lHourOffset
              << " " << lDomOffset
              << " " << lMonthOffset
              << " " << lDowOffset
              << "'"
              << ", DRIFT=" << lDriftOffset
              << ", REF=" << dObjRef.Str()
              << endl;
         // Fail; there is a system problem!
         exit(1);
      }
      // Decrement Limit Counter
      cron_limit--;
   #endif
      #ifdef DEBUG
      cerr << "DEBUG Date::Cron LOOP"
              #ifdef CRON_ASSERT_LOOP_LIMIT
              << "[" << (CRON_ASSERT_LOOP_LIMIT-cron_limit) << "]"
              #endif
              << " in=" << this->Str()
              << ", ref=" << dObjRef.Str()
              << ", calc=" << dObjFinish.Str()
              << endl;
      #endif
      if (bSetSec)
      {
         if(dObjFinish.Sec() > lSecOffset)
         {
            dObjFinish.AddMin(1); // Add 1 minute
            dObjFinish.SetSec(lSecOffset);
            // Continue to parse until the second matches
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=1, min++, sec=" << lSecOffset << endl;
            #endif
            continue;
         }
         else if(dObjFinish.Sec() < lSecOffset)
         {
            dObjFinish.AddSec((lSecOffset-dObjFinish.Sec()));
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=1, min++" << endl;
            #endif
            continue;
         }
         // else we match correctly!
      }

      if (bSetMin)
      {
         if(dObjFinish.Min() > lMinOffset)
         {
            dObjFinish.AddHour(1); // + 1 hour
            dObjFinish.SetMin(lMinOffset);
            // Continue to parse until the minute matches
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=1, hour++, min=" << lMinOffset << endl;
            #endif
            continue;
         }
         else if(dObjFinish.Min() < lMinOffset)
         {
            dObjFinish.AddMin((lMinOffset-dObjFinish.Min()));
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=1, hour++" << endl;
            #endif
            continue;
         }
         // else we match correctly!
      }

      if (bSetHour)
      {
         if(dObjFinish.Hour() > lHourOffset)
         {
            dObjFinish.AddDOM(1); // + 1 day
            dObjFinish.SetHour(lHourOffset);
            // Continue to parse until the hour matches
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=1, dom++, hour=" << lHourOffset << endl;
            #endif
            continue;
         }
         else if(dObjFinish.Hour() < lHourOffset)
         {
            dObjFinish.AddHour(lHourOffset-dObjFinish.Hour());
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=1, dom++" << endl;
            #endif
            continue;
         }
         // else we match correctly!
      }

      if (bSetDow)
      {
         if(dObjFinish.DOW() != lDowOffset)
         {
            dObjFinish.AddDOM(1); // Add 1 day
            // Continue to parse until the DOW matches
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=1, dom++" << endl;
            #endif
            continue;
         }
      }

      if (bSetDom)
      {
         if(dObjFinish.MaxDOMsThisMonth() < lDomOffset)
         {
            // Don't handle values like 31 in February, instead, just add a month
            // and move along
            dObjFinish.AddMonth(1);
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=1, month++" << endl;
            #endif
            continue;
         }
         else if(dObjFinish.DOM() > lDomOffset)
         {
            dObjFinish.AddMonth(1); // + 1 month
            dObjFinish.SetDOM(lDomOffset);
            // Continue to parse until the DOM matches
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=1, month++, dom=" << lDomOffset << endl;
            #endif
            continue;
         }
         else if(dObjFinish.DOM() < lDomOffset)
         {
            dObjFinish.SetDOM(lDomOffset);
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=1, dom=" << lDomOffset << endl;
            #endif
            continue;
         }
         // else we match correctly!
      }

      if (bSetMonth)
      {
         if(dObjFinish.Month() > lMonthOffset)
         {
            dObjFinish.AddYear(1); // + 1 year
            dObjFinish.SetMonth(lMonthOffset);
            // Continue to parse until the Month matches
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=1, year++, month=" << lMonthOffset << endl;
            #endif
            continue;
         }
         else if(dObjFinish.Month() < lMonthOffset)
         {
            dObjFinish.SetMonth(lMonthOffset);
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=1, month=" << lMonthOffset << endl;
            #endif
            continue;
         }
         // else we match correctly!
      }

      // Reset Items backwards based on what is set
      if(bSetMonth)
      {
         if (!bSetHour) dObjFinish.SetHour(Date::T_HOUR_MIN);  // reset hours
         if (!bSetMin) dObjFinish.SetMin(Date::T_MIN_MIN);     // reset minutes
         if (!bSetSec) dObjFinish.SetSec(Date::T_SEC_MIN);     // reset seconds

         if( dObjFinish < dObjRef )
         {
            /* This happens when we're caculating on the same
             * month we are currently in */
            dObjFinish.AddMonth(1);
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=2, month++" << endl;
            #endif
            continue;
         }

      }
      else if ( bSetDom || bSetDow )
      {
         if (!bSetHour) dObjFinish.SetHour(Date::T_HOUR_MIN);  // reset hours
         if (!bSetMin) dObjFinish.SetMin(Date::T_MIN_MIN);     // reset minutes
         if (!bSetSec) dObjFinish.SetSec(Date::T_SEC_MIN);     // reset seconds

         if( dObjFinish < dObjRef )
         {
            /* This happens when we're caculating on the same
             * day we are currently on */
            dObjFinish.AddDOM(1);
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=2, dom++" << endl;
            #endif
            continue;
         }
      }
      else if (bSetHour)
      {
         if (!bSetMin) dObjFinish.SetMin(Date::T_MIN_MIN);     // reset minutes
         if (!bSetSec) dObjFinish.SetSec(Date::T_SEC_MIN);     // reset seconds
         if( dObjFinish < dObjRef )
         {
            /* This happens when we're caculating on the same
             * hour we are currently on */
            dObjFinish.AddHour(1);
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=2, hour++" << endl;
            #endif
            continue;
         }
      }
      else if (bSetMin)
      {
         if (!bSetSec) dObjFinish.SetSec(Date::T_SEC_MIN);     // reset seconds
         if( dObjFinish < dObjRef )
         {
            /* This happens when we're caculating on the same
             * minute we are currently on */
            dObjFinish.AddMin(1);
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=2, min++" << endl;
            #endif
            continue;
         }
      }
      else if (bSetSec)
      {
         if( dObjFinish < dObjRef )
         {
            /* This happens when we're caculating on the same
             * minute we are currently on */
            dObjFinish.AddSec(1);
            #ifdef DEBUG
            cerr << "DEBUG Date::Cron stage=2, sec++" << endl;
            #endif
            continue;
         }
      }

      // Accomodate Drift time if nessisary
      if (lDriftOffset != T_NO_DRIFT)
         dObjFinish.AddSec(lDriftOffset);

      // We don't use our reference object for the final check; instead we use
      // *this object! This is VERY IMPORTANT!
      //
      // We use the real time. This is very important because dObjRef will
      // correctly equate in this equation when there is a drift time involved
      if( dObjFinish == *this )
      {
         #ifdef DEBUG
         cerr << "DEBUG Date::Cron Countermeasures"
              << " ref==finish (" << this->m_time
              << "==" << dObjFinish.m_time << ")"
              << endl;
         #endif
         // times can not be identical .. move along
         //
         //  This Adjustment 'MUST' be done otherwise it is
         //  possible for this loop to return true over and
         //  over again if called consecutively within the same
         //  second.
         //
         //  ie. while [ 1 -eq 1 ]; do ./dateblock -s /5; done
         //
         //  should only be called once every 5 seconds reguardless
         //  if execution time is less then a second
         //
         //  Based on the arguments pased, we only need to adjust the
         //  smallest of them. Hence if Seconds and Years were specified,
         //  we only need to adjust seconds.
         //
         //  This forces the time to be invalid, so the calculation process
         //  begins over again.
         if(bSetSec)dObjFinish.AddSec(1);
         else if(bSetMin)dObjFinish.AddMin(1);
         else if(bSetHour)dObjFinish.AddHour(1);
         else if(bSetDom)dObjFinish.AddDOM(1);
         else if(bSetMonth)dObjFinish.AddMonth(1);
         else if(bSetDow)dObjFinish.AddDOM(1);

         // Remove Drift Accomodation (if set)
         if (lDriftOffset != T_NO_DRIFT)
            dObjFinish.AddSec(-lDriftOffset);

         #ifdef DEBUG
         cerr << "DEBUG Date::Cron Countermeasures"
              << " new_ref=" << dObjFinish.m_time
              << ", difference=+" << (dObjFinish.m_time - this->m_time)
              << " sec(s)"
              << endl;
         #endif
         continue;
      }

      // If the code reaches here, then everything matched
      // correctly.  There is no need to stay in the loop
      break;

   } // end infinite while loop

   // Store new Time
   return dObjFinish;
}

//------------------------------------------------
//
//  Function: CronValid (Advanced)
//
//------------------------------------------------
bool Date::CronValid( const string& sSecOffset,
                            const string& sMinOffset,
                            const string& sHourOffset,
                            const string& sDomOffset,
                            const string& sMonthOffset,
                            const string& sDowOffset,
                            unsigned long lDriftOffset)
{
   static Date tmpDate;
   // Calculate Possible Seconds Combiniation
   set<int> comboList[COMBO_COUNT]; // Build Combination List
   return(
         tmpDate.ParseCronString(
            sSecOffset,
            comboList[COMBO_SEC],T_SEC_MIN,T_SEC_MAX) &&
         tmpDate.ParseCronString(
            sMinOffset,
            comboList[COMBO_MIN],T_MIN_MIN,T_MIN_MAX) &&
         tmpDate.ParseCronString(
            sHourOffset,
            comboList[COMBO_HOUR],T_HOUR_MIN,T_HOUR_MAX) &&
         tmpDate.ParseCronString(
            sDomOffset,
            comboList[COMBO_DOM],T_DOM_MIN,T_DOM_MAX) &&
         tmpDate.ParseCronString(
            sMonthOffset,
            comboList[COMBO_MONTH],T_MONTH_MIN,T_MONTH_MAX) &&
         tmpDate.ParseCronString(
            sDowOffset,
            comboList[COMBO_DOW],T_DOW_MIN,T_DOW_MAX) &&
         lDriftOffset <= T_DRIFT_MAX
    );
}

//------------------------------------------------
//
//  Function: CronValid
//            (using whitespace as a delimiter)
//------------------------------------------------
bool Date::CronValid( const string& strIn,
                      unsigned long lDriftOffset,
                      bool isISC )
{

   //         Normal Format
   //
   //     +------------------------------ second (0 - 59)
   //     |  +--------------------------- min (0 - 59)
   //     |  |  +----------------------- hour (0 - 23)
   //     |  |  |  +----------------- day of month (1 - 31)
   //     |  |  |  |  +----------- month (1 - 12)
   //     |  |  |  |  |  +----- day of week (0 - 6) (Sunday=0)
   //     |  |  |  |  |  |
   //     |  |  |  |  |  |
   //     -  -  -  -  -  -
   //     *  *  *  *  *  *
   //
   //         ISC Format (no second)
   //
   //     +----------------------------- min (0 - 59)
   //     |  +----------------------- hour (0 - 23)
   //     |  |  +----------------- day of month (1 - 31)
   //     |  |  |  +----------- month (1 - 12)
   //     |  |  |  |  +----- day of week (0 - 6) (Sunday=0)
   //     |  |  |  |  |
   //     -  -  -  -  -
   //     *  *  *  *  *

   // Whitespace Separators
   boost::char_separator<char> sep(" \t\n\r\v\f");
   tokenizer tokens(strIn, sep);
   // ISC only processes the first 5 fields, where as
   // otherwise we process the first 6
   size_t token_max = isISC?5:6;

   vector<string> v_tokens;
   for (tokenizer::iterator tok_iter = tokens.begin();
      tok_iter != tokens.end(); ++tok_iter)
   {
      if (v_tokens.size() > token_max)
      {
         // no more entries
         break;
      }
      v_tokens.insert(v_tokens.end(), *tok_iter);
   }

   // Placeholders
   while ( v_tokens.size() < token_max )
      v_tokens.insert(v_tokens.end(), "*");

   // Convert to expected format
   return isISC?
         // Yes? Ok then skip the 'seconds' field (defaults to '*')
         CronValid("*", v_tokens[0], v_tokens[1], v_tokens[2],
                        v_tokens[3], v_tokens[4], lDriftOffset):
         // No? Then handle all of the arguments
         CronValid(v_tokens[0], v_tokens[1], v_tokens[2],
                   v_tokens[3], v_tokens[4], v_tokens[5], lDriftOffset);
}

//------------------------------------------------
//
//  Function: Cron (Advanced)
//
//------------------------------------------------
const Date Date::Cron( const string& sSecOffset,
                       const string& sMinOffset,
                       const string& sHourOffset,
                       const string& sDomOffset,
                       const string& sMonthOffset,
                       const string& sDowOffset,
                       unsigned long lDriftOffset ) const
{

   set<int> comboList[COMBO_COUNT]; // Build Combination List
   Date dObjFinish(*this);

   // Calculate Possible Seconds Combiniation
   if(ParseCronString(sSecOffset,comboList[COMBO_SEC],T_SEC_MIN,T_SEC_MAX) &&
    ParseCronString(sMinOffset,comboList[COMBO_MIN],T_MIN_MIN,T_MIN_MAX) &&
    ParseCronString(sHourOffset,comboList[COMBO_HOUR],T_HOUR_MIN,T_HOUR_MAX) &&
    ParseCronString(sDomOffset,comboList[COMBO_DOM],T_DOM_MIN,T_DOM_MAX) &&
    ParseCronString(sMonthOffset,comboList[COMBO_MONTH],T_MONTH_MIN,T_MONTH_MAX)
    && ParseCronString(sDowOffset,comboList[COMBO_DOW],T_DOW_MIN,T_DOW_MAX))
   {
      // Prepare Iterators
      set<int>::const_iterator itrSec,
                               itrMin,
                               itrHour,
                               itrDom,
                               itrMonth,
                               itrDow;

      // Date Object to Hold on to the lowest reference point
      Date dObjRef(dObjFinish);
      // Big On6 unfortunately.  Perhaps this section could be
      // more smarter and only handle obviously newer times and skip
      // the obvious short ones.
      itrSec  = comboList[COMBO_SEC].begin();
      for(;itrSec != comboList[COMBO_SEC].end();itrSec++)
      {
         itrMin  = comboList[COMBO_MIN].begin();
         for(;itrMin != comboList[COMBO_MIN].end();itrMin++)
         {
            itrHour = comboList[COMBO_HOUR].begin();
            for(;itrHour != comboList[COMBO_HOUR].end();itrHour++)
            {
               itrDom  = comboList[COMBO_DOM].begin();
               for(;itrDom != comboList[COMBO_DOM].end();itrDom++)
               {
                  itrMonth= comboList[COMBO_MONTH].begin();
                  for(;itrMonth != comboList[COMBO_MONTH].end();itrMonth++)
                  {
                     itrDow  = comboList[COMBO_DOW].begin();
                     for(;itrDow != comboList[COMBO_DOW].end();itrDow++)
                     {
                        // Build Reference Point
                        dObjRef=Cron(*itrSec,
                                     *itrMin,
                                     *itrHour,
                                     *itrDom,
                                     *itrMonth,
                                     *itrDow,
                                     lDriftOffset);

                        // Store Value if nessisary
                        if(dObjRef < dObjFinish)
                           dObjFinish = dObjRef;

                        // First time Entry; Store reference
                        // reguardless
                        else if(dObjFinish == *this)
                           dObjFinish = dObjRef;

                     }
                  } // Month
               } // Day of Month
            } // Hours
         } // Minutes
      } // Seconds
   } // Valid Parsing

   return dObjFinish;
}

//------------------------------------------------
//
//  Function: Cron
//            (using whitespace as a delimiter)
//------------------------------------------------
const Date Date::Cron( const string& strIn,
                       unsigned long nDrift, bool isISC ) const
{

   //         Normal Format
   //
   //     +------------------------------ second (0 - 59)
   //     |  +--------------------------- min (0 - 59)
   //     |  |  +----------------------- hour (0 - 23)
   //     |  |  |  +----------------- day of month (1 - 31)
   //     |  |  |  |  +----------- month (1 - 12)
   //     |  |  |  |  |  +----- day of week (0 - 6) (Sunday=0)
   //     |  |  |  |  |  |
   //     |  |  |  |  |  |
   //     -  -  -  -  -  -
   //     *  *  *  *  *  *
   //
   //         ISC Format (no second)
   //
   //     +----------------------------- min (0 - 59)
   //     |  +----------------------- hour (0 - 23)
   //     |  |  +----------------- day of month (1 - 31)
   //     |  |  |  +----------- month (1 - 12)
   //     |  |  |  |  +----- day of week (0 - 6) (Sunday=0)
   //     |  |  |  |  |
   //     -  -  -  -  -
   //     *  *  *  *  *

   // Whitespace Separators
   boost::char_separator<char> sep(" \t\n\r\v\f");
   tokenizer tokens(strIn, sep);
   // ISC only processes the first 5 fields, where as
   // otherwise we process the first 6
   size_t token_max = isISC?5:6;

   vector<string> v_tokens;
   for (tokenizer::iterator tok_iter = tokens.begin();
      tok_iter != tokens.end(); ++tok_iter)
   {
      if (v_tokens.size() > token_max)
      {
         // no more entries
         break;
      }
      v_tokens.insert(v_tokens.end(), *tok_iter);
   }

   // Placeholders
   while ( v_tokens.size() < token_max )
      v_tokens.insert(v_tokens.end(), "*");

   // Convert to cron
   //cout << "Cron(*" << ',' << v_tokens[0] << ','
   //                 << v_tokens[1] << ','
   //                 << v_tokens[2] << ','
   //                 << v_tokens[3] << ','
   //                 << v_tokens[4] << ','
   //                 << v_tokens[5] << ')' << endl;
   return isISC?
         // Yes? Ok then skip the 'seconds' field (defaults to '*')
         Cron("*", v_tokens[0], v_tokens[1], v_tokens[2],
                   v_tokens[3], v_tokens[4], nDrift):
         // No? Then handle all of the arguments
         Cron(v_tokens[0], v_tokens[1], v_tokens[2],
              v_tokens[3], v_tokens[4], v_tokens[5], nDrift);
}

bool Date::ParseCronString(const string& strIn,
                             set<int> &comboListOut,
                             int minVal,
                             int maxVal) const
{
   // Valid characters for strIn are:
   //   0-9,/*
   // - Return immediately if there are invalid characters
   // - Delimit Comma's into array
   // - store unique values into comboList

   vector<string> tokens;
   if(Tokenize(strIn,',',tokens) > 0)
   {
      // Iterate Through Vector Array and Parse Entries
      vector<string>::const_iterator itr;
      size_t startPos;
      size_t nextPos;
      int rangeStart=-1;
      bool modFlag=false;

      for(itr=tokens.begin(); itr!=tokens.end(); itr++)
      {
         // cout << "DEBUG: Parsing '" << *itr <<  "' Size: " << itr->size() << endl;
         startPos=0; // Reset Pointer
         nextPos=0; // Reset Pointer
         while(startPos < itr->size())
         {
            for ( ;  nextPos < itr->size() ; nextPos++ )
               if(!isdigit((*itr)[nextPos]))
                  break;

          //   cout << "DEBUG: NonNumerical: " << (*itr)[nextPos] << endl;
            if(nextPos < itr->size()) // A non-Numerical Value was found
            {
               // Handle It's Value
               // If '-' then begin storing range
               // If '*' then do nothing
               if((*itr)[nextPos] == '*')
               {
                  // Update positions to look past dash
                  startPos = ++nextPos;
                  continue;
               }
               else if((*itr)[nextPos] == '-' && rangeStart < 0 )
               {
                  if(modFlag == true)
                  {
                     // failure
                     return false;
                  }

                  // Store Range
                  istringstream myStream(
                        itr->substr(startPos,nextPos-startPos));
                  if(!(myStream >> rangeStart))
                  {
                     // failure
                     return false;
                  }

                  // Handle Invalid range
                  if(rangeStart < minVal || rangeStart > maxVal)
                  {
                     // failure
                     return false;
                  }

                  // Update positions to look past dash
                  startPos = ++nextPos;
                  continue;

               }
               else if((*itr)[nextPos] == '/' && modFlag == false )
               {
                  // Mod Val
                  modFlag=true;
                  // Update positions to look past dash
                  startPos = ++nextPos;
                  continue;
               }
               else
               {
                  // failure
                  return false;
               }
            }

            if(nextPos > startPos)
            {
               // Store Number
               int val;
               istringstream myStream(itr->substr(startPos,nextPos-startPos));
               if(!(myStream >> val))
               {
                  // failure
                  return false;
               }

               // Handle Invalid range
               if(val < minVal || val > maxVal)
               {
                  // failure
                  return false;
               }

               if(modFlag == true)
               {
                  // Calculate Mod
                  int newVal=0;
                  if (val == 0)
                  {
                     // failure
                     return false;
                  }
                  for(; (newVal <= maxVal); newVal+=val)
                  {
                     if(newVal < minVal)continue;
                     comboListOut.insert(newVal);
                  }
                  // reset mod flag
                  modFlag=false;
               }
               else if(rangeStart < 0)
               {
                  // Store Value Otherwise
                  if(val >= minVal && val <= maxVal)
                     comboListOut.insert(val);
               }
               else
               {
                  if(rangeStart > val)
                  {
                     // Swap
                     int tmp;
                     tmp=rangeStart;
                     rangeStart=val;
                     val=tmp;
                  }

                  // Store Range
                  for(; (rangeStart <= val); rangeStart++)
                     if(rangeStart >= minVal && rangeStart <= maxVal)
                        comboListOut.insert(rangeStart);

                  // Reset Range
                  rangeStart=-1;
               }
            }

            // Increment Position
            startPos = nextPos + 1;
         }
      }
   }

   // At the very least; Token list must contain a '-1'
   // This is used for Basic Cron processing.  '-1' signifies
   // that no combo value was handled
   if(comboListOut.size() <=0)comboListOut.insert(-1);

   // Debug List
   // set<int>::const_iterator itr;
   // cout << "Size: " << comboListOut.size() << endl;
   // for(itr=comboListOut.begin(); itr != comboListOut.end(); itr++)
   //    cout << "Val: " << *itr << endl;

   return true;
}

size_t Date::Tokenize(const string& strIn,
                char delimiter,
                vector<string> &tokens) const
{
   size_t startPos = 0;
   tokens.clear();
   while (startPos <= strIn.size())
   {
      size_t nextPos = strIn.find(delimiter, startPos);
      if (nextPos == string::npos)  // Last token
         nextPos = strIn.size();

      if (nextPos > startPos)
      {
         string token(strIn, startPos, nextPos - startPos);
         tokens.insert(tokens.end(), token);
      }

      // + 1 because it is the size of the seperator (,)
      startPos = nextPos + 1;
   }
   return tokens.size();
}

//--------------------------------------------------
//
//  Function: FindAndReplace
//
//------------------------------------------------
bool Date::FindAndReplace(string &sourceString, const string &findString,
      const string &replaceString) const
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
bool Date::FindAndReplace(string &sourceString, const string &findString,
      const int &replaceNum, int width , char fill) const
{
   string replaceString;
   string::size_type findStringLoc;
   string s;
   stringstream out;
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
