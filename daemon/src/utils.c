#include "utils.h"

void create_daemon(char *PID_FILE){
    pid_t pid;
    FILE *fp;
    if ( (pid = fork()) == -1 )
    {
        syslog (LOG_ERR, "Can't fork");
        exit(1);
    }
    else if ( (pid != 0) )
    {
        exit(0);
    }

    setsid(); 

    if ( (pid = fork()) == -1 )
    {
        syslog (LOG_ERR, "Can't fork");
        exit(1);
    }

    else if ( pid > 0 )
    {
        if ( (fp = fopen(PID_FILE, "w")) == NULL )
        {
            syslog (LOG_ERR, "Can't open file");
            exit(1);
        }
        fprintf(fp, "%d\n", (int)pid); 
        fclose(fp);
        exit(0);
    }
}

void check_status(int rc, char *error_msg){
    if ( rc != 0 ) {
        syslog (LOG_ERR, "%s rc=%d\n", error_msg, rc);
        sleep(5);
    }
}
