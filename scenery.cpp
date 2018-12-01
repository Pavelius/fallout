#include "main.h"

static struct scenery : public animable {
	int	getid() const override;
} objects[LastScenery - FirstScenery + 1];

int scenery::getid() const {
	return this - objects + FirstScenery;
}

static struct scenery_drawable_plugin : public drawable::plugin {
	void updating() override {
		for(int i = 0; i < scenery_manager.count; i++)
			objects[i].update();
	}
	void selecting(drawable** result, unsigned flags) override {
		if((flags&DrawableModeMask) != 0)
			return;
		auto p = result;
		for(int i = 0; i < scenery_manager.count; i++)
			*p++ = objects + i;
		*p = 0;
	}
} drawable_plugin;