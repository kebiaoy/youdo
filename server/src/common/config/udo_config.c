#include "Shlwapi.h"

#include "../error/udo_error.h"

#include "udo_config.h"

const char resource_path[MAX_PATH] = "..\\resource";

char* udo_config_work_path(udo_config* self)
{
	if (!GetModuleFileName(NULL, self->path, MAX_PATH))
	{
		udo_assert_system(0);
	}
	PathRemoveFileSpec(self->path);
	return self->path;
}

void udo_config_init(udo_config* self)
{
	memset(self->path, 0, MAX_PATH);
}

char* udo_config_get_res(udo_config* self, char* html_name)
{
	udo_config_work_path(self);
	if (!PathAppend(self->path, resource_path))
	{
		udo_assert_system(0);
	}
	if (!PathAppend(self->path, html_name))
	{
		udo_assert_system(0);
	}
	return self->path;
}

