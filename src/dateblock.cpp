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

#include <signal.h>
#include <iostream>
#include <sstream>
#include <string>
#include "Date.h"
#include "dateblock.h"

#ifdef PYTHON_MODULE
   // Support python module for Crons
   #include "Python.h"
   #include "datetime.h"

   // Support for DateTime Lookups
   #ifdef IS_PY3K
      #define PySideDateTime_IMPORT PyDateTime_IMPORT
   #else
     #define PySideDateTime_IMPORT \
             PyDateTimeAPI = (PyDateTime_CAPI*) PyCObject_Import((char*)"datetime", \
                                                                 (char*)"datetime_CAPI")
   #endif
#else
   #include <boost/program_options.hpp>
   namespace po = boost::program_options;
#endif

using namespace std;

extern "C" void sigAlarmHandler();
void sigAlarmHandler(int signal)
{
   // do nothing
}

//------------------------------------------------
//
//  Function: Block
//
//  Desc: Blocks for a specified period (in seconds)
//
//------------------------------------------------
unsigned long Block(unsigned long nsecs)
{
   struct sigaction newact, oldact;
   sigset_t         newmask, oldmask, suspmask;
   unsigned long     unslept;

   newact.sa_handler = sigAlarmHandler;
   sigemptyset(&newact.sa_mask);
   newact.sa_flags = 0;
   sigaction(SIGALRM, &newact, &oldact);

   /* set our handler, save previous information */
   sigemptyset(&newmask);
   sigaddset(&newmask, SIGALRM);
   /* block SIGALRM and save current signal mask */
   sigprocmask(SIG_BLOCK, &newmask, &oldmask);

   if(nsecs > 0)
   {
      alarm(nsecs);
      suspmask = oldmask;
      /* make sure SIGALRM isn't blocked */
      sigdelset(&suspmask, SIGALRM);

      /* wait for any signal to be caught */
      sigsuspend(&suspmask);
   }

   /* some signal has been caught, SIGALRM is now blocked */
   unslept = alarm(0);
   /* reset previous action */
   sigaction(SIGALRM, &oldact, NULL);

   /* reset signal mask, which unblocks SIGALRM */
   sigprocmask(SIG_SETMASK, &oldmask, NULL);

   return(unslept);

}

#ifndef PYTHON_MODULE
//------------------------------------------------
//
//  Function: ProgramDetails
//
//------------------------------------------------
void ProgramDetails()
{
   cerr << "Dateblock v" << PACKAGE_VERSION  << " "
        << "Copyright (C) 2013-2017 " << AUTHOR << endl << endl
        << "Dateblock comes with ABSOLUTELY NO WARRANTY" << endl
        << "This is free software, and you are welcome to redistribute it" << endl
        << "under certain conditions; see man pages for details." << endl << endl

        << "This tool works similar to how sleep works except instead." << endl
        << "of delaying the system 'for' a specific time, it delays the" << endl
        << "system 'until' a specific time. Dateblock could be compared" << endl
        << "to how a cron works with its crontab definitions." << endl
        << "Dateblock will block until a specified criteria is met." << endl
        << "Syntax: "
        << "dateblock [options]" << endl << endl;
}

