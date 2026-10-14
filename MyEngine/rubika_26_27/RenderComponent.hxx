template<typename T>
inline T* RenderComponent::AddDrawable()
{
	static_assert(std::is_base_of<IDrawable, T>(), "Given Drawable Type doesn't inherit from IDrawable");

	T* drawable = new T();
	drawables.push_back(drawable);
	drawables.at(drawables.size()-1)->Start();

	return drawable;
}