#ifndef PLUG_H
#define PLUG_H

#include <stddef.h>

void (*plug_init)(void) = NULL;
void *(*plug_pre_reload)(void) = NULL;
void (*plug_post_reload)(void *) = NULL;
void (*plug_update)(void) = NULL;
void (*plug_destroy)(void) = NULL;

#endif // PLUG_H
