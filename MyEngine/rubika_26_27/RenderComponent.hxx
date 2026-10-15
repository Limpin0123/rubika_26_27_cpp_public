#include "RenderComponent.h"
template<typename T>
inline T* RenderComponent::AddDrawable()
{
	static_assert(std::is_base_of<IDrawable, T>(), "Given Drawable Type doesn't inherit from IDrawable");

	T* drawable = new T();
	drawables.push_back(drawable);
	drawables.at(drawables.size()-1)->Start();

	return drawable;
}

template<typename T>
inline void RenderComponent::RemoveDrawable(T* drawable)
{
	static_assert(std::is_base_of<IDrawable, T>(), "Given Drawable Type doesn't inherit from IDrawable");

	auto it = std::find(drawables.begin(), drawables.end(), drawable);
	if (it == drawables.end())
	{
		return;
	}
	IDrawable* d = *it;

	d->Destroy();
	drawables.erase(it);

	delete d;
}