void ProgramExamples()
{
   cerr << "A variety of syntax is accepted by this tool such as:" << endl

     << "  x               (Value)     where 'x' is represented"
     << " numerically." << endl
     << "  */x (or /x)     (Modulus)   where 'x' is represented"
     << " numerically." << endl

     << "  x-y (or y-x)    (Range)     where 'x' and 'y' are are"
     << " represented numerically." << endl

     << "  x,y             (Separator) where 'x' and 'y' are are"
     << " represented numerically." << endl << endl;
   cerr << " Note: All variations of the syntax mentioned above can be mixed"
     << " if separated" << endl << "       using the 'comma' (Separator)"
     << " operator.  ie: */a,b,c-d,/e is valid." << endl
     << "       However: x-y-z is not valid, nor is /x/y or /x-y. All"
     << " values must be" << endl << "       within the range of it's time type."
     << " Thus 400-4000 would never work since" << endl << "       no time"
     << " constraint even resides within that range."
     << endl << endl;

   cerr << " The cron (--cron|-c) switch allows one to specify the standard cron"
     <<  " formatting:" << endl;
   cerr
     << "     drift time ------------------------------------------+" << endl
     << "     day of week (0 - 6) (Sunday=0) -------------------+  |" << endl
     << "     month (1 - 12) --------------------------------+  |  |" << endl
     << "     day of month (1 - 31) ----------------------+  |  |  |" << endl
     << "     hour (0 - 23) ---------------------------+  |  |  |  |" << endl
     << "     min (0 - 59) -------------------------+  |  |  |  |  |" << endl
     << "     sec (0 - 59) -----------------------  |  |  |  |  |  |" << endl
     << "                                        |  |  |  |  |  |  |" << endl
     << "                                        -  -  -  -  -  -  -" << endl
     << "                                        *  *  *  *  *  *  *" << endl;
   cerr << "  Substitute a asterix (*) as a placeholder for arguments"
     << " you are not" << endl
     << "  interested in. Asterixes are automatically placed in strings missing all 6"
     << endl << "  arguments (separated by white space)." << endl << endl;
   cerr << "Drifts: " << endl
        << endl << "  Drifting is an option that allows you to adjust the calculated results by some"
        << endl << "   additional time.  Lets say you wanted the application to wake up on the 1st"
        << endl << "   minute of each 10 min interval (1, 11, 21, 31, 41, 51). Specifying the cron"
        << endl << "   (minute) '*/1' would not work. In fact the cron of '*/1' would unblock at"
        << endl << "   every minute. You could however achieve the previous example using a drift"
        << endl << "   value of '60' (seconds) and a cron (minute) entry of '*/10'." << endl << endl;
   cerr << "  Some things to consider about drifting:" << endl
     << "   - Drifting is always calculated 'after' a specified cron." << endl
     << "   - You can not specify a drifting value on it's own without a cron entry to go"
     << endl << "     with it."
     << endl << "   - A drift value is always specified in seconds."
     << endl << "   - You can not drift longer then the interval calculated by the cron itself."
     << endl << "     In the event this occurs, only the remainder (modulus) is kept; the rest is"
     << endl << "     is considered overflow and will simply be ignored.  Drifting behaves this"
     << endl << "     way to prevent missing a segment of time that would have otherwise been"
     << endl << "     calculated." << endl
     << endl << "     For example... if you specify a a cron of '*/10' (which would equate to"
     << endl << "     unblocking every 10 seconds), and specified a drift of of 11 (seconds), the"
     << endl << "     overflow would leave you with an actual drift of '1' (not 11)." << endl << endl;
   cerr << "Examples: " << endl;

   cerr << "$> dateblock -n /10" << endl
     << "    This would block until a minute divisible by 10 was reached."
     <<  "  Minutes" << endl << "     divisible by 10 are: 0,10,20,30,40"
     << " and 50."
     << endl << endl;

   cerr << "$> dateblock -c \"/5 * 3,4 * * *\"" << endl
     << "    This would block until a second is divisible by 5 was reached and"
     << " only on" << endl
     << "    hours 3 and 4." << endl
     << "    This can also be written as such: $> dateblock -s /5 -o 3,4"
     << endl << endl;

      cerr << "$> dateblock -c \"* 5\"" << endl
     << "    This would block until the 5 minute of every hour. This can also be written"
     << endl << "    as such:" << endl
     << "          1. $> dateblock -n 5" << endl
     << "          2. $> dateblock -c \"* 5 * * * *\""
     << endl << endl;

   cerr << "$> dateblock -o /5 -d 1-14,20" << endl
     << "    This would block until a hour divisible by 5 was reached on"
     << " the first half" << endl << "     of the month as well as the"
     << " 20th of the month) Hours divisible by 5 are: 0,"
     << endl <<  "     5,10,15 and 20."
     << endl << endl;
}

