#include "TagSystem.h"

void TagSystem::AddTag(TAG_TYPE tag)
{
	Tag |= tag;
}

void TagSystem::RemoveTag(TAG_TYPE tag)
{
	Tag &= ~tag;
}

bool TagSystem::ContainsExactly(TAG_TYPE tag) const
{
	return (Tag & tag) == tag;
}

bool TagSystem::ContainsAtLeastOne(TAG_TYPE tag) const
{
	return (Tag & tag) != 0;
}