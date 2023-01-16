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
#include <libubus.h>
#include "utils.h"
#include "ubus.h"



#define CONFIG_FILE "/usr/bin/config.yaml"



void handle_log_in(struct arguments arguments, char cwd[], int *run_loop){

    int testCycle = 0;
    int rc = 0;
    
    
    IoTPConfig *config = NULL;
    IoTPDevice *device = NULL;


    rc = IoTPConfig_setLogHandler(IoTPLog_FileDescriptor, stdout);
    if ( rc != 0 ) {
        syslog (LOG_ERR, "WARN: Failed to set IoTP Client log handler: rc=%d\n", rc);
        close_program(device, config);
    }

    rc = IoTPConfig_create(&config, CONFIG_FILE);
    if ( rc != 0 ) {
        syslog (LOG_ERR, "ERROR: Failed to initialize configuration: rc=%d\n", rc);
        syslog (LOG_ERR, "ERROR: Config file is missing \n");
        close_program(device, config);
    }

    override_config(arguments, config);


    rc = IoTPDevice_create(&device, config);
    if ( rc != 0 ) {
        syslog (LOG_ERR, "ERROR: Failed to configure IoTP device: rc=%d\n", rc);
        close_program(device, config);
    }

    rc = IoTPDevice_connect(device);

    if ( rc != 0 ) {
        syslog (LOG_ERR, "ERROR: Failed to connect to Watson IoT Platform: rc=%d\n", rc);
        syslog (LOG_ERR, "ERROR: Returned error reason: %s\n", IOTPRC_toString(rc));

        close_program(device, config);
    }

    publish_data(device, testCycle, run_loop);

    rc = IoTPDevice_disconnect(device);
    if ( rc != IOTPRC_SUCCESS ) {
        syslog (LOG_ERR, "ERROR: Failed to disconnect from  Watson IoT Platform: rc=%d\n", rc);
    }

    IoTPDevice_destroy(device);
    IoTPConfig_clear(config);
}



void publish_data(IoTPDevice *device, int testCycle, int *run_loop){
    struct ubus_context *ctx;

    ctx = ubus_connect(NULL);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
	}


    int rc = 0;
    int cycle = 0;
    char data[256];

    struct myArgs *RAMdata;


    
    while(*run_loop == 0)
    {       
        RAMdata = get_response_from_ubus(ctx);

        if(RAMdata == NULL){
            syslog(LOG_ERR, "Failed to get RAM data\n");
            continue;
        }

        if(RAMdata->total == 0 || RAMdata->free == 0 || RAMdata->shared == 0 || RAMdata->buffered == 0){
            syslog(LOG_ERR, "Failed to get RAM data\n");
            continue;
        }

        sprintf(data, 
        "{\"data\": {\"Total memory\" : \"%.2f MB\",\"Free memory\" : \"%.2f MB\",\"Shared memory\" : \"%.2f KB\",\"Buffered memory\" : \"%.2f KB\"}}", 
        RAMdata->total, RAMdata->free, RAMdata->shared, RAMdata->buffered
        );

        syslog (LOG_INFO,  "Send status event\n");
        rc = IoTPDevice_sendEvent(device,"status", data, "json", QoS0, NULL);
        syslog (LOG_INFO,  "RC from publishEvent(): %d\n", rc);


        if ( testCycle > 0 ) {
            cycle += 1;
            if ( cycle >= testCycle ) {
                break;
            }
        }
        free(RAMdata);
        sleep(10);
    }
    ubus_free(ctx);
}

void override_config(struct arguments args, IoTPConfig *config){
    if(args.org_id != NULL)
        IoTPConfig_setProperty(config, "identity.orgId", args.org_id);
    if(args.type_id != NULL)
        IoTPConfig_setProperty(config, "identity.typeId", args.type_id);
    if(args.device_id != NULL)
        IoTPConfig_setProperty(config, "identity.deviceId", args.device_id);
    if(args.auth_token != NULL)
        IoTPConfig_setProperty(config, "auth.token", args.auth_token);
}
