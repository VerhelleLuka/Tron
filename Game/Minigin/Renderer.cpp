#include "MiniginPCH.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl2.h>
#include "../3rdParty/imgui-plot-master/include/imgui_plot.h"
#include "../3rdParty/ImPlot/implot.h"
#include "TrashTheCache.h"
#include "Structs.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();

}

void dae::Renderer::Render()
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render();

	////Trash the cache exercise
	//ImGui_ImplOpenGL2_NewFrame();
	//ImGui_ImplSDL2_NewFrame(m_Window);
	//ImGui::NewFrame();
	//ImGui::ShowDemoWindow();
	//ImGui::Begin("Exercise1");
	//if (ImGui::Button("TrashTheCache"))
	//{
	//	GetCacheData(1);
	//}
	//if (ImPlot::BeginPlot(" "))
	//{
	//	ImPlot::PlotLine("Plot", m_XData, m_YData, 11);
	//	ImPlot::EndPlot();
	//}
	//ImGui::End();
	////
	//ImGui::Begin("Exercise2");
	//if (ImGui::Button("TrashTheCache GameObject3D"))
	//{
	//	GetCacheData(2);
	//}
	//if (ImPlot::BeginPlot("GameObject3D"))
	//{
	//	ImPlot::PlotLine("Plot", m_XData, m_YData3D, 11);
	//	ImPlot::EndPlot();
	//}
	//if (ImGui::Button("TrashTheCache GameObject3DAlt"))
	//{
	//	GetCacheData(3);
	//}
	//if (ImPlot::BeginPlot("GameObject3DAlt"))
	//{
	//	ImPlot::PlotLine("Plot", m_XData, m_YData3DAlt, 11);
	//	ImPlot::EndPlot();
	//}
	//if (ImPlot::BeginPlot("Combined: "))
	//{
	//	ImPlot::PlotLine("3DObjectAlt", m_XData, m_YData3DAlt, 11);
	//	ImPlot::PlotLine("3DObject", m_XData, m_YData3D, 11);
	//	ImPlot::EndPlot();
	//}
	//ImGui::End();
	//ImGui::Render();
	//ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_Renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, Float4 srcRect, Float4 dstRect, bool isFlipped = false) const
{
	SDL_Rect source{
		(int)srcRect.x,
		(int)srcRect.y,
		(int)srcRect.z,
		(int)srcRect.w
	};

	SDL_Rect dest{
	(int)dstRect.x,
	(int)dstRect.y,
	(int)dstRect.z,
	(int)dstRect.w
	};
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (isFlipped)
		 flip = SDL_FLIP_HORIZONTAL;
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &source, &dest, 0.f, NULL,flip );
}

void dae::Renderer::GetCacheData(int fromWhere)
{

	if (fromWhere == 1)
	{
		auto yData = m_CacheData.IntegerCache();
		for (size_t i = 0; i < yData.size(); ++i)
		{
			m_YData[i] = yData[i];
		}
	}
	else if (fromWhere == 2)
	{
		auto yData3D = m_CacheData.GameObject3DTime();
		for (size_t i = 0; i < yData3D.size(); ++i)
		{
			m_YData3D[i] = yData3D[i];
		}
	}
	else if (fromWhere == 3)
	{
		auto yData3DAlt = m_CacheData.GameObject3DAltTime();
		for (size_t i = 0; i < yData3DAlt.size(); ++i)
		{
			m_YData3DAlt[i] = yData3DAlt[i];
		}
	}
}
