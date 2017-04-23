Date/Time Manipulation
======================
Datetools packages itself with 2 tools, dateblock and datemath.

## Dateblock
Dateblock allows you to block (exactly how a Linux/Unix Cron might) _until_ a
specific point of time unlike 'sleep' which blocks _for_ a set period of time.
The difference is very subtle but can prove to be very useful.

It offers a few new features that the Internet Systems Consortium (ICS) version
does not. Specifically the ability to wake on a specific second and
additionally drift for certain period aftewards (kind of like combining sleep
and cron into one).

Python bindings can additionally be installed for this too.

__Syntax__:
dateblock [options]

__Allowed options__:
```
  -h [ --help ]         Show this help screen.
  -t [ --test ]         Test (Do not block for the specified period)
  -v [ --verbose ]      Verbose mode
  -i [ --isc ]          ISC Mode
  -s [ --second ] arg   Second (0-59)
  -n [ --minute ] arg   Minute (0-59)
  -o [ --hour ] arg     Hour (0-23)
  -d [ --dom ] arg      Day of Month (1-31)
  -m [ --month ] arg    Month (1-12) {Jan=1,...,Dec=12}
  -w [ --dow ] arg      Day of Week (0-6) {Sun=0,...,Sat=6}
  -c [ --cron ] arg     Cron string formatting
  -x [ --drift ] arg    Additional drift time (in seconds).
```

A variety of syntax is accepted by this tool such as:
```
  x               (Value)     where 'x' is represented numerically.
  */x (or /x)     (Modulus)   where 'x' is represented numerically.
  x-y (or y-x)    (Range)     where 'x' and 'y' are are represented numerically.
  x,y             (Separator) where 'x' and 'y' are are represented numerically.
  +x              (Drift)     where 'x' is a drift value.  This option must be
                               the last specified.
```

__Note__: With the exception of the drift option (+), all variations of the syntax
      mentioned above can be mixed if separated using the 'comma' (Separator)
      operator.  ie: */a,b,c-d,/e is valid.
      However: x-y-z is not valid, nor is /x/y or /x-y. All values must be
      within the range of it's time type. Thus 400-4000 would never work since
      no time constraint even resides within that range.

The __--cron__(__-c__) switch allows one to specify the standard cron formatting:
```
     drift time ----------------------------------------------+
     day of week (0 - 6) (Sunday=0) -----------------------+  |
     month (1 - 12) ------------------------------------+  |  |
     day of month (1 - 31) --------------------------+  |  |  |
     hour (0 - 23) -------------------------------+  |  |  |  |
     min (0 - 59) -----------------------------+  |  |  |  |  |
     sec (0 - 59) ---------------------------  |  |  |  |  |  |
                                            |  |  |  |  |  |  |
                                            -  -  -  -  -  -  -
                                            *  *  *  *  *  *  *
```
Substitute an asterisk (*) as a placeholder for arguments you are not
interested in. Asterisks are automatically placed in strings missing all 6
arguments (separated by white space).

The __--isc__ (__-i__) switch makes the cron interpretation equivalent to
what it is today (ISC stands for Internet Systems Consortium). Hence:
```
         ISC Format (no second or drift)

     +----------------------------- min (0 - 59)
     |  +----------------------- hour (0 - 23)
     |  |  +----------------- day of month (1 - 31)
     |  |  |  +----------- month (1 - 12)
     |  |  |  |  +----- day of week (0 - 6) (Sunday=0)
     |  |  |  |  |
     -  -  -  -  -
     *  *  *  *  *
```

### Drifts:
Drifting is an option that allows you to adjust the calculated results by some
additional time.  Lets say you wanted the application to wake up on the 1st
minute of each 10 min interval (1, 11, 21, 31, 41, 51). Specifying the cron
(minute) '*/1' would not work. In fact the cron of '*/1' would unblock at
every minute. You could however achieve the previous example using a drift
value of '60' (seconds) and a cron (minute) entry of '*/10'.

In a way, drifting is a little like executing a cron and then sleeping for
an additional period right afterwards.  However this is a much more elegant
solution with a more precise blocking period.

Some things to consider about drifting:

- Drifting is always calculated 'after' a specified cron.
- You can not specify a drifting value on it's own without a cron entry to go
  with it.
