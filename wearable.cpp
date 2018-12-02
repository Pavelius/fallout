#include "main.h"

adat<wearable::element, 2048> wearable_data;

void wearable::additem(item& it) {
	for(auto& e : wearable_data) {
		if(e.source == this && e.ismatch(it)) {
			e.join(it);
			if(!it)
				return;
		}
	}
	for(auto& e : wearable_data) {
		if(!e) {
			*((item*)&e) = it;
			e.source = this;
			return;
		}
	}
	auto p = wearable_data.add();
	*((item*)p) = it;
	p->source = this;
}

item* wearable::find(item_s type) {
	for(auto& e : wearable_data) {
		if(e.source == this && e.get() == type)
			return &e;
	}
	return 0;
}

aref<item*> wearable::select(aref<item*> result) const {
	auto ps = result.data;
	auto pe = ps + result.count;
	for(auto& e : wearable_data) {
		if(e && e.source == this) {
			if(ps < pe)
				*ps++ = &e;
			if(ps >= pe)
				break;
		}
	}
	result.count = ps - result.data;
	return result;
}