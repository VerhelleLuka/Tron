#pragma once
#include "Singleton.h"
#include "TrashTheCache.h"
#include "Structs.h"
#include "SDL_render.h"
#include "SDL.h"
namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, Float4 srcRect, Float4 dstRect, bool isFlipped) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

		//Stuff I added
	private:
		const float m_XData[11] = { 1,2,4,8,16,32,64,128,256,512,1024 };
		TrashTheCache m_CacheData;
		float m_YData[11] = {100000,0,0,0,0,0,0,0,0,0,0};
		float m_YData3D[11] = {100000,0,0,0,0,0,0,0,0,0,0};
		float m_YData3DAlt[11] = {100000,0,0,0,0,0,0,0,0,0,0};
		void GetCacheData(int fromWhere);



	};
}

