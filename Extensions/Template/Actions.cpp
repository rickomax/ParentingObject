
#include "Common.h"
#include <algorithm>

void Extension::SetParent(LPRO childLPRO, LPRO parentLPRO)
{
	if (parentLPRO != nullptr && childLPRO != nullptr) {
		auto child = Runtime.FixedFromLPRO(childLPRO);
		auto relationship = &relationships[child];
		relationship->parent = Runtime.FixedFromLPRO(parentLPRO);
		relationship->x = childLPRO->roHo.hoX - parentLPRO->roHo.hoX;
		relationship->y = childLPRO->roHo.hoY - parentLPRO->roHo.hoY;
	}
}

void Extension::SetParentByFixed(int child, int parent)
{
	auto parentLPRO = Runtime.LPROFromFixed(parent);
	auto childLPRO = Runtime.LPROFromFixed(child);
	if (parentLPRO != nullptr && childLPRO != nullptr) {
		auto relationship = &relationships[child];
		relationship->parent = parent;
		relationship->x = childLPRO->roHo.hoX - parentLPRO->roHo.hoX;
		relationship->y = childLPRO->roHo.hoY - parentLPRO->roHo.hoY;
	}
}

void Extension::SetFrameDelayByFixed(int child, int frameDelay)
{
	auto childLPRO = Runtime.LPROFromFixed(child);
	if (childLPRO != nullptr) {
		auto relationship = &relationships[child];
		frameDelay = std::max<int>(0, frameDelay);
		relationship->frameDelay = frameDelay;
		relationship->curFrameDelay = frameDelay;
		std::swap(relationship->lastPositions, std::queue<Point>());
	}
}

void Extension::RemoveParent(LPRO childLPRO)
{
	relationships.erase(Runtime.FixedFromLPRO(childLPRO));
}

void Extension::RemoveParentByFixed(int child)
{
	relationships.erase(child);
}

void Extension::RemoveAllParents()
{
	relationships.clear();
}

void Extension::CallCustomEvent(int fixed)
{
	customEventValue = fixed;
	Runtime.GenerateEvent(2);
}
