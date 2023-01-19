#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <memory.h>
#include <iotp_device.h>
#include <argp.h>
#include <syslog.h>


void create_daemon(char *PID_FILE);
void check_status(int rc, char *error_msg);

struct arguments{
    char *args[5];
    char *device_id;
    char *org_id;
    char *type_id;
    char *auth_token;
    int daemon;
};