int main(int argc, char **argv)
{

   Date dObjStart;
   Date dObjFinish;

   string sSecOffset="*";
   string sMinOffset="*";
   string sHourOffset="*";
   string sDomOffset="*";
   string sMonthOffset="*";
   string sDowOffset="*";
   string sDriftOffset="*";
   string sCronStr="";

   // Declare the supported options.
   po::options_description poAllOptions("Allowed options");
   po::variables_map poVariablesMap;
   poAllOptions.add_options()
       ("help,h", "Show this help screen.")
       ("test,t", "Test (Do not block for the specified period)")
       ("verbose,v", "Verbose mode")
       ("isc,i", "ISC mode")
       ("second,s", po::value<string>(), "Second (0-59)")
       ("minute,n", po::value<string>(), "Minute (0-59)")
       ("hour,o", po::value<string>(), "Hour (0-23)")
       ("dom,d", po::value<string>(), "Day of Month (1-31)")
       ("month,m", po::value<string>(), "Month (1-12) {Jan=1,...,Dec=12}")
       ("dow,w", po::value<string>(), "Day of Week (0-6) {Sun=0,...,Sat=6}")
       ("cron,c", po::value<string>(), "Cron string formatting")
       ("drift,x", po::value<string>(), "Additional drift time (in seconds).")
   ;

   // Parse all the options and ensure the critical ones have been defined.
   // It's up to the application mainline to ensure application-specific
   // arguments have been specified on the command-line.
   try
   {
      po::store(
            po::parse_command_line(argc, argv, poAllOptions),
            poVariablesMap);
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
      ProgramExamples();
      return 0;
   }
   else if (! ( (poVariablesMap.count("cron")) ||
         (poVariablesMap.count("second")) ||
         (poVariablesMap.count("minute")) ||
         (poVariablesMap.count("hour")) ||
         (poVariablesMap.count("dom")) ||
         (poVariablesMap.count("month")) ||
         (poVariablesMap.count("dow"))
         )
      )
   {
      cerr << "Error: You must specify at least one option." << endl;
      ProgramDetails();
      cerr << poAllOptions << endl;
      ProgramExamples();
      return 1;
   }

   if (!poVariablesMap.count("cron"))
   {
      // Store Second
      if ((poVariablesMap.count("second")))
      {
         sSecOffset = poVariablesMap["second"].as<string>();
      }

      // Store Minute
      if ((poVariablesMap.count("minute")))
      {
         sMinOffset = poVariablesMap["minute"].as<string>();
      }

      // Store Hour
      if ((poVariablesMap.count("hour")))
      {
         sHourOffset = poVariablesMap["hour"].as<string>();
      }

      // Store DOM of Month
      if ((poVariablesMap.count("dom")))
      {
         sDomOffset = poVariablesMap["dom"].as<string>();
      }

      // Store Month
      if ((poVariablesMap.count("month")))
      {
         sMonthOffset = poVariablesMap["month"].as<string>();
      }
      // Store DOM of Week
      if ((poVariablesMap.count("dow")))
      {
         sDowOffset = poVariablesMap["dow"].as<string>();
      }
      // Store Drift
      if ((poVariablesMap.count("drift")))
      {
         sDriftOffset = poVariablesMap["drift"].as<string>();
      }

      // cout << "Executing Cron("
      //   << sSecOffset
      //   << "," << sMinOffset
      //   << "," << sHourOffset
      //   << "," << sDomOffset
      //   << "," << sMonthOffset
      //   << "," << sDowOffset << ");" << endl;
      if(!Date::CronValid(sSecOffset,
                            sMinOffset,
                            sHourOffset,
                            sDomOffset,
                            sMonthOffset,
                            sDowOffset,
                            sDriftOffset))
      {
         cerr << "Error: Syntax Invalid : '"
            << sSecOffset
            << " " << sMinOffset
            << " " << sHourOffset
            << " " << sDomOffset
            << " " << sMonthOffset
            << " " << sDowOffset
            << " " << sDriftOffset
            << "'" << endl;
            ProgramExamples();
         return 1;
      }

      dObjFinish = dObjStart.Cron(sSecOffset,
                     sMinOffset,
                     sHourOffset,
                     sDomOffset,
                     sMonthOffset,
                     sDowOffset,
                     sDriftOffset);
   }
   else
   {
      // ISC Format
      bool useISC = (poVariablesMap.count("isc") > 0);

      // Parse the old school cron format
      sCronStr = poVariablesMap["cron"].as<string>();
      //cout << "Executing Cron(\""
      //     << sCronStr << "\");" << endl;
      if(!Date::CronValid(sCronStr, useISC))
      {
         cerr << "Error: Syntax Invalid : '"
            << sCronStr << "'" << endl;
            ProgramExamples();
         return 1;
      }

      dObjFinish = dObjStart.Cron(sCronStr, useISC);
   }


   if (poVariablesMap.count("verbose") || poVariablesMap.count("test"))
   {
      cout << "Current Time : "<< dObjStart.Str("%Y-%m-%d %H:%M:%S (%a)") << endl;
      cout << "Block Until  : "<< dObjFinish.Str("%Y-%m-%d %H:%M:%S (%a)") << endl;
   }

   if (!(poVariablesMap.count("test")))
   {
      if(dObjFinish.Time() > dObjStart.Time())
      {
         Block(dObjFinish.Time()-dObjStart.Time());
      }
      Date dObjNow;
      if (poVariablesMap.count("verbose"))
      {
         cout << "UnBlock Time : "
              << dObjNow.Str("%Y-%m-%d %H:%M:%S") << endl;
      }
   }
   return 0;
}
#else
/* Block for a specified cron period; return None if a failure occurs otherwise
 * return the date calculated (after blocking until that period of time).
 *
 * You can additionally use this function to just preform calculations if
 * you specify block=False
 */
