/**
 * @file leapsectest.cpp
 *
 * @brief Report UTC second count around leap seconds
 *
 * @version 0.1
 *
 * @author (c) Innova Engineering Limited
 * @date 10/5/2017
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[], char *envp[])
{
   // how is the leap second represented?
   struct tm leapsec;
   memset(&leapsec, 0, sizeof(leapsec));
   leapsec.tm_hour = 23;
   leapsec.tm_min = 59;
   leapsec.tm_sec = 60;
   leapsec.tm_year = 116;
   leapsec.tm_mon = 11;
   leapsec.tm_mday = 31;
   printf("\nThe leap second at 23:59:60 on 31 December 2016 is represented as:\n\n");
   printf("asctime(struct tm):      %s", asctime(&leapsec));
   time_t leaptime = mktime(&leapsec);
   printf("mktime(struct tm):       %d\n", leaptime);
   printf("asctime(gmtime(time_t)): %s\n", asctime(gmtime(&leaptime)));

   // walk over leap second, displaying time_t and asctime
   printf("Examining timestamps before and after the leap second:\n\n");
   struct tm start;
   memset(&start, 0, sizeof(start));
   start.tm_hour = 23;
   start.tm_min = 59;
   start.tm_sec = 58;
   start.tm_year = 116;
   start.tm_mon = 11;
   start.tm_mday = 31;
   time_t starttm = mktime(&start);
   // print out time_ts and strings
   for (time_t now = starttm; now < starttm + 5; now++) {
      printf("%d: %s", now, asctime(gmtime(&now)));
   }
   printf("\n");

   printf("Note that the first time the leap-second is printed, it is correct.\n"
          "But after the call to mktime, the structure is updated and the leap-second removed.\n"
          "This is because UTC and most OSs prefer to treat all days as having the same number of seconds.\n"
          "And this is demonstrated in the list of time_ts, which are unable to represent the leap second at all.\n"
          "\n"
          "Some of the consequences of this are:\n"
          " - logfile timestamps will be wrong during this second (typically repeating 2017/01/01:00:00:00)\n"
          " - software may be confused or fail if sub-second timestamps do not increase monotonically\n"
          " - duration calculations spanning the leap second will be wrong by 1 second\n"
          " - short durations could even come out negative if e.g. 2017/01/01:00:00:00 repeats\n");
   printf("\n");

   return 0;
}
