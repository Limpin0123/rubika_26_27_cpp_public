#pragma once

#define TAG_TYPE __int8

#define COLLIDER_TAG_NONE 0
#define COLLIDER_TAG_DYNAMIC 1 << 0


struct TagSystem
{
public:
	void AddTag(TAG_TYPE tag);
	void RemoveTag(TAG_TYPE tag);

	bool ContainsExactly(TAG_TYPE tag) const;
	bool ContainsAtLeastOne(TAG_TYPE tag) const;

private:
	TAG_TYPE Tag = 0;
};

