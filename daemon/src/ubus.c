#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include <math.h>
#include "ubus.h"


enum {
	TOTAL_MEMORY,
	FREE_MEMORY,
	SHARED_MEMORY,
	BUFFERED_MEMORY,
	__MEMORY_MAX,
};

enum {
	MEMORY_DATA,
	__INFO_MAX,
};

static const struct blobmsg_policy memory_policy[__MEMORY_MAX] = {
	[TOTAL_MEMORY] = { .name = "total", .type = BLOBMSG_TYPE_INT64 },
	[FREE_MEMORY] = { .name = "free", .type = BLOBMSG_TYPE_INT64 },
	[SHARED_MEMORY] = { .name = "shared", .type = BLOBMSG_TYPE_INT64 },
	[BUFFERED_MEMORY] = { .name = "buffered", .type = BLOBMSG_TYPE_INT64 },
};

static const struct blobmsg_policy info_policy[__INFO_MAX] = {
	[MEMORY_DATA] = { .name = "memory", .type = BLOBMSG_TYPE_TABLE },
};


void myCallback(struct ubus_request *req, int type, struct blob_attr *msg, void *priv) {
	struct myArgs **args = (struct myArgs**)req->priv;

	struct blob_attr *tb[__INFO_MAX];
	struct blob_attr *memory[__MEMORY_MAX];

	blobmsg_parse(info_policy, __INFO_MAX, (struct blob_attr**)&tb, blob_data(msg), blob_len(msg));

	if (!tb[MEMORY_DATA]) {
		fprintf(stderr, "No memory data received\n");
		return;
	}
	blobmsg_parse(memory_policy, __MEMORY_MAX, memory, blobmsg_data(tb[MEMORY_DATA]), blobmsg_data_len(tb[MEMORY_DATA]));


	(*args)->total = (double)blobmsg_get_u64(memory[TOTAL_MEMORY])/ 1000000;
	(*args)->free = (double)blobmsg_get_u64(memory[FREE_MEMORY])/ 1000000;
	(*args)->shared = (double)blobmsg_get_u64(memory[SHARED_MEMORY])/ 1000;
	(*args)->buffered = (double)blobmsg_get_u64(memory[BUFFERED_MEMORY])/ 1000;

}


struct myArgs* get_response_from_ubus(struct ubus_context *ctx)
{
	uint32_t id;
	struct myArgs *customArgs = (struct myArgs*) malloc(sizeof(struct myArgs));

	if(customArgs == NULL){
		syslog(LOG_ERR, "Failed to malloc element\n");
		return customArgs;
	}

	customArgs->total = 0;
	customArgs->free = 0;
	customArgs->shared = 0;
	customArgs->buffered = 0;


	if (ubus_lookup_id(ctx, "system", &id) || ubus_invoke(ctx, id, "info", NULL, myCallback, &customArgs, 3000)){
		syslog(LOG_ERR, "cannot request memory info\n");
	}

	return customArgs;
}


