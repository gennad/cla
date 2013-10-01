#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

using namespace std;

#define DAEMON_NAME "mydaemon"


void process() {
    syslog (LOG_NOTICE, "Writing to my Syslog");
    syslog (LOG_INFO, "Some info");
    syslog (LOG_DEBUG, "Some debug");
    syslog (LOG_WARNING, "Some warning");
    syslog (LOG_ERR, "Some error");
    syslog (LOG_CRIT, "Some crit");
}   


int main(int argc, char *argv[]) {

    int n;
    long elapsed_seconds;
    char line[80];
    // clrscr();
    FILE *fr = NULL;

    fr = fopen ("test", "rt");  /* open the file for reading */
    /* elapsed.dta is the name of the file */
    /* "rt" means open the file for reading text */

    while(fgets(line, 80, fr) != NULL)
    {
	 /* get a line, up to 80 chars from fr.  done if NULL */
	 sscanf (line, "%ld", &elapsed_seconds);
	 /* convert the string to a long int */
	 printf ("%ld\n", elapsed_seconds);

         printf ("%s\n", line);
         
         char * n = strstr(line, "econd");

         if (n) {
             printf("Found\n");
             printf("%s\n", n);
         }
         
    }
    fclose(fr);  /* close the file prior to exiting the routine */   


    return 0;

    // Set our Logging Mask and open the Log
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

    syslog(LOG_INFO, "Entering Daemon");

    pid_t pid, sid;

    // Fork the Parent Process
    pid = fork();

    if (pid < 0) { 
        exit(EXIT_FAILURE); 
    }

    // We got a good pid, Close the Parent Process
    if (pid > 0) { 
        exit(EXIT_SUCCESS); 
    }

    // Change File Mask
    umask(0);

    // Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) { 
        exit(EXIT_FAILURE); 
    }

    // Change Directory
    // If we cant find the directory we exit with failure.
    if ((chdir("/")) < 0) 
        { exit(EXIT_FAILURE); 
    }

    // Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //----------------
    //Main Process
    //----------------
    while(true){
        process();    // Run our Process
        sleep(5);    // Sleep for 60 seconds
    }

    // Close the log
    closelog ();
}