- A drift value is always specified in seconds.
- You can not drift longer then the interval calculated by the cron itself.
  In the event this occurs, only the remainder (modulus) is kept; the rest is
  is considered overflow and will simply be ignored.  Drifting behaves this
  way to prevent missing a segment of time that would have otherwise been
  calculated. For example... lets say you wanted to unblock every hour, your
  cron might look like this: ```dateblock --hour=/1```. With this cron in
  mind, you could never add a drift of a value larger then 1 hour (3660)...
  Why? Because the _--hour_ entry has already defined the time (slice) bounds
  of 1 hour and a drift value of that same (or more) would cause it to spill
  into _the next_ hour; an hour that would have already been calculated.

  The drift can still be used in all scenarios; lets say you wanted to wake
  ever 2 hours. ```dateblock --hour=/2```.  Well with this, you can set your
  drift up to that same amount (2 hours or 7320 seconds).

  Perhaps another way to word this is: _The maximum drift value is __always__
  relative to the largest timeslice it can fit within._

- You can use the shortcut character of a _plus_ (_+_) to immediately invoke
  the drift (from a cron entry) or simply use it with the __--drift__ (__-d__)
  option.

  For example... if you specify a a cron of '*/10' (which would equate to
  unblocking every 10 seconds), and specified a drift of of 11 (seconds), the
  overflow would leave you with an actual drift of '1' (not 11).

### Examples
The following would block until a minute divisible by 10 was reached.  Minutes
divisible by 10 are: 0,10,20,30,40 and 50.
```bash
dateblock -n /10
```

The following would block until a second is divisible by 5 was reached and
only on hours 3 and 4. This can also be written as such:
```bash
dateblock -s /5 -o 3,4

# you can rewrite the above like this too:
dateblock -c "/5 * 3,4 * * *"
```

The following would block until the 5 minute of every hour:
```bash
dateblock -c "* 5"

# you can rewrite the above like this too:
dateblock -n 5
dateblock -c "* 5 * * * *"
```

The following would block until a hour divisible by 5 was reached on the first
half of the month as well as the 20th of the month) Hours divisible by 5 are:
0, 5,10,15 and 20.
```bash
dateblock -o /5 -d 1-14,20
```

Drifting using the cron was added in v1.0.0 (but was always available using
__-x__ or __--drift__). The advantage you get from using it inline with the
cron string is the ability to chain them with multiple drift times using
a comma and or range operator. The modulus one will not work here.

The following will block until the 5:20 minute mark of each hour (20 seconds
past)
```bash
dateblock -c "* 5 * * * * 20"

# you can rewrite the above like this too:
dateblock -n 5

# You can skip to the drift portion by just denoting a plus (+) as the last
# entry specified.  This is interpreted as a drift time.
dateblock -c "* 5 +20"
```
The expression is considered invalid if more entries are found after the drift
period. However this is still valid:
```bash
dateblock -c "* 5 * * * * +20"
```

### DateBlock Python Bindings
The DateBlock python bindings are really easy to use too:
```python
from dateblock import dateblock
from datetime import datetime

# Returns the date object it will unblock at
result = dateblock("* /20", block=False)

print("Blocking until %s" % result.strftime("%Y-%m-%d %H:%M:%S"))
dateblock("* /20")
print("Unblocked at %s" % datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

# Perhaps you want to calculate a time relative to somewhere in the past?
# No problem; lets get a relative time 4 days ago (from now)
relative = datetime.now() - timedelta(days=4)
result = dateblock("* /20", ref=relative, block=False)

# To use the isc switch with the bindings, just set isc=True (by default
# it is off). Below would block for the start of each minute (not second
# had you not set the isc flag)
result = dateblock("/1", isc=True)

# Dateblock supports date, time and epoch time too!
#
# Date: (yyyy, mm, dd)
from dateblock import date
result = dateblock("/5", ref=date(2017, 4, 22))

# Time: (hh, mm, ss)
from dateblock import time
result = dateblock("/5", ref=time(10, 5, 0))

# Epoch: seconds
result = dateblock("/5", ref=13424236)

```

## Datemath
Datemath allows you to manipulate a date and or time by adding
and or subtracting to it. It can help with scripting and calculating
a specific date.

__Syntax__:
datemath [options] [-f DateTimeFormat]

__Options__:
```
  -h [ --help ]         Show this help screen.
  -s [ --seconds ] arg  Specify the offset (+/-) in seconds.
  -n [ --minutes ] arg  Specify the offset (+/-) in minutes.
  -o [ --hours ] arg    Specify the offset (+/-) in hours.
  -d [ --days ] arg     Specify the offset (+/-) in days.
  -m [ --months ] arg   Specify the offset (+/-) in months.
  -y [ --years ] arg    Specify the offset (+/-) in years.
  -f [ --format ] arg   Specify the desired output format (see $>man date). The
                        default is: %Y-%m-%d %H:%M:%S
```

## Installation
Assuming you have GNU C++ compiler and the standard development tools that
usually go with it (make, autoconf, automake, etc) then the following will
install everything for you.

```bash
autogen.sh
./configure
make
sudo make install
```
