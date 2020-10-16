#pragma once

namespace sdk
{
	class IVRenderView
	{
	public:
		void set_blend(float alpha) {
			using original_fn = void(__thiscall*)(IVRenderView*, float);
			return (*(original_fn**)this)[4](this, alpha);
		}

		void color_modulation(float const* colors)
		{
			using original_fn = void(__thiscall*)(IVRenderView*, float const*);
			return (*(original_fn**)this)[6](this, colors);
		}
	};
}