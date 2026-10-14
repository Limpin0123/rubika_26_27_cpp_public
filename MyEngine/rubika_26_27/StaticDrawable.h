#pragma once

#include "IDrawable.h"

class StaticDrawable : public IDrawable
{
public: 
	StaticDrawable();
	~StaticDrawable();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;
};

