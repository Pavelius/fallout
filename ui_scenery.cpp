#include "main.h"

static struct scenery : public animable {
	int	getid() const override;
} objects[LastScenery - FirstScenery + 1];

int scenery::getid() const {
	return this - objects + FirstScenery;
}