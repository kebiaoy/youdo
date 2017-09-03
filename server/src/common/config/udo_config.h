#ifndef UDO_CONFIG_H__
#define UDO_CONFIG_H__
#include <windows.h>


typedef struct udo_config
{
	char path[MAX_PATH];
}udo_config;

void udo_config_init(udo_config* self);

char* udo_config_work_path(udo_config* self);

char* udo_config_get_res(udo_config* self, char* html_name);
#endif