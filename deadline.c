#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/resource.h>

void testrt(void)
{
    time_t recent;
    time_t new;
    int count;
    int inner;
    int missedDeadline;
    setdl (120); /* let all other RT processes start up */
    sleep (1); /* while we sleep */
    recent = time(NULL);
    missedDeadline = 0;
    count = 0;
    while (count < 100) {
        setdl (60);/* set deadline */
        while ((new = time(NULL)) == recent) { /* still on the same second */
            /* do nothing */
        }

        if (new != (recent + 1)) { /* new second: did we skip any? */
            /* missed the deadline */
            setdl (0); /* no longer real time */
            missedDeadline++;
            printf ("missed deadline after %d seconds\n", count);
            return;
        } else {
            count++;
        }
        recent = new;
    }

    setdl (0); /* no longer real time */
    printf ("successfully met %d successive deadlines\n",
missedDeadline);
}

int main (int argc, char ** argv)
{
    testrt();
    return 0;
}

