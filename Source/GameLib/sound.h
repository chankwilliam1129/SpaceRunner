#pragma once
#include <memory>
#include <vector>
#include <dsound.h>
#include <map>
#include <string>
#include "singleton.h"

// 音源
class SoundSource
{
public:
	SoundSource(IDirectSound* ds, const char* filename);
	~SoundSource();

	void Play(bool loop);
	void Stop();
	void SetVolume(float volume);	// 0.0 ～ 1.0
	void SetPan(float pan);			// -1.0[L] ～ 1.0[R]

private:
	IDirectSoundBuffer* soundBuffer = nullptr;
};

// サウンドマネージャー
class SoundManager :public Singleton<SoundManager>
{
public:
	static std::map<std::string, std::unique_ptr<SoundSource>> Data;
	std::unique_ptr<SoundManager> soundManager = nullptr;

public:
	SoundManager() {};
	SoundManager(HWND hWnd);
	~SoundManager();

	void Initialize();
	void Release();

	static void Load();
	std::unique_ptr<SoundSource> CreateSoundSource(const char* filename);

private:
	IDirectSound* directSound = nullptr;
	IDirectSoundBuffer* primaryBuffer = nullptr;
};

// WAVEデータ
struct Wave
{
	Wave(const char* filename);

	// RIFFヘッダ
	struct Riff
	{
		UINT32				tag;			// RIFF形式の識別子 'RIFF'
		UINT32				size;			// これ以降のファイルサイズ(ファイルサイズ - 8)
		UINT32				type;			// RIFFの種類を表す識別子 'WAVE'
	};

	// チャンク
	struct Chunk
	{
		UINT32				tag;			// チャンク形式の識別子 'fmt ' 'data'
		UINT32				size;			// データサイズ('fmt 'リニアPCMならば16 'data'波形データサイズ)
	};

	// fmt チャンク
	struct Fmt
	{
		UINT16				fmtId;			// フォーマットID(リニアPCMならば1)
		UINT16				channel;		// チャンネル数(モノラル:1 ステレオ:2)
		UINT32				sampleRate;		// サンプリングレート(44.1kHzなら44100)
		UINT32				transRate;		// データ速度(Byte/sec) [ 44.1kHz 16bit ステレオ 44100×2×2 ]
		UINT16				blockSize;		// ブロックサイズ(Byte/sample×チャンネル数)
		UINT16				quantumBits;	// サンプルあたりのビット数(bit/sample)
	};

	Riff					riff;
	Fmt						fmt;
	std::vector<char>		data;
};