static PyObject* dateblock(PyObject *self, PyObject *args, PyObject *kwds)
{
   char *str;
   PyObject *pCronObj = NULL;
   PyObject *pBlockObj = Py_True;
   PyObject *pIscObj = Py_False;

   static char* kwlist[] = {"cron", "block", "ref", "isc", NULL};

   // Block Flag; By default we always block (true=block, false=no block)
   bool block = true;

   // use isc cron type (no seconds or drift in cron entry); by default we use
   // the dateblock format
   bool isc = false;

   // Reference Date/Time
   PyObject *pDateObj = NULL;

   // Time initializes to 'Now'
   Date dObjStart;

   // Reference Time; the only difference between this and the start time
   // is that the Start time may be adjusted based on a passed in reference
   // time.  When we preform a check to decide if we need to block or not, we
   // need to preform this calculation based on 'now' not the start time.
   Date dObjRef(dObjStart);

   // Finish time will be adjusted below base on cron value specified
   Date dObjFinish;

   if(!PyArg_ParseTupleAndKeywords(args, kwds, "O|OOO", kwlist,
            &pCronObj, &pBlockObj, &pDateObj, &pIscObj))
   {
      // Couldn't parse content
      return NULL;
   }

   if(!PyString_Check(pCronObj))
   {
      // Raise TypeError Exception
      PyErr_SetString(PyExc_TypeError, "Only strings accepted.");
      return NULL;
   }

   if (!PyDateTimeAPI)
      /* Since we are dealing with python datetime objects, the following step
       * is VERY IMPORTANT or the code will crash on the below checks since they
       * will internally reference values that have not been initialized */
      PyDateTime_IMPORT;

   if(pDateObj != NULL)
   {
      if(pDateObj != Py_None)
      {
         /* A Reference Date was specified */
         if(PyDateTime_CheckExact(pDateObj))
         {
            /* We got a datetime reference object */
            dObjStart.Set(
               PyDateTime_GET_YEAR(pDateObj),
               PyDateTime_GET_MONTH(pDateObj),
               PyDateTime_GET_DAY(pDateObj),
               PyDateTime_DATE_GET_HOUR(pDateObj),
               PyDateTime_DATE_GET_MINUTE(pDateObj),
               PyDateTime_DATE_GET_SECOND(pDateObj)
            );
         }
         else if (PyDate_CheckExact(pDateObj))
         {
            /* We got a date reference object */
            dObjStart.Set(
               PyDateTime_GET_YEAR(pDateObj),
               PyDateTime_GET_MONTH(pDateObj),
               PyDateTime_GET_DAY(pDateObj),
               0, 0, 0);
         }
         else if (PyTime_CheckExact(pDateObj))
         {
            /* We got a time reference object */
            dObjStart.Set(
               dObjStart.Year(),
               dObjStart.Month(),
               dObjStart.DOM(),
               PyDateTime_TIME_GET_HOUR(pDateObj),
               PyDateTime_TIME_GET_MINUTE(pDateObj),
               PyDateTime_TIME_GET_SECOND(pDateObj)
            );
         }
         else if (PyLong_CheckExact(pDateObj))
         {
            /* We got EPOC Time Value */
            dObjStart.Set((time_t) PyLong_AsUnsignedLongLongMask(pDateObj));
         }
         else if (PyInt_Check(pDateObj))
         {
            /* We got an ill formated EPOC time value; this will only
             * happen until 2038 when the REAL Y2K takes in effect,
             * at that point, we'll always get a Lon */
            dObjStart.Set((time_t) PyInt_AsUnsignedLongLongMask(pDateObj));
         }
         else
         {
            /* Unsupported reference date was specified */
            PyErr_SetString(PyExc_SyntaxError,
                  "Reference date format is not supported.");
            return NULL;
         }
         #ifdef DEBUG
         cerr << "DEBUG Python::Reference('" <<
            dObjStart.Year() <<
            "-" << dObjStart.Month() <<
            "-" << dObjStart.DOM() <<
            " " << dObjStart.Hour() <<
            ":" << dObjStart.Min() <<
            ":" << dObjStart.Sec()
               << ")" << endl;
         #endif
      }
   }

   /* Acquire String */
   str = PyString_AsString(pCronObj);
   if(str == NULL)
   {
      PyErr_SetString(PyExc_SyntaxError,
            "The cron specified is not formatted correctly.");
      return NULL;
   }

   /* Determine if we are blocking or not */
   if(PyBool_Check(pBlockObj))
   {
      block = (bool) ((PyBoolObject *) pBlockObj)->ob_ival;
   }
   else if (PyInt_Check(pBlockObj))
   {
      // We'll be kind to those who ignore the fact that we're
      // expecting a bool and treat integers as we would booleans
      // smart developers will never cause this part of the code
      // to be reached :)
      block = (PyInt_AS_LONG(pBlockObj) != 0);
   }

   /* Determine if we are using isc format or not */
   if(PyBool_Check(pIscObj))
   {
      isc = (bool) ((PyBoolObject *) pIscObj)->ob_ival;
   }
   else if (PyInt_Check(pIscObj))
   {
      // We'll be kind to those who ignore the fact that we're
      // expecting a bool and treat integers as we would booleans
      // smart developers will never cause this part of the code
      // to be reached :)
      isc = (PyInt_AS_LONG(pIscObj) != 0);
   }

   #ifdef DEBUG
   cerr << "DEBUG Python::dateblock('" << str
      << "', isc=" << (string) (isc?"y":"n")
      << ",block=" << (string) (block?"y":"n") << endl;
   #endif

   if(!Date::CronValid(str, isc))
   {
      // Raise SyntaxError Exception
      PyErr_SetString(PyExc_SyntaxError,
            "The cron specified is not formatted correctly.");
      return NULL;
   }

   dObjFinish = dObjStart.Cron(str, isc);

   if(block && dObjFinish.Time() > dObjRef.Time())
   {
      if(Block(dObjFinish.Time()-dObjRef.Time()))
      {
         // Simply put, if block is interupted by any
         // means, we need to return this to the python
         // tool calling it. Block() returns the amount of
         // time still left to drift for; therfore we reached
         // here because a non-zero value was returned
         return Py_None;
      }
   }

   // Return Finish Date/Time
   return PyDateTime_FromDateAndTime(dObjFinish.Year(),
                                     dObjFinish.Month(),
                                     dObjFinish.DOM(),
                                     dObjFinish.Hour(),
                                     dObjFinish.Min(),
                                     dObjFinish.Sec(),
                                     0);
}

