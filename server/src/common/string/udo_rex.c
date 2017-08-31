#include <pcre2.h>
#include "../error/udo_error.h"
#include "udo_rex.h"

void udo_rex_init(udo_rex* self)
{
	self->re = NULL;
	self->match_data = NULL;
	self->match_count = 0;
}


int udo_rex_match(udo_rex* self, char* rex_pattern, int rex_pattern_length, char* rex_str)
{
	int errornumber;
	PCRE2_SIZE erroroffset;
	self->rex_pattern = rex_pattern;
	if (self->re)
	{
		pcre2_code_free(self->re);
		self->re = NULL;
		self->match_count = 0;
	}
	self->re = pcre2_compile(rex_str, PCRE2_ZERO_TERMINATED, 0, &errornumber, &erroroffset, NULL);
	if (self->re == NULL)
	{
		return UDO_REX_NO_MATCH;
	}
	if (self->match_data)
	{
		pcre2_match_data_free(self->match_data);
		self->match_data = NULL;
		self->match_count = 0;
	}
	self->match_data = pcre2_match_data_create_from_pattern(self->re, NULL);
	self->match_count = pcre2_match(self->re, self->rex_pattern, rex_pattern_length,
		0, 0, self->match_data, NULL);
	if (self->match_count <= 0)
	{
		pcre2_code_free(self->re);
		pcre2_match_data_free(self->match_data);
		self->re = NULL;
		self->match_data = NULL;
		self->match_count = 0;
		return UDO_REX_NO_MATCH;
	}
	return UDO_REX_MATCHED;
}

int udo_rex_count(udo_rex* self)
{
	return self->match_count;
}

char* udo_rex_index(udo_rex* self, int index,int* ret_length)
{
	PCRE2_SIZE *ovector;
	udo_assert(index <= self->match_count&&index >= 0);
	ovector = pcre2_get_ovector_pointer(self->match_data);
	*ret_length = ovector[2 * index+1] - ovector[2 * index];
	return self->rex_pattern + ovector[2 * index];
}

void udo_rex_term(udo_rex* self)
{
	if (self->re)
	{
		pcre2_code_free(self->re);
	}
	if (self->match_data)
	{
		pcre2_match_data_free(self->match_data);
	}
	self->match_count = 0;
	self->rex_pattern = NULL;
}

