Date/Time Manipulation
======================
Datetools packages itself with 2 tools, dateblock and datemath.

## Dateblock
Dateblock allows you to block until a period of time arrives
unlike 'sleep' which blocks for a set period of time. The difference
is very subtle but can prove to be useful. Python bindings can
additionally be installed for this too.

###Syntax:
$> dateblock [options]

###Allowed options:
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
  */x (or /x)     (Modulus)  where 'x' is represented numerically.
  x-y (or y-x)    (Range)     where 'x' and 'y' are are represented numerically.
  x,y             (Separator) where 'x' and 'y' are are represented numerically.
```

 Note: All variations of the syntax mentioned above can be mixed if separated
       using the 'comma' (Separator) operator.  ie: */a,b,c-d,/e is valid.
       However: x-y-z is not valid, nor is /x/y or /x-y. All values must be
       within the range of it's time type. Thus 400-4000 would never work since
       no time constraint even resides within that range.

 The cron (--cron|-c) switch allows one to specify the standard cron formatting:
```
     day of week (0 - 6) (Sunday=0) -----------------------+
     month (1 - 12) ------------------------------------+  |
     day of month (1 - 31) --------------------------+  |  |
     hour (0 - 23) -------------------------------+  |  |  |
     min (0 - 59) -----------------------------+  |  |  |  |
     sec (0 - 59) ---------------------------  |  |  |  |  |
                                            |  |  |  |  |  |
                                            -  -  -  -  -  -
                                            *  *  *  *  *  *
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

###Examples
The following would block until a minute divisible by 10 was reached.  Minutes divisible by 10 are: 0,10,20,30,40 and 50.
```bash
$> dateblock -n /10
```

The following would block until a second is divisible by 5 was reached and only on hours 3 and 4. This can also be written as such: $> dateblock -s /10 -o 3,4
```bash
$> dateblock -c "/5 * 3,4 * * *"
```

The following would block until the 5 minute of every hour.
```bash
$> dateblock -c "* 5"

# you can rewrite the above like this too:
$> dateblock -n 5
$> dateblock -c "* 5 * * * *"
```

The following would block until a hour divisible by 5 was reached on the first half of the month as well as the 20th of the month) Hours divisible by 5 are: 0, 5,10,15 and 20.
```bash
$> dateblock -o /5 -d 1-14,20
```

## Datemath
Datemath allows you to manipulate a date and or time by adding
and or subtracting to it. It can help with scripting and calculating
a specific date.

###Syntax:
datemath [options] [-f DateTimeFormat]

###Options
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
