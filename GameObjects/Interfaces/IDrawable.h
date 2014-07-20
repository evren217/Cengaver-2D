#pragma once

#include <windows.h>
#include "../UserInterface/Viewpoint.h"

class IDrawable
{
public:
	virtual void Draw(Viewpoint window)=0;
};