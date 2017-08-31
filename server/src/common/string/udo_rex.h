#ifndef UDO_REX_H__
#define UDO_REX_H__


#define	UDO_REX_NO_MATCH 0
#define UDO_REX_MATCHED 1

typedef struct udo_rex
{
	int match_count;
	void* re;
	void* match_data;
	char* rex_pattern;
}udo_rex;

void udo_rex_init(udo_rex* self);

int udo_rex_match(udo_rex* self, char* rex_pattern, int rex_pattern_length, char* rex_str);

int udo_rex_count(udo_rex* self);

char* udo_rex_index(udo_rex* self, int index, int* ret_length);

void udo_rex_term(udo_rex* self);

#endif