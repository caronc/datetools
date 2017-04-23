Date/Time Manipulation
======================
Datetools packages itself with 2 tools, dateblock and datemath.

## Dateblock
Dateblock allows you to block until a period of time arrives
unlike 'sleep' which blocks for a set period of time. The difference
is very subtle but can prove to be useful. Python bindings can
additionally be installed for this too.

__Syntax__:
dateblock [options]

__Allowed options__:
```
  -h [ --help ]         Show this help screen.
  -t [ --test ]         Test (Do not block for the specified period)
  -v [ --verbose ]      Verbose mode
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

 Note: With the exception of the drift option (+), all variations of the syntax
       mentioned above can be mixed if separated using the 'comma' (Separator)
       operator.  ie: */a,b,c-d,/e is valid.
       However: x-y-z is not valid, nor is /x/y or /x-y. All values must be
       within the range of it's time type. Thus 400-4000 would never work since
       no time constraint even resides within that range.

 The cron (--cron|-c) switch allows one to specify the standard cron formatting:
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
  Substitute a asterisk (*) as a placeholder for arguments you are not
  interested in. Asterisks are automatically placed in strings missing all 6
  arguments (separated by white space).

Drifts:

Drifting is an option that allows you to adjust the calculated results by some
additional time.  Lets say you wanted the application to wake up on the 1st
minute of each 10 min interval (1, 11, 21, 31, 41, 51). Specifying the cron
(minute) '*/1' would not work. In fact the cron of '*/1' would unblock at
every minute. You could however achieve the previous example using a drift
value of '60' (seconds) and a cron (minute) entry of '*/10'.

Some things to consider about drifting:

- Drifting is always calculated 'after' a specified cron.
- You can not specify a drifting value on it's own without a cron entry to go
  with it.
- A drift value is always specified in seconds.
- You can not drift longer then the interval calculated by the cron itself.
  In the event this occurs, only the remainder (modulus) is kept; the rest is
  is considered overflow and will simply be ignored.  Drifting behaves this
  way to prevent missing a segment of time that would have otherwise been
  calculated.

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

__Note:__ Since version 1.0.0 introduced inline drift entries, the
__--drift__ (__-x__) becomes ambigious. But to remain backwards compatible I
prefer to leave these switches as they are. So how it will work is: if you
specify a drift value on the command line 'and' in the cron, they will both
be used. For example, the following will treat the result as a drift of 20
and a drift of 10:
```bash
dateblock -c "* 5 * * * * 20" --drift=10

# the above could be re-written like this if you wanted:
dateblock -c "* 5 * * * * 20,10"

# Or even this:
dateblock -c "* 5 +20,10"

# Lastly, this is also viable:
dateblock -c "* 5 * * * * 10" --drift=20

# and...
dateblock -c "* 5" --drift=10,20
```


The python bindings are really easy to use too:
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

# You can specify the drift as an option to the command line as well
# The following would return the unblock time with the drift included
result = dateblock("* /20", drift=300, block=False)
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
Assuming you have GNU C++ compiler and the standard development tools that usually go with it (make, autoconf, automake, etc) then the following will install everything for you.

```bash
autogen.sh
./configure
make
sudo make install
```
