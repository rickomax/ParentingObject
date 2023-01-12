#include "Common.h"

bool Extension::PickObjectParent(LPRO childLPRO)
{
	if (childLPRO != nullptr) {
		auto child = Runtime.FixedFromLPRO(childLPRO);
		if (relationships.find(child) != relationships.end()) {
			auto relationship = &relationships[child];
			auto parentLPRO = Runtime.LPROFromFixed(relationship->parent);
			if (parentLPRO != nullptr) {
				Runtime.ObjectSelection.SelectOneObject(parentLPRO);
				return true;
			}
		}
	}
	return false;
}

bool Extension::PickObjectParentByFixed(int child)
{
	auto childLPRO = Runtime.LPROFromFixed(child);
	if (childLPRO != nullptr) {
		if (relationships.find(child) != relationships.end()) {
			auto relationship = &relationships[child];
			auto parentLPRO = Runtime.LPROFromFixed(relationship->parent);
			if (parentLPRO != nullptr) {
				Runtime.ObjectSelection.SelectOneObject(parentLPRO);
				return true;
			}
		}
	}
	return false;
}

bool Extension::OnCustomEvent()
{
	return true;
}