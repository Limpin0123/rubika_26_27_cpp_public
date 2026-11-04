#pragma once

#define TAG_TYPE __int8

#define COLLIDER_TAG_NONE 0
#define COLLIDER_TAG_STATIC 1 << 0
#define COLLIDER_TAG_DYNAMIC 1 << 1


struct TagSystem
{
public:
	void AddTag(TAG_TYPE tag);
	void RemoveTag(TAG_TYPE tag);

	bool ContainsExactly(TAG_TYPE tag);
	bool ContainsAtLeastOne(TAG_TYPE tag);

private:
	TAG_TYPE Tag = 0;
};

