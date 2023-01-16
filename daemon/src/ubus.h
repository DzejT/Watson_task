struct myArgs* get_response_from_ubus(struct ubus_context *ctx);
void myCallback(struct ubus_request *req, int type, struct blob_attr *msg, void *priv);


struct myArgs{
	double total;
	double free;
	double shared;
	double buffered;
};