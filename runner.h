#ifndef _RUNNER_H_
#define _RUNNER_H_

#include "remote_process_client.h"

struct runner {
    struct remote_process_client client;
};

void runner_init(struct runner * const runner, const char*, const char*, const char*);
void runner_run(struct runner * const runner);

#endif