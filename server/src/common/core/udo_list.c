#include "udo_list.h"
#include "../error/udo_error.h"

void udo_list_item_init(udo_list_item* self)
{
	self->next = UDO_LIST_ITEM_NOTINLIST;
	self->pre = UDO_LIST_ITEM_NOTINLIST;
}

int udo_list_item_inlist(udo_list_item* self)
{
	return self->next == UDO_LIST_ITEM_NOTINLIST&&
		self->pre == UDO_LIST_ITEM_NOTINLIST ? 0 : 1;
}

void udo_list_item_term(udo_list_item* self)
{
	udo_assert(self->next != UDO_LIST_ITEM_NOTINLIST);
	udo_assert(self->pre != UDO_LIST_ITEM_NOTINLIST);
}

void udo_list_init(udo_list* self)
{
	self->first = NULL;
	self->last = NULL;
}

udo_list_item* udo_list_begin(udo_list* self)
{
	return self->first;
}

udo_list_item* udo_list_next(udo_list* self, udo_list_item* it)
{
	return it->next;
}

udo_list_item* udo_list_end(udo_list* self)
{
	return NULL;
}


void udo_list_insert(udo_list* self, udo_list_item* it, udo_list_item* it_after)
{
	udo_list_item_inlist(!udo_list_item_inlist(it));
	it_after = it_after ? it_after : self->last;
	it->pre = it_after;
	if (it_after)
	{
		it->next = it_after->next;
		it_after->next = it;
	}
	else
	{
		it->next = NULL;
		self->first = it;
	}
	if (it->next)
	{
		it->next->pre = it;
		self->last = it;
	}
}


void udo_list_term(udo_list* self)
{
	udo_list_item_term(self->first);
	udo_list_item_term(self->last);
}





