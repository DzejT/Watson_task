// void handle_log_in(struct arguments arguments, int *run_loop);
void publish_data(IoTPDevice *device, int *run_loop);
int override_config(struct arguments args, IoTPConfig *config);
void create_config(IoTPConfig **config, IoTPDevice *device, struct arguments arguments);
void connect_device(IoTPDevice **device, IoTPConfig *config);

