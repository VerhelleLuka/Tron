#pragma once
#include <string>
#include "Texture2D.h"
#include "Structs.h"
#include "Transform.h"
namespace dae
{
	class Animation final
	{
	public:
		Animation(int rows, int nrFrames);
		~Animation() = default;
		Animation(const Animation& other) = delete;
		Animation(Animation&& other) = delete;
		Animation& operator=(const Animation& other) = delete;
		Animation& operator=(Animation&& other) = delete;
		void Update(float deltaTime,Transform transform);
		void Render();

		void SetTexture(const std::string& fileName);
		void SetScale(float scale) { m_Scale = scale; }


		float GetScaledWidth() const{ return (float)m_Width * m_Scale; }
		float GetScaledHeight() const{ return (float)m_Height * m_Scale; }
		//Set all positions to the position of the sprite that is in use right now
		void SetReversed(bool flipped) { m_IsFlipped = flipped; }


		int GetFrameNr() const { return m_CurrentFrame; }
		void SetFrameNr(int frame) { m_CurrentFrame = frame; }
		int GetNrFrames() const { return m_NrFrames; }
	private:

		std::shared_ptr<Texture2D> m_pTexture;
		int  m_Rows;
		int m_NrFrames, m_CurrentFrame;
		float m_FramesSec = 0.16f;
		float m_FrameChangeCounter = 0.f;
		Float4 m_DstRect, m_SrcRect;
		float m_Scale{ 1.0f };
		bool m_IsFlipped = false;

		int m_Width{0}, m_Height{ 0 };

	};
}