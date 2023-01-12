
#include "Common.h"


///
/// EXTENSION CONSTRUCTOR/DESTRUCTOR
///

Extension::Extension(LPRDATA _rdPtr, LPEDATA edPtr, fpcob cobPtr)
	: rdPtr(_rdPtr), rhPtr(_rdPtr->rHo.hoAdRunHeader), Runtime(_rdPtr)
{
	/*
		Link all your action/condition/expression functions to their IDs to match the
		IDs in the JSON here
	*/

	LinkAction(0, SetParent);
	LinkAction(1, SetParentByFixed);
	LinkAction(2, RemoveParent);
	LinkAction(3, RemoveParentByFixed);
	LinkAction(4, RemoveAllParents);
	LinkAction(5, CallCustomEvent);
	LinkAction(6, SetFrameDelayByFixed);
	
	LinkCondition(0, PickObjectParent);
	LinkCondition(1, PickObjectParentByFixed);
	LinkCondition(2, OnCustomEvent);
	//LinkCondition(3, PickObjectsParent);
	//LinkCondition(4, PickObjectsParentByFixed);

	LinkExpression(0, GetCustomEventValue);

	/*
		This is where you'd do anything you'd do in CreateRunObject in the original SDK

		It's the only place you'll get access to edPtr at runtime, so you should transfer
		anything from edPtr to the extension class here.

	*/



}

Extension::~Extension()
{
	/*
		This is where you'd do anything you'd do in DestroyRunObject in the original SDK.
		(except calling destructors and other such atrocities, because that's automatic in Edif)
	*/



}


short Extension::Handle()
{
	/*
	   If your extension will draw to the MMF window you should first
	   check if anything about its display has changed :

		   if (rdPtr->roc.rcChanged)
			  return REFLAG_DISPLAY;
		   else
			  return 0;

	   You will also need to make sure you change this flag yourself
	   to 1 whenever you want to redraw your object

	   If your extension won't draw to the window, but it still needs
	   to do something every MMF loop use :

			return 0;

	   If you don't need to do something every loop, use :

			return REFLAG_ONESHOT;

	   This doesn't mean this function can never run again. If you want MMF
	   to handle your object again (causing this code to run) use this function:

			Runtime.Rehandle();

	   At the end of the loop this code will run

	*/

	for (auto it = relationships.begin(); it != relationships.end(); it++) {
		auto childLPRO = Runtime.LPROFromFixed(it->first);
		auto relationship = it->second;
		auto parentLPRO = Runtime.LPROFromFixed(relationship.parent);
		auto hasToDelete = false;
		if (childLPRO != nullptr) {
			if (parentLPRO != nullptr) {
				if (relationship.frameDelay > 0) {
					Point point;
					point.x = parentLPRO->roHo.hoX;
					point.y = parentLPRO->roHo.hoY;
					relationship.lastPositions.emplace(point);
					if (relationship.curFrameDelay > 0) {
						relationship.curFrameDelay--;
					}
					else {
						Point lastPoint = relationship.lastPositions.back();
						childLPRO->roHo.hoX = lastPoint.x + relationship.x;
						childLPRO->roHo.hoY = lastPoint.y + relationship.y;
					}
				}
				else {
					childLPRO->roHo.hoX = parentLPRO->roHo.hoX + relationship.x;
					childLPRO->roHo.hoY = parentLPRO->roHo.hoY + relationship.y;
				}
				childLPRO->roc.rcChanged = true;
				childLPRO->roc.rcCheckCollides = true;
			}
		}
	}
	return 0;
}


short Extension::Display()
{
	/*
	   If you return REFLAG_DISPLAY in Handle() this routine will run.
	*/

	// Ok
	return 0;
}

short Extension::Pause()
{

	// Ok
	return 0;
}

short Extension::Continue()
{

	// Ok
	return 0;
}

bool Extension::Save(HANDLE File)
{
	bool OK = false;

#ifndef VITALIZE

	// Save the object's data here

	OK = true;

#endif

	return OK;
}

bool Extension::Load(HANDLE File)
{
	bool OK = false;

#ifndef VITALIZE

	// Load the object's data here

	OK = true;

#endif

	return OK;
}


// These are called if there's no function linked to an ID

void Extension::Action(int ID, LPRDATA rdPtr, long param1, long param2)
{

}

long Extension::Condition(int ID, LPRDATA rdPtr, long param1, long param2)
{

	return false;
}

long Extension::Expression(int ID, LPRDATA rdPtr, long param)
{
	// Default: read parameters otherwise the Windows runtime can crash in expressions
	LPEVENTINFOS2 Infos = GetEventInformations((LPEVENTINFOS2) &::SDK->ExpressionInfos[0], ID);
	for (int i = 0; i < Infos->infos.nParams; ++i)
	{
		switch (EVINFO2_PARAM(Infos, i))
		{
		case EXPPARAM_STRING:
			if (!i)
			{
				CNC_GetFirstExpressionParameter(rdPtr, param, TYPE_STRING);
				break;
			}
			CNC_GetNextExpressionParameter(rdPtr, param, TYPE_STRING);
			break;

		case EXPPARAM_LONG:

			int Type = ((::SDK->ExpressionFloatFlags[ID] & (1 << i)) != 0) ? TYPE_FLOAT : TYPE_LONG;
			if (!i)
			{
				CNC_GetFirstExpressionParameter(rdPtr, param, Type);
				break;
			}
			CNC_GetNextExpressionParameter(rdPtr, param, Type);
			break;
		}
	}

	return 0;
}

