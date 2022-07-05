#include "MiniginPCH.h"
#include "Sound.h"
#include "SDL_mixer.h"
#include <queue>
#include <thread>
#include <condition_variable>
#include <map>
const char* audioClips[] =
{
"Bomb_Explode.wav",
"Hurt.wav",
"Music.wav"
};
dae::null_sound_system dae::ServiceLocator::m_ssDefault;
dae::sound_system* dae::ServiceLocator::m_ssInstance = &m_ssDefault;

class dae::sound_system::sound_systemImpl
{

public:
	sound_systemImpl()
	{
		m_thread = std::thread(&sound_systemImpl::Thread, this);
		m_IsRunning = true;

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			exit(2);
		}
		if (Mix_AllocateChannels(m_SoundChannels) < 0)
		{
			exit(-1);
		}
	};
	~sound_systemImpl()
	{
		m_IsRunning = false;
		m_Cv.notify_one();
		for (size_t i = 0; i < m_Sounds.size(); ++i)
		{
			Mix_FreeChunk(m_Sounds[i]);
		}
		Mix_CloseAudio();
		m_thread.join();
	};
	void RegisterSound(const sound_id id, const std::string& path)
	{
		audioClips[id] = path.c_str();
	}

	void Play(const sound_id id, const float volume)
	{
		std::unique_lock lock(m_Mutex);
		m_SoundsToPlay.push(std::make_pair(id, volume) );
		lock.unlock();
		m_Cv.notify_one();
	}


private:
	std::thread m_thread;
	std::condition_variable m_Cv;
	std::queue<std::pair< unsigned short, float>> m_SoundsToPlay;
	std::deque<Mix_Chunk*> m_Sounds;
	std::mutex m_Mutex;
	bool m_IsRunning;
	const int m_SoundChannels = 16;
	void Thread()
	{
		std::unique_lock lock(m_Mutex);

		while (m_IsRunning)
		{
			m_Cv.wait(lock);
			while (!m_SoundsToPlay.empty())
			{
				Mix_Chunk* _sample;
				const char* audioClip = audioClips[m_SoundsToPlay.front().first];
				std::string clipName = "../Data/Sound/";
				clipName.append(audioClip);
				const char* clipNameChar = clipName.c_str();

				lock.unlock();
				_sample = Mix_LoadWAV(clipNameChar);
				_sample->volume = (Uint8)m_SoundsToPlay.front().second;
				Mix_PlayChannel(-1, _sample, 0);
				m_Sounds.push_back(_sample);
				lock.lock();
				m_SoundsToPlay.pop();

				while (m_Sounds.size() >= (size_t)m_SoundChannels)
				{
					//Mix_Playing();
					Mix_FreeChunk(m_Sounds.front());
					m_Sounds.front() = nullptr;
					m_Sounds.pop_front();
					
				}

			}
		}

	}
};

dae::sound_system::sound_system()
	:m_pImpl(new sound_systemImpl())
{

}
dae::sound_system::~sound_system()
{
	delete m_pImpl;
}


void dae::sound_system::RegisterSound(const sound_id id, const std::string& path)
{
	m_pImpl->RegisterSound(id, path);
}

void dae::sound_system::Play(const sound_id id, const float volume)
{
	m_pImpl->Play(id, volume);
}



void dae::logging_sound_system::Play(const sound_id id, const float volume)
{
	m_ssReal->Play(id, volume);
	std::cout << "Playing " << id << " at volume " << volume << std::endl;
}

void dae::logging_sound_system::RegisterSound(const sound_id id, const std::string& path)
{
	m_ssReal->RegisterSound(id, path);
}

void dae::ServiceLocator::RegisterSoundSystem(sound_system* ss)
{
	m_ssInstance = ss == nullptr ? &m_ssDefault : ss;
}