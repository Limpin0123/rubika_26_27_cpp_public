#include "AnimatedDrawable.h"

#ifdef _USE_IMGUI
#include "Imgui/imgui.h"
#endif // _USE_IMGUI

AnimatedDrawable::AnimatedDrawable()
{
}

AnimatedDrawable::~AnimatedDrawable()
{
}

void AnimatedDrawable::Start()
{
}

void AnimatedDrawable::Update(float deltaTime)
{
}

void AnimatedDrawable::Destroy()
{
}

void AnimatedDrawable::DrawDebug()
{
	IDrawable::DrawDebug();
	ImGui::PushID(this);
	ImGui::PopID();
}
