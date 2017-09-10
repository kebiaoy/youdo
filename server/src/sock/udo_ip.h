#ifndef udo_ip_h__
#define udo_ip_h__

typedef struct udo_ip
{
	unsigned int ip;
}udo_ip;

void udo_ip_init(udo_ip* self);

void udo_ip_setformat(udo_ip* self, unsigned char* ip);

unsigned char* udo_ip_getformat(udo_ip* self);

void udo_ip_setnet(udo_ip* self, unsigned char* ip);

unsigned char* udo_ip_getnet(udo_ip* self);

void udo_ip_copy(udo_ip* self, udo_ip* other);

int udo_ip_equal(udo_ip* self, udo_ip* other);

void udo_ip_term(udo_ip* self);

#endif // udo_ip_h__
