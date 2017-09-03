#ifndef udo_list_h__
#define udo_list_h__

#define UDO_LIST_ITEM_NOTINLIST ((udo_list_item*)-1)

typedef struct udo_list_item
{
	struct udo_list_item* pre;
	struct udo_list_item* next;
}udo_list_item;

void udo_list_item_init(udo_list_item* self);

int udo_list_item_inlist(udo_list_item* self);

void udo_list_item_term(udo_list_item* self);


typedef struct udo_list
{
	udo_list_item* first;
	udo_list_item* last;
}udo_list;

void udo_list_init(udo_list* self);

udo_list_item* udo_list_begin(udo_list* self);

udo_list_item* udo_list_next(udo_list* self, udo_list_item* it);

udo_list_item* udo_list_end(udo_list* self);

void udo_list_insert(udo_list* self, udo_list_item* it, udo_list_item* it_after);

void udo_list_term(udo_list* self);




#endif // udo_list_h__