/* Python Method Table.
 */
static PyMethodDef CronMethods[] =
{
    {"dateblock", (PyCFunction)dateblock, METH_VARARGS|METH_KEYWORDS,
                  "Cron like blocking function"},
    /* sentinel */
    {NULL, NULL }
};

/* Register the method table.
 */
PyMODINIT_FUNC
initdateblock()
{
   PyObject *m;

   m = Py_InitModule("dateblock", CronMethods);

   /* Create Constants */
   PyModule_AddIntConstant(m, "SEC_MIN", Date::T_SEC_MIN);
   PyModule_AddIntConstant(m, "SEC_MAX", Date::T_SEC_MAX);
   PyModule_AddIntConstant(m, "MIN_MIN", Date::T_MIN_MIN);
   PyModule_AddIntConstant(m, "MIN_MAX", Date::T_MIN_MAX);
   PyModule_AddIntConstant(m, "HOUR_MIN", Date::T_HOUR_MIN);
   PyModule_AddIntConstant(m, "HOUR_MAX", Date::T_HOUR_MAX);
   PyModule_AddIntConstant(m, "DOM_MIN", Date::T_DOM_MIN);
   PyModule_AddIntConstant(m, "DOM_MAX", Date::T_DOM_MAX);
   PyModule_AddIntConstant(m, "MONTH_MIN", Date::T_MONTH_MIN);
   PyModule_AddIntConstant(m, "MONTH_MAX", Date::T_MONTH_MAX);
   PyModule_AddIntConstant(m, "YEAR_MIN", Date::T_YEAR_MIN);
   PyModule_AddIntConstant(m, "YEAR_MAX", Date::T_YEAR_MAX);
   PyModule_AddIntConstant(m, "DOW_MIN", Date::T_DOW_MIN);
   PyModule_AddIntConstant(m, "DOW_MAX", Date::T_DOW_MAX);
   PyModule_AddIntConstant(m, "DRIFT_MIN", Date::T_DRIFT_MIN);
   PyModule_AddIntConstant(m, "DRIFT_MAX", Date::T_DRIFT_MAX);
}
#endif
