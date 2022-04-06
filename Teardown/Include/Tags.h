#pragma once

#include "small_string.h"
#include "Entities.h"

namespace Teardown {
	typedef void (*tSetTag) (CEntity* pEntity, small_string* ssName, small_string* ssValue);
	inline tSetTag SetTag;

	typedef bool (*tHasTag) (CEntity* pEntity, small_string* ssName);
	inline tHasTag HasTag;

	typedef void (*tRemoveTag) (CEntity* pEntity, small_string* ssName);
	inline tRemoveTag RemoveTag;

	typedef small_string* (*tGetTag) (CEntity* pEntity, small_string* ssReturn, small_string* ssName);
	inline tGetTag GetTag;
}