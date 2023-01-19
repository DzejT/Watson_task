#include <libubus.h>
#include "utils.h"
#include "ubus.h" 


void create_config(IoTPConfig **config, IoTPDevice *device, struct arguments arguments){
    int rc;

    while((rc = IoTPConfig_setLogHandler(IoTPLog_FileDescriptor, stdout)) != 0)
        check_status(rc, "Failed to set IoTP Client log handler");

    while((rc = IoTPConfig_create(config, NULL)) != 0)
        check_status(rc, "Failed to initialize configuration");
    
    while((rc = override_config(arguments, *config)) != 0)
        check_status(rc, "Failed to read configurations");
}

void connect_device(IoTPDevice **device, IoTPConfig *config){
    int rc;

    while((rc = IoTPDevice_create(device, config)) != 0)
        check_status(rc, "Failed to configure IoTP device");

    while((rc = IoTPDevice_connect(*device)) != 0)
        check_status(rc, "Failed to connect to Watson IoT Platform");

}

void publish_data(IoTPDevice *device, int *run_loop){
    struct ubus_context *ctx;
    struct myArgs *RAMdata;

    ctx = ubus_connect(NULL);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
	}

    int rc = 0;
    char data[256];

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

        free(RAMdata);
        sleep(10);
    }
    ubus_free(ctx);
}


int override_config(struct arguments args, IoTPConfig *config){
    if(args.org_id == NULL || args.type_id == NULL || args.device_id == NULL || args.auth_token == NULL){
        syslog(LOG_ERR, "Watson device information is incorrect");
        return 1;
    }
    IoTPConfig_setProperty(config, IoTPConfig_identity_orgId, args.org_id);
    IoTPConfig_setProperty(config, IoTPConfig_identity_typeId, args.type_id);
    IoTPConfig_setProperty(config, IoTPConfig_identity_deviceId, args.device_id);
    IoTPConfig_setProperty(config, IoTPConfig_auth_token, args.auth_token);

    return 0;
    
}
