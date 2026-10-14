#pragma once

#include "IDrawable.h"
class AnimatedDrawable : public IDrawable
{
public:
	AnimatedDrawable();
	~AnimatedDrawable();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;
};

