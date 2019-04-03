#include "Core.h"

Core core;
Background background;
Background ending;
HP hp;
HP enemyhp;
MainTitle intro;
IXAudio2SourceVoice* pSourceVoice; //BGM
XAUDIO2_BUFFER buffer = {0};
IXAudio2SourceVoice* pEffectVoice; //효과음 전역변수화
XAUDIO2_BUFFER Effectbuffer = {0};
IXAudio2SourceVoice* pEndingVoice; //엔딩 전역변수화
XAUDIO2_BUFFER Endingbuffer = {0};


int stageNum = 0;
bool startBtn = false;

void startStage(int num) {
	for (int i = 0; i < 200; i++) {
		body[i].initializeBody();
	}
	for (int i = 0; i < 10; i++) {
		enemy[i].initializeEnemy();
	}
	core.initializeCore();

	//initialize enemies
	initEnemy();
	initFood();
	InitbodyInform();

	for (int i = 1; i < 50; i++)  //먹히면 isExist를 false로
		food[i].set_Exist();

	for (int j = 0; j < 10; j++)
		enemy[j].set_Exist();


	core.set_Exist();

	if (num == 1) {
		//stage 1
		//-------------------------------------------------------------------
		body[100].set_Exist();
		body[100].set_weapon();

		body[150].set_Exist();
		body[150].set_tail();

		core.insertBody(1, 150);
		core.insertBody(5, 100);

		for (int i = 0; i < 10; i++)
		{
			body[101 + i*2].set_Exist();
			body[101 + i*2].set_weapon();
			bodyInform[101 + i].is_weapone = true;

			body[102 + i*2].set_Exist();
			body[102 + i*2].set_weapon();
			bodyInform[102 + i*2].is_weapone = true;

			body[151 + i].set_Exist();
			body[151 + i].set_tail();
			bodyInform[151 + i].is_Tail = true;

			enemy[i].insertBody(1, 151 + i);

			enemy[i].insertAnyWhere(101 + i * 2);
			enemy[i].setLeaves();
			enemy[i].insertAnyWhere(102 + i * 2);
			enemy[i].setLeaves();
		}
		//-------------------------------------------------------------------
	}
	else if (num == 2) {
		//stage 2
		//-------------------------------------------------------------------
		body[1].set_Exist();
		body[2].set_Exist();
		body[3].set_Exist();

		body[100].set_Exist();
		body[100].set_weapon();
		body[101].set_Exist();
		body[101].set_weapon();

		body[150].set_Exist();
		body[150].set_tail();
		body[151].set_Exist();
		body[151].set_tail();

		core.insertBody(3, 1);
		core.insertBody(4, 2);
		core.insertBody(5, 3);

		body[1].insertBody(3, 100);
		body[1].insertBody(4, 150);
		body[2].insertBody(4, 101);
		body[2].insertBody(3, 151);
		core.setLeaves();

		for (int i = 0; i < 10; i++)
		{
			body[4 + i*5].set_Exist();
			body[5 + i*5].set_Exist();
			body[6 + i*5].set_Exist();
			body[7 + i*5].set_Exist();
			body[8 + i * 5].set_Exist();

			body[102 + i * 4].set_Exist();
			body[102 + i * 4].set_weapon();
			bodyInform[102 + i * 4].is_weapone = true;
			body[103 + i * 4].set_Exist();
			body[103 + i * 4].set_weapon();
			bodyInform[103 + i * 4].is_weapone = true;
			body[104 + i * 4].set_Exist();
			body[104 + i * 4].set_weapon();
			bodyInform[104 + i * 4].is_weapone = true;
			body[105 + i * 4].set_Exist();
			body[105 + i * 4].set_weapon();
			bodyInform[105 + i * 4].is_weapone = true;

			body[152 + i*3].set_Exist();
			body[152 + i*3].set_tail();
			bodyInform[152 + i * 3].is_Tail = true;
			body[153 + i*3].set_Exist();
			body[153 + i*3].set_tail();
			bodyInform[153 + i * 3].is_Tail = true;
			body[154 + i * 3].set_Exist();
			body[154 + i * 3].set_tail();
			bodyInform[154 + i * 3].is_Tail = true;
	
			enemy[i].insertBody(3, 4 + i * 5);
			enemy[i].insertBody(4, 5 + i * 5);
			enemy[i].insertBody(5, 6 + i * 5);
			enemy[i].insertBody(1, 7 + i * 5);
			enemy[i].insertBody(2, 8 + i * 5);

			body[4 + i * 5].insertBody(3, 102 + i * 4);
			body[4 + i * 5].insertBody(4, 152 + i * 3);
			body[5 + i * 5].insertBody(4, 103 + i * 4);
			body[5 + i * 5].insertBody(3, 153 + i * 3);
			enemy[i].setLeaves();

			
			enemy[i].insertAnyWhere(104 + i * 4);
			enemy[i].setLeaves();
			enemy[i].insertAnyWhere(105 + i * 4);
			enemy[i].setLeaves();
			enemy[i].insertAnyWhere(154 + i * 3);
			enemy[i].setLeaves();
		}
		//-------------------------------------------------------------------
	}
	else if (num == 3) {
		//stage 3
		//-------------------------------------------------------------------
		body[1].set_Exist();
		body[2].set_Exist();
		body[3].set_Exist();
		body[4].set_Exist();

		body[100].set_Exist();
		body[100].set_weapon();
		body[101].set_Exist();
		body[101].set_weapon();
		body[102].set_Exist();
		body[102].set_weapon();

		body[150].set_Exist();
		body[150].set_tail();
		body[151].set_Exist();
		body[151].set_tail();
		body[152].set_Exist();
		body[152].set_tail();
		body[153].set_Exist();
		body[153].set_tail();

		core.insertBody(4, 1);
		core.insertBody(3, 2);
		core.insertBody(5, 100);
		
		body[1].insertBody(3, 150);
		body[1].insertBody(4, 3);
		body[1].insertBody(5, 151);

		body[2].insertBody(3, 4);
		body[2].insertBody(4, 152);
		body[2].insertBody(5, 153);
		core.setLeaves();

		body[3].insertBody(3, 101);
		body[4].insertBody(4, 102);
		core.setLeaves();

		for (int i = 0; i < 10; i++)
		{
			body[5 + i * 7].set_Exist();
			body[6 + i * 7].set_Exist();
			body[7 + i * 7].set_Exist();
			body[8 + i * 7].set_Exist();
			body[9 + i * 7].set_Exist();
			body[10 + i * 7].set_Exist();
			body[11 + i * 7].set_Exist();

			body[80 + i].set_Exist();
			body[80 + i].set_HP(20);

			body[103 + i * 4].set_Exist();
			body[103 + i * 4].set_weapon();
			bodyInform[103 + i * 4].is_weapone = true;
			body[104 + i * 4].set_Exist();
			body[104 + i * 4].set_weapon();
			bodyInform[104 + i * 4].is_weapone = true;
			body[105 + i * 4].set_Exist();
			body[105 + i * 4].set_weapon();
			bodyInform[105 + i * 4].is_weapone = true;
			body[106 + i * 4].set_Exist();
			body[106 + i * 4].set_weapon();
			bodyInform[106 + i * 4].is_weapone = true;

			body[154 + i * 4].set_Exist();
			body[154 + i * 4].set_tail();
			bodyInform[154 + i * 4].is_Tail = true;
			body[155 + i * 4].set_Exist();
			body[155 + i * 4].set_tail();
			bodyInform[155 + i * 4].is_Tail = true;
			body[156 + i * 4].set_Exist();
			body[156 + i * 4].set_tail();
			bodyInform[156 + i * 4].is_Tail = true;
			body[157 + i * 4].set_Exist();
			body[157 + i * 4].set_tail();
			bodyInform[157 + i * 4].is_Tail = true;

			enemy[i].insertBody(4, 5 + i * 7);
			enemy[i].insertBody(3, 6 + i * 7);
			enemy[i].insertBody(5, 11 + i * 7);
			enemy[i].insertBody(1, 9 + i * 7);
			enemy[i].insertBody(2, 10 + i * 7);

			body[5 + i * 7].insertBody(3, 154 + i * 4);
			body[5 + i * 7].insertBody(4, 7 + i * 7);
			body[5 + i * 7].insertBody(5, 155 + i * 4);
			enemy[i].setLeaves();

			body[6 + i * 7].insertBody(3, 8 + i * 7);
			body[6 + i * 7].insertBody(4, 156 + i * 4);
			body[6 + i * 7].insertBody(5, 157 + i * 4);
			enemy[i].setLeaves();

			body[7 + i * 7].insertBody(3, 104 + i * 4);
			enemy[i].setLeaves();

			body[8 + i * 7].insertBody(4, 105 + i * 4);
			enemy[i].setLeaves();
	
			enemy[i].insertAnyWhere(80 + i);
			enemy[i].setLeaves();
			enemy[i].insertAnyWhere(103 + i * 4);
			enemy[i].setLeaves();
			enemy[i].insertAnyWhere(106 + i * 4);
			enemy[i].setLeaves();
			
		}
		//-------------------------------------------------------------------
	}
	else if (num == 4) {
		//boss
		//-------------------------------------------------------------------
		
		//player
		int x;
		int z;
		for (int i = 1; i < 15; i++)
		{
			x = rand() % 120;	//0~120
			z = rand() % 120;	//0~120
			body[i].set_Exist();
			body[i].setWorldCoord(x - 60, 0, z - 60);//-60~60
			body[i].setCollisionBox();
		}
		for (int i = 100; i < 110; i++)
		{
			x = rand() % 120;	//0~120
			z = rand() % 120;	//0~120
			body[i].set_Exist();
			body[i].set_weapon();
			body[i].setWorldCoord(x - 60, 0, z - 60);//-60~60
			body[i].setCollisionBox();
		}
		for (int i = 150; i < 160; i++)
		{
			x = rand() % 120;	//0~120
			z = rand() % 120;	//0~120
			body[i].set_Exist();
			body[i].set_tail();
			body[i].setWorldCoord(x - 60, 0, z - 60);//-60~60
			body[i].setCollisionBox();
		}

		//boss
		for (int i = 15; i < 34; i++) {
			body[i].set_Exist();
		}
		body[17].set_Delete();
		body[22].set_Delete();
		body[26].set_Delete();
		body[32].set_Delete();

		for (int i = 85; i < 89; i++) {
			body[i].set_Exist();
			body[i].set_HP(20);
		}
		for (int i = 110; i < 126; i++) {
			body[i].set_weapon();
			body[i].set_Exist();
		}
		for (int i = 160; i < 164; i++) {
			body[i].set_tail();
			body[i].set_Exist();
		}
		
		for (int j = 1; j < 10; j++) {
			enemy[j].set_Delete();
			enemyLocation[j].Exist = false;
		}

		enemy[0].insertBody(5, 15);

		body[15].insertBody(3, 25);
		body[15].insertBody(4, 16);
		body[15].insertBody(5, 160);
		body[15].insertBody(1, 118);
		body[15].insertBody(2, 119);
		enemy[0].setLeaves();

		body[16].insertBody(3, 110);
		body[16].insertBody(5, 85);
		enemy[0].setLeaves();

		body[85].insertBody(4, 18);
		enemy[0].setLeaves();

		body[18].insertBody(3, 112);
		body[18].insertBody(5, 19);
		enemy[0].setLeaves();

		body[19].insertBody(3, 161);
		body[19].insertBody(4, 20);
		body[19].insertBody(5, 21);
		body[19].insertBody(1, 120);
		body[19].insertBody(2, 121);
		enemy[0].setLeaves();

		body[21].insertBody(3, 113);
		body[21].insertBody(5, 86);
		enemy[0].setLeaves();

		body[86].insertBody(4, 23);
		enemy[0].setLeaves();

		body[23].insertBody(3, 116);
		body[23].insertBody(5, 24);
		enemy[0].setLeaves();

		body[24].insertBody(3, 163);
		body[24].insertBody(1, 124);
		body[24].insertBody(2, 125);
		enemy[0].setLeaves();

		body[25].insertBody(4, 111);
		body[25].insertBody(5, 87);
		enemy[0].setLeaves();

		body[87].insertBody(3, 27);
		enemy[0].setLeaves();

		body[27].insertBody(4, 114);
		body[27].insertBody(5, 28);
		enemy[0].setLeaves();

		body[28].insertBody(3, 29);
		body[28].insertBody(4, 162);
		body[28].insertBody(5, 31);
		body[28].insertBody(1, 122);
		body[28].insertBody(2, 123);
		enemy[0].setLeaves();

		body[29].insertBody(5, 30);
		enemy[0].setLeaves();

		body[31].insertBody(4, 115);
		body[31].insertBody(5, 88);
		enemy[0].setLeaves();

		body[88].insertBody(3, 33);
		enemy[0].setLeaves();

		body[33].insertBody(4, 117);
		enemy[0].setLeaves();
		//-------------------------------------------------------------------
	}
	else if (num == 5) {
		//ending
		//-------------------------------------------------------------------
		pSourceVoice->Stop();
		pEndingVoice->Start( 0 ); //엔딩곡 실행
		body[1].set_Exist();
		body[2].set_Exist();
		body[3].set_Exist();
		body[4].set_Exist();
		body[5].set_Exist();

		body[100].set_Exist();
		body[100].set_weapon();
		body[101].set_Exist();
		body[101].set_weapon();

		body[150].set_Exist();
		body[150].set_tail();
		body[151].set_Exist();
		body[151].set_tail();

		core.insertBody(2, 1);

		body[1].insertBody(3, 2);
		body[1].insertBody(4, 4);
		core.setLeaves();

		body[2].insertBody(5, 150);
		body[2].insertBody(2, 3);
		core.setLeaves();

		body[3].insertBody(3, 100);
		core.setLeaves();

		body[4].insertBody(5, 151);
		body[4].insertBody(2, 5);
		core.setLeaves();

		body[5].insertBody(4, 101);
		core.setLeaves();

		core.setRotate(-XM_PI/2);
		core.setLeaves();

		for (int j = 0; j < 10; j++)
			enemy[j].set_Delete();
		//-------------------------------------------------------------------
	}
}


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

		
	//--------------------------------------------------------------------------------------
	// 음악 실행하기
	    //
    // Initialize XAudio2
    //
    CoInitializeEx( nullptr, COINIT_MULTITHREADED );

	IXAudio2* pXAudio2 = nullptr;	
    UINT32 flags = 0;
    HRESULT hr = XAudio2Create( &pXAudio2, flags );
    if( FAILED( hr ) )
    {
      //  wprintf( L"Failed to init XAudio2 engine: %#X\n", hr );
        CoUninitialize();
        return 0;
    }
    //
    // Create a mastering voice
    //
    IXAudio2MasteringVoice* pMasteringVoice = nullptr;

    if( FAILED( hr = pXAudio2->CreateMasteringVoice( &pMasteringVoice ) ) )
    {
       // wprintf( L"Failed creating mastering voice: %#X\n", hr );
        SAFE_RELEASE( pXAudio2 );
        CoUninitialize();
        return 0;
    }
	
	//--------------------------------------------------------------------------------------

	LPCWSTR szFilename=L"Music\\bgm.wav";
	WCHAR strFilePath[MAX_PATH];
    hr = FindMediaFileCch( strFilePath, MAX_PATH, szFilename );
    if( FAILED( hr ) )
    {
       // wprintf( L"Failed to find media file: %s\n", szFilename );
        return hr;
    }

    //
    // Read in the wave file
    //
    std::unique_ptr<uint8_t[]> waveFile;
    DirectX::WAVData waveData;
    if ( FAILED( hr = DirectX::LoadWAVAudioFromFileEx( strFilePath, waveFile, waveData ) ) )
    {
        return hr;
    }

    //
    // Play the wave using a XAudio2SourceVoice
    //

    // Create the source voice	
    if( FAILED( hr = pXAudio2->CreateSourceVoice( &pSourceVoice, waveData.wfx ) ) )
    {
      
        return hr;
    }

    // Submit the wave sample data using an XAUDIO2_BUFFER structure
    buffer.pAudioData = waveData.startAudio;
    buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
    buffer.AudioBytes = waveData.audioBytes;

    if ( waveData.loopLength > 0 )
    {
        buffer.LoopBegin = waveData.loopStart;
        buffer.LoopLength = waveData.loopLength;
        buffer.LoopCount = 1; // We'll just assume we play the loop twice
    }else if( FAILED( hr = pSourceVoice->SubmitSourceBuffer( &buffer ) ) )
    {
        pSourceVoice->DestroyVoice();
        return hr;
    }
	hr = pSourceVoice->Start( 0 );
	
    // Let the sound play
    BOOL isRunning = TRUE;
	
	// 효과음 적용해보기--------------------------------------------------------------------
	CoInitializeEx( nullptr, COINIT_MULTITHREADED );

	IXAudio2* pXEffectAudio2 = nullptr;	
    UINT32 Effectflags = 0;
    hr = XAudio2Create( &pXEffectAudio2, Effectflags );
    if( FAILED( hr ) )
    {
      //  wprintf( L"Failed to init XAudio2 engine: %#X\n", hr );
        CoUninitialize();
        return 0;
    }
    //
    // Create a mastering voice
    //
    IXAudio2MasteringVoice* pMasteringEffectVoice = nullptr;

    if( FAILED( hr = pXEffectAudio2->CreateMasteringVoice( &pMasteringEffectVoice ) ) )
    {
       // wprintf( L"Failed creating mastering voice: %#X\n", hr );
        SAFE_RELEASE( pXEffectAudio2 );
        CoUninitialize();
        return 0;
    }
	//IXAudio2SourceVoice* pEffectVoice;
	//XAUDIO2_BUFFER Effectbuffer = {0};
	LPCWSTR szEffectFilename=L"Music\\effect.wav";
	WCHAR strEffectFilePath[MAX_PATH];
    hr = FindMediaFileCch( strEffectFilePath, MAX_PATH, szEffectFilename );
    if( FAILED( hr ) )
    {
       // wprintf( L"Failed to find media file: %s\n", szFilename );
        return hr;
    }

    //
    // Read in the wave file
    //
    std::unique_ptr<uint8_t[]> waveEffectFile;
    DirectX::WAVData waveEffectData;
    if ( FAILED( hr = DirectX::LoadWAVAudioFromFileEx( strEffectFilePath, waveEffectFile, waveEffectData ) ) )
    {
        return hr;
    }

    //
    // Play the wave using a XAudio2SourceVoice
    //

    // Create the source voice	
    if( FAILED( hr = pXEffectAudio2->CreateSourceVoice( &pEffectVoice, waveEffectData.wfx ) ) )
    {
      
        return hr;
    }

    // Submit the wave sample data using an XAUDIO2_BUFFER structure
    Effectbuffer.pAudioData = waveEffectData.startAudio;
    Effectbuffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
    Effectbuffer.AudioBytes = waveEffectData.audioBytes;

    if ( waveEffectData.loopLength > 0 )
    {
        Effectbuffer.LoopBegin = waveEffectData.loopStart;
        Effectbuffer.LoopLength = waveEffectData.loopLength;
        Effectbuffer.LoopCount = 1; // We'll just assume we play the loop twice
    }else if( FAILED( hr = pEffectVoice->SubmitSourceBuffer( &Effectbuffer ) ) )
    {
        pEffectVoice->DestroyVoice();
        return hr;
    }
	//hr = pEffectVoice->Start( 0 );
	

	//--------------------------------------------------------------------------------------
	// 엔딩음 적용해보기--------------------------------------------------------------------
	CoInitializeEx( nullptr, COINIT_MULTITHREADED );

	IXAudio2* pXEndingAudio2 = nullptr;	
    UINT32 Endingflags = 0;
    hr = XAudio2Create( &pXEndingAudio2, Endingflags );
    if( FAILED( hr ) )
    {
      //  wprintf( L"Failed to init XAudio2 engine: %#X\n", hr );
        CoUninitialize();
        return 0;
    }
    //
    // Create a mastering voice
    //
    IXAudio2MasteringVoice* pMasteringEndingVoice = nullptr;

    if( FAILED( hr = pXEndingAudio2->CreateMasteringVoice( &pMasteringEndingVoice ) ) )
    {
       // wprintf( L"Failed creating mastering voice: %#X\n", hr );
        SAFE_RELEASE( pXEndingAudio2 );
        CoUninitialize();
        return 0;
    }
	//IXAudio2SourceVoice* pEndingVoice;
	//XAUDIO2_BUFFER Endingbuffer = {0};
	LPCWSTR szEndingFilename=L"Music\\Ending.wav";
	WCHAR strEndingFilePath[MAX_PATH];
    hr = FindMediaFileCch( strEndingFilePath, MAX_PATH, szEndingFilename );
    if( FAILED( hr ) )
    {
       // wprintf( L"Failed to find media file: %s\n", szFilename );
        return hr;
    }

    //
    // Read in the wave file
    //
    std::unique_ptr<uint8_t[]> waveEndingFile;
    DirectX::WAVData waveEndingData;
    if ( FAILED( hr = DirectX::LoadWAVAudioFromFileEx( strEndingFilePath, waveEndingFile, waveEndingData ) ) )
    {
        return hr;
    }

    //
    // Play the wave using a XAudio2SourceVoice
    //

    // Create the source voice	
    if( FAILED( hr = pXEndingAudio2->CreateSourceVoice( &pEndingVoice, waveEndingData.wfx ) ) )
    {
      
        return hr;
    }

    // Submit the wave sample data using an XAUDIO2_BUFFER structure
    Endingbuffer.pAudioData = waveEndingData.startAudio;
    Endingbuffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
    Endingbuffer.AudioBytes = waveEndingData.audioBytes;

    if ( waveEndingData.loopLength > 0 )
    {
        Endingbuffer.LoopBegin = waveEndingData.loopStart;
        Endingbuffer.LoopLength = waveEndingData.loopLength;
        Endingbuffer.LoopCount = 1; // We'll just assume we play the loop twice
    }else if( FAILED( hr = pEndingVoice->SubmitSourceBuffer( &Endingbuffer ) ) )
    {
        pEndingVoice->DestroyVoice();
        return hr;
    }
	//hr = pEndingVoice->Start( 0 );
	BOOL EndingRunning = TRUE;

	//--------------------------------------------------------------------------------------
	//read modeling file
	core.readModel("data/core_edit.txt");
	background.readModel("data/field.txt");
	body[0].readModel("data/body_edit.txt");
	body[80].readModel("data/body2.txt");
	body[100].readModel("data/horn.txt");
	body[150].readModel("data/tail.txt");
	hp.readModel("data/hp.txt");
	enemyhp.readModel("data/enemyhp.txt");
	food[0].readModel("data/food.txt");
	intro.readModel("data/text.txt");
	ending.readModel("data/ending.txt");

	//initialize models
	bind(0, core.RenderModel(), core.RenderIndex(), core.get_vc());
	bind(1, body[0].RenderModel(), body[0].RenderIndex(), body[0].get_vc());
	bind(2, body[100].RenderModel(), body[0].RenderIndex(), body[100].get_vc());
	bind(3, body[150].RenderModel(), body[0].RenderIndex(), body[150].get_vc());
	bind(4, background.RenderModel(), background.RenderIndex(), background.get_vc());
	bind(5, hp.RenderModel(), hp.RenderIndex(), hp.get_vc());
	bind(6, enemyhp.RenderModel(), enemyhp.RenderIndex(), enemyhp.get_vc());
	bind(7, food[0].RenderModel(), food[0].RenderIndex(), food[0].get_vc());
	bind(8, intro.RenderModel(), intro.RenderIndex(), intro.get_vc());
	bind(9, ending.RenderModel(), ending.RenderIndex(), ending.get_vc());
	bind(10, body[80].RenderModel(), body[80].RenderIndex(), body[80].get_vc());
	
	/*stageNum = 3;
	startStage(3);*/
	//--------------------------------------------------------------------------------------

    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
		if(stageNum!=5){
			XAUDIO2_VOICE_STATE state;
			pSourceVoice->GetState( &state );
			isRunning = ( state.BuffersQueued > 0 ) != 0;
			if(isRunning==false)
				pSourceVoice->SubmitSourceBuffer( &buffer );
		}else {

			XAUDIO2_VOICE_STATE state;
			pEndingVoice->GetState( &state );
			EndingRunning = ( state.BuffersQueued > 0 ) != 0;
			if(isRunning==false)
				pEndingVoice->SubmitSourceBuffer( &Endingbuffer );
		}
        if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {		
            Render();
        }
    }

    CleanupDevice();

    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;
	int upBodyIndex = -1;

	string s;

    switch( message )
    {
    case WM_PAINT:
        hdc = BeginPaint( hWnd, &ps );
        EndPaint( hWnd, &ps );
        break;

    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
		
	case WM_KEYUP:
		switch (wParam) {
		case VK_LEFT:
			keyList[0] = false;
			break;
		case VK_RIGHT:
			keyList[1] = false;
			break;
		case VK_UP:
			keyList[2] = false;
			break;
		case VK_DOWN:
			keyList[3] = false;
			break;

			// Q
		case 0x51:
			keyList[4] = false;
			break;
			// W
		case 0x57:
			keyList[5] = false;
			break;
			// A
		case 0x41:
			keyList[7] = false;
			break;
			// S
		case 0x53:
			keyList[9] = false;
			break;
			// D
		case 0x44:
			keyList[8] = false;
			break;
			// X
		case 0x58:
			keyList[6] = false;
			break;

			// C
		case 0x43:
			keyList[10] = false;
			break;
			// V
		case 0x56:
			keyList[11] = false;
			break;
		}
		break;

	case WM_KEYDOWN:
		switch (wParam) {
			case VK_F1:
         for (int i = 0; i < 10; i++) {
            enemy[i].set_Delete();
            enemyLocation[i].Exist = false;
            enemy[i].detachBody(1);
            enemy[i].detachBody(2);
            enemy[i].detachBody(3);
            enemy[i].detachBody(4);
            enemy[i].detachBody(5);
         }

         break;
		case VK_LEFT:
			keyList[0] = true;
			break;
		case VK_RIGHT:
			keyList[1] = true;
			break;
		case VK_UP:
			keyList[2] = true;
			break;
		case VK_DOWN:
			keyList[3] = true;
			break;

			// Q
		case 0x51:
			keyList[4] = true;
			break;
			// W
		case 0x57:
			keyList[5] = true;
			break;
			// A
		case 0x41:
			keyList[7] = true;
			break;
			// S
		case 0x53:
			keyList[9] = true;
			break;
			// D
		case 0x44:
			keyList[8] = true;
			break;
			// X
		case 0x58:
			keyList[6] = true;
			break;

			// C
		case 0x43:
			keyList[10] = true;
			break;
			// V
		case 0x56:
			keyList[11] = true;
			break;

		case VK_SPACE:
			
			core.attack();
			break;
		}
		break;
	case WM_MOUSEMOVE:
		if (stageNum == 0) {
			mouse_X = LOWORD(lParam);
			mouse_Y = HIWORD(lParam);

			if ( 494 < mouse_X && mouse_X < 626 && 258 < mouse_Y && mouse_Y < 405) {
				startBtn = true;
			}
			else {
				startBtn = false;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		/*mouse_X = LOWORD(lParam);
		mouse_Y = HIWORD(lParam);
		s = to_string(mouse_X) + " " + to_string(mouse_Y);
		MessageBoxA(nullptr, s.c_str(), "" , MB_OK);*/

		if (stageNum == 0 && startBtn) {
			pEffectVoice->Start( 0 );
			stageNum++;
			//stageNum=5;
			startStage(stageNum);
			break;
		}

		mouse_X = LOWORD(lParam);
		mouse_Y = HIWORD(lParam);
		pickBodyIndex = PickBody();
		break;

	case WM_LBUTTONUP:
		mouse_X = LOWORD(lParam);
		mouse_Y = HIWORD(lParam);
		upBodyIndex = PickBody();

		if (pickBodyIndex > 0) {
			for (int i = 0; i < 6; i++) {
				if (keyList[i + 4]) {
					if (pickBodyIndex < 200) {
						if (upBodyIndex < 200 && upBodyIndex > 0) {
							if (body[pickBodyIndex].get_isConnect()) break;

							body[upBodyIndex].insertBody(i, pickBodyIndex);
							core.setLeaves();
							break;
						}
						else if (upBodyIndex > 200) {
							if (body[pickBodyIndex].get_isConnect()) break;

							core.insertBody(i, pickBodyIndex);
							break;
						}
						else {
							core.detachBodyBody(pickBodyIndex, i);
							core.setLeaves();
							break;
						}
					}
					else {
						core.detachBody(i);
						core.setLeaves();
						break;
					}
				}
			}
		}
		break;

    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

int PickBody() {
	XMVECTOR Eye = XMVectorSet(core.get_x(), core.get_y() + 20.0f, core.get_z() - 30.0f, 0.0f);
	CollisionRay Ray;
	float n_x = 2 * mouse_X / 800 - 1;
	float n_y = 1 - 2 * mouse_Y / 600;
	XMVECTOR pick = XMVectorSet(n_x, n_y, 0.0f, 1.0f);
	XMMATRIX inverse_viewProj = XMMatrixInverse(NULL, g_View * g_Projection);
	Ray.origin = XMVector3TransformCoord(pick, inverse_viewProj);
	Ray.direction = XMVector3Normalize(Ray.origin - Eye);
	float minDistance, fDist;
	int resultIndex = -1;

	minDistance = 10000.0f;
	if (core.get_collisionBox()->obox.Intersects(Ray.origin, Ray.direction, fDist))
	{
		minDistance = fDist;
		resultIndex = 10000;
	}

	for (int i = 1; i < 200; i++) {
		if (body[i].get_isExist()) {
			if (body[i].get_collisionBox()->obox.Intersects(Ray.origin, Ray.direction, fDist))
			{
				if (minDistance > fDist) {
					minDistance = fDist;
					resultIndex = i;
				}
			}
		}
	}
	return resultIndex;
}

//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render() {
	int enemyCount = 0;

	if (stageNum == 0) {
		// Render Initialize
		//--------------------------------------------------------------------------------------
		// Initialize the light matrix
		XMFLOAT3 vLightPoss;
		XMFLOAT4 vLightDirs = XMFLOAT4(0.0f, 0.7f, -0.5f, 1.0f);
		XMFLOAT4 vLightColors = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// Initialize the view matrix
		XMVECTOR Eye = XMVectorSet(0, 30.0f, -40.0f, 0.0f);
		XMVECTOR At = XMVectorSet(0, 10, 0, 0.0f);
		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		g_View = XMMatrixLookAtLH(Eye, At, Up);

		// Initialize the constant buffer
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		ConstantBuffer cb1;
		cb1.mView = XMMatrixTranspose(g_View);
		cb1.mProjection = XMMatrixTranspose(g_Projection);
		cb1.vLightDir = vLightDirs;
		cb1.vLightColor = vLightColors;
		cb1.vOutputColor = XMFLOAT4(0, 0, 0, 0);

		// Initialize the target view
		g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::CornflowerBlue);
		g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
		g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
		g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//--------------------------------------------------------------------------------------


		//background Rendering
		//-------------------------------------------------------------------------------------
		g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV ); //픽셀 쉐이더에 텍스쳐 set
		g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear ); //sampler set
		g_World = XMMatrixTranslation(background.get_x(), background.get_y() - 5, background.get_z());
		vLightPoss = XMFLOAT3(background.get_x(), background.get_y() + 1, background.get_z() - 1);

		cb1.mWorld = XMMatrixTranspose(g_World);
		cb1.vLightPos = vLightPoss;

		g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[4], 0, nullptr, &cb1, 0, 0);
		g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[4]);
		g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[4]);
		g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[4], &stride, &offset);
		g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[4], DXGI_FORMAT_R16_UINT, 0);
		g_pImmediateContext->DrawIndexed(background.get_vc(), 0, 0);
		//--------------------------------------------------------------------------------------
		g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV2 ); //픽셀 쉐이더에 텍스쳐 set
		g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear ); //sampler set

		//Intro text Rendering
		//-------------------------------------------------------------------------------------
		g_World = XMMatrixTranslation(intro.get_x(), intro.get_y() , intro.get_z());
		vLightPoss = XMFLOAT3(intro.get_x(), intro.get_y() + 1, intro.get_z() - 3);

		cb1.mWorld = XMMatrixTranspose(g_World);
		cb1.vLightPos = vLightPoss;

		g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[8], 0, nullptr, &cb1, 0, 0);
		g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[8]);
		g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[8]);
		g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[8], &stride, &offset);
		g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[8], DXGI_FORMAT_R16_UINT, 0);
		g_pImmediateContext->DrawIndexed(intro.get_vc(), 0, 0);
		//--------------------------------------------------------------------------------------

		//body Rendering
		//--------------------------------------------------------------------------------------	
		if (startBtn)
		{
			body[1].setRotate(body[1].get_rotate() + 0.003f);
		}

		g_World = XMMatrixScaling(4.0f, 4.0f, 4.0f)*XMMatrixRotationY(body[1].get_rotate())*XMMatrixTranslation(11, 4, 6);
		vLightPoss = XMFLOAT3(11, 4 + 2, 6 - 3);

		cb1.mWorld = XMMatrixTranspose(g_World);
		cb1.vLightPos = vLightPoss;

		g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[1], 0, nullptr, &cb1, 0, 0);
		g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[1]);
		g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[1]);
		g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[1], &stride, &offset);
		g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[1], DXGI_FORMAT_R16_UINT, 0);
		g_pImmediateContext->DrawIndexed(body[0].get_vc(), 0, 0);
			
		//--------------------------------------------------------------------------------------
	}
	else if (stageNum == 5) {
		// Render Initialize
		//--------------------------------------------------------------------------------------
		// Initialize the light matrix
		XMFLOAT3 vLightPoss;
		XMFLOAT4 vLightDirs = XMFLOAT4(0.0f, 0.7f, -0.5f, 1.0f);
		XMFLOAT4 vLightColors = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// Initialize the view matrix
	/*	XMVECTOR Eye = XMVectorSet(core.get_x(), core.get_y() + 20.0f, core.get_z() - 30.0f, 0.0f);
		XMVECTOR At = XMVectorSet(core.get_x(), core.get_y(), core.get_z(), 0.0f);*/
		XMVECTOR Eye = XMVectorSet(50, 10.0f, 0.0f, 0.0f);
		XMVECTOR At = XMVectorSet(0, 10, 0, 0.0f);
		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		g_View = XMMatrixLookAtLH(Eye, At, Up);

		// Initialize the constant buffer
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		ConstantBuffer cb1;
		cb1.mView = XMMatrixTranspose(g_View);
		cb1.mProjection = XMMatrixTranspose(g_Projection);
		cb1.vLightDir = vLightDirs;
		cb1.vLightColor = vLightColors;
		cb1.vOutputColor = XMFLOAT4(0, 0, 0, 0);

		// Initialize the target view
		g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::CornflowerBlue);
		g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
		g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
		g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//--------------------------------------------------------------------------------------


		//key Loop
		//--------------------------------------------------------------------------------------
		if (keyList[0]) {
			core.turn_left();
		}
		if (keyList[1]) {
			core.turn_right();
		}
		if (keyList[2]) {
			core.go_forward();
		}
		if (keyList[3]) {
			core.go_backward();
		}
		//--------------------------------------------------------------------------------------


		//ending background Rendering
		//-------------------------------------------------------------------------------------
		g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV ); //픽셀 쉐이더에 텍스쳐 set
		g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear ); //sampler set
		g_World = XMMatrixRotationY(XM_PI)*XMMatrixTranslation(ending.get_x(), ending.get_y() - 3, ending.get_z());
		vLightPoss = XMFLOAT3(ending.get_x(), ending.get_y() + 1, ending.get_z() - 1);

		cb1.mWorld = XMMatrixTranspose(g_World);
		cb1.vLightPos = vLightPoss;

		g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[9], 0, nullptr, &cb1, 0, 0);
		g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[9]);
		g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[9]);
		g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[9], &stride, &offset);
		g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[9], DXGI_FORMAT_R16_UINT, 0);
		g_pImmediateContext->DrawIndexed(ending.get_vc(), 0, 0);
		//--------------------------------------------------------------------------------------
		g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV2 ); //픽셀 쉐이더에 텍스쳐 set
		g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear ); //sampler set
		//core Rendering
		//-------------------------------------------------------------------------------------
		g_World = XMMatrixRotationY(core.get_rotate())*XMMatrixTranslation(core.get_x(), core.get_y(), core.get_z());
		vLightPoss = XMFLOAT3(core.get_x(), core.get_y() + 1, core.get_z() - 1);

		cb1.mWorld = XMMatrixTranspose(g_World);
		cb1.vLightPos = vLightPoss;

		g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[0], 0, nullptr, &cb1, 0, 0);
		g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[0]);
		g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[0]);
		g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[0], &stride, &offset);
		g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[0], DXGI_FORMAT_R16_UINT, 0);
		g_pImmediateContext->DrawIndexed(core.get_vc(), 0, 0);
		core.interact_core_food();
		//--------------------------------------------------------------------------------------

		//body Rendering
		//--------------------------------------------------------------------------------------
		for (int i = 0; i < 100; i++) {
			if (body[i].get_isExist()) {
				g_World = XMMatrixRotationX(body[i].get_rotateX())*XMMatrixRotationY(body[i].get_rotate())*XMMatrixTranslation(body[i].get_x(), body[i].get_y(), body[i].get_z());
				vLightPoss = XMFLOAT3(body[i].get_x(), body[i].get_y() + 1, body[i].get_z() - 1);

				cb1.mWorld = XMMatrixTranspose(g_World);
				cb1.vLightPos = vLightPoss;

				g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[1], 0, nullptr, &cb1, 0, 0);
				g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[1]);
				g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[1]);
				g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[1], &stride, &offset);
				g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[1], DXGI_FORMAT_R16_UINT, 0);
				g_pImmediateContext->DrawIndexed(body[0].get_vc(), 0, 0);
			}
		}
		//--------------------------------------------------------------------------------------

		//weapon Rendering
		//--------------------------------------------------------------------------------------
		for (int i = 100; i < 150; i++) {
			if (body[i].get_isExist()) {
				if (body[i].isFire()) {
					body[i].moveWeaponValue();

					g_World = XMMatrixRotationX(body[i].get_rotateX())*XMMatrixRotationY(body[i].get_rotate())
						*XMMatrixTranslation(body[i].get_x() + body[i].getPartx(), body[i].get_y() + body[i].getParty(), body[i].get_z() + body[i].getPartz());
					vLightPoss = XMFLOAT3(body[i].get_x() + body[i].getPartx(), body[i].get_y() + body[i].getParty() + 1, body[i].get_z() + body[i].getPartz() - 1);

					cb1.mWorld = XMMatrixTranspose(g_World);
					cb1.vLightPos = vLightPoss;

					g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[2], 0, nullptr, &cb1, 0, 0);
					g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[2]);
					g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[2]);
					g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[2], &stride, &offset);
					g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[2], DXGI_FORMAT_R16_UINT, 0);
					g_pImmediateContext->DrawIndexed(body[100].get_vc(), 0, 0);

				}
				else {
					g_World = XMMatrixRotationX(body[i].get_rotateX())*XMMatrixRotationY(body[i].get_rotate())*XMMatrixTranslation(body[i].get_x(), body[i].get_y(), body[i].get_z());
					vLightPoss = XMFLOAT3(body[i].get_x(), body[i].get_y() + 1, body[i].get_z() - 1);

					cb1.mWorld = XMMatrixTranspose(g_World);
					cb1.vLightPos = vLightPoss;

					g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[2], 0, nullptr, &cb1, 0, 0);
					g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[2]);
					g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[2]);
					g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[2], &stride, &offset);
					g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[2], DXGI_FORMAT_R16_UINT, 0);
					g_pImmediateContext->DrawIndexed(body[100].get_vc(), 0, 0);
				}
			}
		}
		//--------------------------------------------------------------------------------------

		//tail Rendering
		//--------------------------------------------------------------------------------------
		for (int i = 150; i < 200; i++) {
			if (body[i].get_isExist()) {
				body[i].MoveTailvalue();

				g_World = XMMatrixRotationX(body[i].get_rotateX())*XMMatrixRotationY(body[i].get_rotate())*XMMatrixTranslation(body[i].get_x(), body[i].get_y(), body[i].get_z());
				vLightPoss = XMFLOAT3(body[i].get_x(), body[i].get_y() + 1, body[i].get_z() - 1);

				cb1.mWorld = XMMatrixTranspose(g_World);
				cb1.vLightPos = vLightPoss;

				g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[3], 0, nullptr, &cb1, 0, 0);
				g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[3]);
				g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[3]);
				g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[3], &stride, &offset);
				g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[3], DXGI_FORMAT_R16_UINT, 0);
				g_pImmediateContext->DrawIndexed(body[150].get_vc(), 0, 0);

				g_World = XMMatrixScaling(0.75f, 0.75f, 0.75f)*XMMatrixRotationX(body[i].get_rotateX())*XMMatrixRotationY(body[i].get_rotate())
					*XMMatrixTranslation(body[i].get_x() - body[i].getPart1x(), body[i].get_y() - body[i].getPart1y(), body[i].get_z() - body[i].getPart1z());
				vLightPoss = XMFLOAT3(body[i].get_x() - body[i].getPart1x(), body[i].get_y() - body[i].getPart1y() + 1, body[i].get_z() - body[i].getPart1z() - 1);

				cb1.mWorld = XMMatrixTranspose(g_World);
				cb1.vLightPos = vLightPoss;

				g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[3], 0, nullptr, &cb1, 0, 0);
				g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[3]);
				g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[3]);
				g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[3], &stride, &offset);
				g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[3], DXGI_FORMAT_R16_UINT, 0);
				g_pImmediateContext->DrawIndexed(body[150].get_vc(), 0, 0);

				g_World = XMMatrixScaling(0.5f, 0.5f, 0.5f)*XMMatrixRotationX(body[i].get_rotateX())*XMMatrixRotationY(body[i].get_rotate())
					*XMMatrixTranslation(body[i].get_x() - body[i].getPart2x(), body[i].get_y() - body[i].getPart2y(), body[i].get_z() - body[i].getPart2z());
				vLightPoss = XMFLOAT3(body[i].get_x() - body[i].getPart2x(), body[i].get_y() - body[i].getPart2y() + 1, body[i].get_z() - body[i].getPart2z() - 1);

				cb1.mWorld = XMMatrixTranspose(g_World);
				cb1.vLightPos = vLightPoss;

				g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[3], 0, nullptr, &cb1, 0, 0);
				g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[3]);
				g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[3]);
				g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[3], &stride, &offset);
				g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[3], DXGI_FORMAT_R16_UINT, 0);
				g_pImmediateContext->DrawIndexed(body[150].get_vc(), 0, 0);
			}
		}
		//--------------------------------------------------------------------------------------

		if (core.get_x() < -20) {
			if (core.get_y() < 8.0f) {
				core.set_y(core.get_y() + 0.003f);
				core.setLeaves();
			}
		}
		if (core.get_x() < -40) {
			static int cnt = 0;
			if (cnt > 3000) {
				pEndingVoice->Stop(); //엔딩곡 중지
				pSourceVoice->Start(0);
				stageNum = 0;
				cnt = 0;
			}
			cnt++;
		}
		
	}
	else {
			if(isEffectRunning==TRUE){
				XAUDIO2_VOICE_STATE state;
				pEffectVoice->GetState( &state );
				isEffectRunning = ( state.BuffersQueued > 0 ) != 0;
				if(isEffectRunning==FALSE){
				pEffectVoice->SubmitSourceBuffer( &Effectbuffer );
				}
			}//효과음 이펙트 적용
	// Render Initialize
	//--------------------------------------------------------------------------------------
	// Initialize the light matrix
	XMFLOAT3 vLightPoss;
	XMFLOAT4 vLightDirs = XMFLOAT4(0.0f, 0.7f, -0.5f, 1.0f);
	XMFLOAT4 vLightColors = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(core.get_x(), core.get_y() + 20.0f, core.get_z() - 30.0f, 0.0f);
	XMVECTOR At = XMVectorSet(core.get_x(), core.get_y(), core.get_z(), 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	if (!core.get_isExist()) {
		static int cnt = 0;
		vLightColors = XMFLOAT4(1.0f, 0.5f, 0.5f, 0.5f);
		if (cnt > 3000) {
			stageNum = 0;
			cnt = 0;
		}
		cnt++;
	}

	
	// Initialize the constant buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	ConstantBuffer cb1;
	cb1.mView = XMMatrixTranspose(g_View);
	cb1.mProjection = XMMatrixTranspose(g_Projection);
	cb1.vLightDir = vLightDirs;
	cb1.vLightColor = vLightColors;
	cb1.vOutputColor = XMFLOAT4(0, 0, 0, 0);

	// Initialize the target view
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::CornflowerBlue);
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//--------------------------------------------------------------------------------------

	
	//key Loop
	//--------------------------------------------------------------------------------------
	if (keyList[0]) {
		core.turn_left();
	}
	if (keyList[1]) {
		core.turn_right();
	}
	if (keyList[2]) {
		core.go_forward();
	}
	if (keyList[3]) {
		core.go_backward();
	}
	if (keyList[10]) {
		core.set_y(core.get_y() - 0.006f);
		core.setLeaves();
	}
	else if (core.get_y() < 0) {
		core.set_y(core.get_y() + 0.003f);
		core.setLeaves();
	}
	if (keyList[11]) {
		core.set_y(core.get_y() + 0.006f);
		core.setLeaves();
	}
	else if (core.get_y() > 0) {
		core.set_y(core.get_y() - 0.003f);
		core.setLeaves();
	}
	//--------------------------------------------------------------------------------------


	//food Rendering
	//-------------------------------------------------------------------------------------
	for (int i = 1; i < 50; i++) {
		if (food[i].get_isExist())
		{
			g_World = XMMatrixTranslation(food[i].get_x(), food[i].get_y(), food[i].get_z());
			vLightPoss = XMFLOAT3(food[i].get_x(), food[i].get_y() + 1, food[i].get_z() - 1);
			foodLocation[i] = identify(food[i].get_x(), food[i].get_z(), food[i].get_isExist(), 0);

			cb1.mWorld = XMMatrixTranspose(g_World);
			cb1.vLightPos = vLightPoss;

			g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[7], 0, nullptr, &cb1, 0, 0);
			g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[7]);
			g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[7]);
			g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[7], &stride, &offset);
			g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[7], DXGI_FORMAT_R16_UINT, 0);
			g_pImmediateContext->DrawIndexed(food[0].get_vc(), 0, 0);
		}
		else
			foodLocation[i] = identify(food[i].get_x(), food[i].get_z(), false, 0);

	}


	//background Rendering
	//-------------------------------------------------------------------------------------
	g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV ); //픽셀 쉐이더에 텍스쳐 set
		g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear ); //sampler set
	g_World = XMMatrixRotationY(background.get_rotate())*XMMatrixTranslation(background.get_x(), background.get_y() - 15, background.get_z());
	vLightPoss = XMFLOAT3(background.get_x(), background.get_y() + 1 + stageNum - 12, background.get_z() - 1);

	cb1.mWorld = XMMatrixTranspose(g_World);
	cb1.vLightPos = vLightPoss;

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[4], 0, nullptr, &cb1, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[4]);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[4]);
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[4], &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[4], DXGI_FORMAT_R16_UINT, 0);
	g_pImmediateContext->DrawIndexed(background.get_vc(), 0, 0);
	//--------------------------------------------------------------------------------------
	g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV2 ); //픽셀 쉐이더에 텍스쳐 set
		g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear ); //sampler set

	//core Rendering
	//-------------------------------------------------------------------------------------
	g_World = XMMatrixRotationY(core.get_rotate())*XMMatrixTranslation(core.get_x(), core.get_y(), core.get_z());
	vLightPoss = XMFLOAT3(core.get_x(), core.get_y() + 1, core.get_z() - 1);

	cb1.mWorld = XMMatrixTranspose(g_World);
	cb1.vLightPos = vLightPoss;

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[0], 0, nullptr, &cb1, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[0]);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[0]);
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[0], &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[0], DXGI_FORMAT_R16_UINT, 0);
	g_pImmediateContext->DrawIndexed(core.get_vc(), 0, 0);
	core.interact_core_food();	//동기화 과정. 6.20 윤승희 추가

	enemyLocation[enemyLocation.size() - 1] = identify(core.get_x(), core.get_z(), core.get_isExist(), core.get_HP()); //contain player location
	core.set_HP(enemyLocation[10].hp);
	core.modify_isExist(enemyLocation[10].Exist);
	//--------------------------------------------------------------------------------------

	//core hp interface
	//--------------------------------------------------------------------------------------
	for (int i = 0; i < core.get_HP(); i++) {
		g_World = XMMatrixTranslation(core.get_x() - 0.35f + i*0.075f, core.get_y() + 15.8f, core.get_z() - 23.0f);
		vLightPoss = XMFLOAT3(core.get_x() - 0.35f + i*0.075f, core.get_y() + 15.8f + 1, core.get_z() - 23.0f - 1);

		cb1.mWorld = XMMatrixTranspose(g_World);
		cb1.vLightPos = vLightPoss;

		g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[5], 0, nullptr, &cb1, 0, 0);
		g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[5]);
		g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[5]);
		g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[5], &stride, &offset);
		g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[5], DXGI_FORMAT_R16_UINT, 0);
		g_pImmediateContext->DrawIndexed(hp.get_vc(), 0, 0);
	}
	//--------------------------------------------------------------------------------------



	//body Rendering
	//--------------------------------------------------------------------------------------
	for (int i = 0; i < 80; i++) {
		if (body[i].get_isExist()) {
			g_World = XMMatrixRotationX(body[i].get_rotateX())*XMMatrixRotationY(body[i].get_rotate())*XMMatrixTranslation(body[i].get_x(), body[i].get_y(), body[i].get_z());
			vLightPoss = XMFLOAT3(body[i].get_x(), body[i].get_y() + 1, body[i].get_z() - 1);

			cb1.mWorld = XMMatrixTranspose(g_World);
			cb1.vLightPos = vLightPoss;

			g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[1], 0, nullptr, &cb1, 0, 0);
			g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[1]);
			g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[1]);
			g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[1], &stride, &offset);
			g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[1], DXGI_FORMAT_R16_UINT, 0);
			g_pImmediateContext->DrawIndexed(body[0].get_vc(), 0, 0);
		}
	}
	for (int i = 80; i < 100; i++) {
		if (body[i].get_isExist()) {
			g_World = XMMatrixRotationX(body[i].get_rotateX())*XMMatrixRotationY(body[i].get_rotate())*XMMatrixTranslation(body[i].get_x(), body[i].get_y(), body[i].get_z());
			vLightPoss = XMFLOAT3(body[i].get_x(), body[i].get_y() + 1, body[i].get_z() - 1);

			cb1.mWorld = XMMatrixTranspose(g_World);
			cb1.vLightPos = vLightPoss;

			g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[10], 0, nullptr, &cb1, 0, 0);
			g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[10]);
			g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[10]);
			g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[10], &stride, &offset);
			g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[10], DXGI_FORMAT_R16_UINT, 0);
			g_pImmediateContext->DrawIndexed(body[80].get_vc(), 0, 0);
		}
	}
	//--------------------------------------------------------------------------------------

	//weapon Rendering
	//--------------------------------------------------------------------------------------
	for (int i = 100; i < 150; i++) {
		if (body[i].get_isExist()) {
			if (body[i].isFire()) {
				body[i].moveWeaponValue();

				g_World = XMMatrixRotationX(body[i].get_rotateX())*XMMatrixRotationY(body[i].get_rotate())
					*XMMatrixTranslation(body[i].get_x() + body[i].getPartx(), body[i].get_y() + body[i].getParty(), body[i].get_z() + body[i].getPartz());
				vLightPoss = XMFLOAT3(body[i].get_x() + body[i].getPartx(), body[i].get_y() + body[i].getParty() + 1, body[i].get_z() + body[i].getPartz() - 1);

				cb1.mWorld = XMMatrixTranspose(g_World);
				cb1.vLightPos = vLightPoss;

				g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[2], 0, nullptr, &cb1, 0, 0);
				g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[2]);
				g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[2]);
				g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[2], &stride, &offset);
				g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[2], DXGI_FORMAT_R16_UINT, 0);
				g_pImmediateContext->DrawIndexed(body[100].get_vc(), 0, 0);

			}
			else {
				g_World = XMMatrixRotationX(body[i].get_rotateX())*XMMatrixRotationY(body[i].get_rotate())*XMMatrixTranslation(body[i].get_x(), body[i].get_y(), body[i].get_z());
				vLightPoss = XMFLOAT3(body[i].get_x(), body[i].get_y() + 1, body[i].get_z() - 1);

				cb1.mWorld = XMMatrixTranspose(g_World);
				cb1.vLightPos = vLightPoss;

				g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[2], 0, nullptr, &cb1, 0, 0);
				g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[2]);
				g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[2]);
				g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[2], &stride, &offset);
				g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[2], DXGI_FORMAT_R16_UINT, 0);
				g_pImmediateContext->DrawIndexed(body[100].get_vc(), 0, 0);
			}
		}
	}
	//--------------------------------------------------------------------------------------

	//tail Rendering
	//--------------------------------------------------------------------------------------
	for (int i = 150; i < 200; i++) {
		if (body[i].get_isExist()) {
			body[i].MoveTailvalue();

			g_World = XMMatrixRotationX(body[i].get_rotateX())*XMMatrixRotationY(body[i].get_rotate())*XMMatrixTranslation(body[i].get_x(), body[i].get_y(), body[i].get_z());
			vLightPoss = XMFLOAT3(body[i].get_x(), body[i].get_y() + 1, body[i].get_z() - 1);

			cb1.mWorld = XMMatrixTranspose(g_World);
			cb1.vLightPos = vLightPoss;

			g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[3], 0, nullptr, &cb1, 0, 0);
			g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[3]);
			g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[3]);
			g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[3], &stride, &offset);
			g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[3], DXGI_FORMAT_R16_UINT, 0);
			g_pImmediateContext->DrawIndexed(body[150].get_vc(), 0, 0);

			g_World = XMMatrixScaling(0.75f, 0.75f, 0.75f)*XMMatrixRotationX(body[i].get_rotateX())*XMMatrixRotationY(body[i].get_rotate())
				*XMMatrixTranslation(body[i].get_x() - body[i].getPart1x(), body[i].get_y() - body[i].getPart1y(), body[i].get_z() - body[i].getPart1z());
			vLightPoss = XMFLOAT3(body[i].get_x() - body[i].getPart1x(), body[i].get_y() - body[i].getPart1y() + 1, body[i].get_z() - body[i].getPart1z() - 1);

			cb1.mWorld = XMMatrixTranspose(g_World);
			cb1.vLightPos = vLightPoss;

			g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[3], 0, nullptr, &cb1, 0, 0);
			g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[3]);
			g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[3]);
			g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[3], &stride, &offset);
			g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[3], DXGI_FORMAT_R16_UINT, 0);
			g_pImmediateContext->DrawIndexed(body[150].get_vc(), 0, 0);

			g_World = XMMatrixScaling(0.5f, 0.5f, 0.5f)*XMMatrixRotationX(body[i].get_rotateX())*XMMatrixRotationY(body[i].get_rotate())
				*XMMatrixTranslation(body[i].get_x() - body[i].getPart2x(), body[i].get_y() - body[i].getPart2y(), body[i].get_z() - body[i].getPart2z());
			vLightPoss = XMFLOAT3(body[i].get_x() - body[i].getPart2x(), body[i].get_y() - body[i].getPart2y() + 1, body[i].get_z() - body[i].getPart2z() - 1);

			cb1.mWorld = XMMatrixTranspose(g_World);
			cb1.vLightPos = vLightPoss;

			g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[3], 0, nullptr, &cb1, 0, 0);
			g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[3]);
			g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[3]);
			g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[3], &stride, &offset);
			g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[3], DXGI_FORMAT_R16_UINT, 0);
			g_pImmediateContext->DrawIndexed(body[150].get_vc(), 0, 0);
		}
	}
	//--------------------------------------------------------------------------------------

	//enemy AI_ (each members are gonna decide what to do)
	static int AIcnt = 0;
	if (enemy[AIcnt].get_isExist())
		enemy[AIcnt].AI_decideAction();
	AIcnt++;
	if (AIcnt > 9) {
		AIcnt = 0;
	}

	//enemyLocation[10].hp = core.get_HP();
	core.set_HP(enemyLocation[10].hp);
	core.modify_isExist(enemyLocation[10].Exist);


	//enemy Rendering
	//--------------------------------------------------------------------------------------
	for (int i = 0; i < 10; i++) {
		static int cnt[10];
		if (enemy[i].immortal)
		{
			enemy[i].set_HP(10);
			//enemy[i].get_HP() - enemyLocation[i].biteDamage
			//enemyLocation[i].biteDamage = 0;
		}
		else
			enemy[i].set_HP(enemyLocation[i].hp); //enemyLocation[i].hp

		enemy[i].modify_isExist(enemyLocation[i].Exist); //동기화 //enemyLocation[i].Exist

		if (enemy[i].get_isExist()) {
			enemyCount++;
			cnt[i]++;
			if (enemy[i].Action == atk)
			{	
				if (cnt[i] > 500) {
					attack(i);
					cnt[i] = 0;
				}	
			}
			g_World = XMMatrixRotationY(enemy[i].get_rotate())*XMMatrixTranslation(enemy[i].get_x(), enemy[i].get_y(), enemy[i].get_z());
			enemyLocation[i] = identify(enemy[i].get_x(), enemy[i].get_z(), enemy[i].get_isExist(), enemy[i].get_HP()); //위치 정보 기록
			vLightPoss = XMFLOAT3(enemy[i].get_x(), enemy[i].get_y() + 1, enemy[i].get_z() - 1);

			cb1.mWorld = XMMatrixTranspose(g_World);
			cb1.vLightPos = vLightPoss;

			g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[0], 0, nullptr, &cb1, 0, 0);
			g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[0]);
			g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[0]);
			g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[0], &stride, &offset);
			g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[0], DXGI_FORMAT_R16_UINT, 0);
			g_pImmediateContext->DrawIndexed(core.get_vc(), 0, 0);

			for (int j = 0; j < enemy[i].get_HP(); j++) {
				g_World = XMMatrixTranslation(enemy[i].get_x() - 1.75f + j*0.375f, enemy[i].get_y() + 2.0f, enemy[i].get_z());
				vLightPoss = XMFLOAT3(enemy[i].get_x() - 1.0f + j*0.375f, enemy[i].get_y() + 2.0f + 1, enemy[i].get_z() - 1);
				cb1.mWorld = XMMatrixTranspose(g_World);
				cb1.vLightPos = vLightPoss;

				g_pImmediateContext->UpdateSubresource(g_pConstantBuffer[6], 0, nullptr, &cb1, 0, 0);
				g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[6]);
				g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer[6]);
				g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer[6], &stride, &offset);
				g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer[6], DXGI_FORMAT_R16_UINT, 0);
				g_pImmediateContext->DrawIndexed(enemyhp.get_vc(), 0, 0);
			}
		}
		else
		{
			if (enemyLocation[i].x == 100)
			{
				int leafContain[5];
				for (int j = 1; j < 6; j++)
				{
					leafContain[j] = enemy[i].get_Index(j);
				}
				enemy[i].detachBody(1);
				enemy[i].detachBody(2);
				enemy[i].detachBody(3);
				enemy[i].detachBody(4);
				enemy[i].detachBody(5);
				for (int j = 1; j < 6; j++)
				{
					if (leafContain[j] != 0)
					{
						enemy[(int)(enemyLocation[i].z)].insertAnyWhere(leafContain[j]);
						enemy[(int)(enemyLocation[i].z)].setLeaves();
					}

				}
				continue;
			}

			enemyLocation[i] = identify(enemy[i].get_x(), enemy[i].get_z(), false, 0); //사망 정보 기록
			enemy[i].detachBody(1);
			enemy[i].detachBody(2);
			enemy[i].detachBody(3);
			enemy[i].detachBody(4);
			enemy[i].detachBody(5);

		}
	}
	//--------------------------------------------------------------------------------------
	if (enemyCount == 0) {
		core.detachBody(1);
		core.detachBody(2);
		core.detachBody(3);
		core.detachBody(4);
		core.detachBody(5);

		static int cnt = 0;
		core.set_y(core.get_y() + 0.02f);
		if (cnt > 3000) {
			stageNum++;
			stageNum %= 6;
			startStage(stageNum);
			cnt = 0;
		}
		cnt++;
	}
}

	//presentation
	g_pSwapChain->Present(0, 0);
}

void initEnemy()
{
	enemyLocation.clear(); //6.20 윤승희 추가. 
	int x, z;
	float t;

	for (int i = 0; i < 10; i++)
	{
		x = rand() % 100;	//0~60
		z = rand() % 100;	//0~60
		t = XM_PI / 180 * (rand() % 360);
		enemy[i].setWorldCoord(x - 50, 0, z - 50);//-50~50
		enemy[i].setRotate(t);
		enemy[i].set_id(i);
		//map[x][z] = 2;
		enemyLocation.push_back(identify(x - 50, z - 50, true, 10));
	}
	enemyLocation.push_back(identify(core.get_x(), core.get_z(), true, 10));
	core.set_id((enemyLocation.size() - 1));
}

void InitbodyInform()
{
	bodyInform.clear();
	for (int i = 0; i < 200; i++)
	{
		bodyInform.push_back(bodyCertification(-1, i, -1));  //int Core, int Own, int Parent
		bodyInform[i].is_Tail = false;
		bodyInform[i].is_weapone = false;
		body[i].set_id(i);	//body 자신의 id를 i로 지정 (인덱스)
		if (i >= 80 && 100 > i)
			body[i].color = 1;
	}
}

void initFood()
{
	foodLocation.clear();
	int x;
	int z;
	for (int i = 0; i < 50; i++)
	{
		x = rand() % 120;	//0~120
		z = rand() % 120;	//0~120
		food[i].setWorldCoord(x - 60, 0, z - 60);//-60~60
		foodLocation.push_back(identify(x - 60, z - 60, true, 0));
	}

}

void attack(int OwnEnemyInd) {
	CollisionRay ray;
	float fDist, minDistance;
	int index;

	for (int i = 1; i < 6; i++) {
		if (enemy[OwnEnemyInd].Leaf[i] == 0) continue;
		enemy[OwnEnemyInd].travelLeaf(enemy[OwnEnemyInd].Leaf[i]);
	}

	float x = enemy[OwnEnemyInd].get_x();
	float z = enemy[OwnEnemyInd].get_z();
	vector<int> weapon = enemy[OwnEnemyInd].weapon;

	for (int i = 0; i < weapon.size(); i++) {
		minDistance = 10;
		index = -1;
		body[weapon[i]].set_range(minDistance - 1);

		// 적공격판정
		for (int n = 0; n < 10; n++) {
			if (enemy[n].get_isExist() && (abs(x - enemy[n].get_x()) + abs(z - enemy[n].get_z()) < 15)) {
				ray = body[weapon[i]].attackRay();
				if (enemy[n].get_collisionBox()->obox.Intersects(ray.origin, ray.direction, fDist)) {
					if (fDist < 10) {
						if (minDistance > fDist) {
							minDistance = fDist;
							body[weapon[i]].set_range(minDistance);
							index = n + 200;
						}
					}
				}
			}
		}

		if ((abs(x - core.get_x()) + abs(z - core.get_z()) < 15)) {
			ray = body[weapon[i]].attackRay();
			if (core.get_collisionBox()->obox.Intersects(ray.origin, ray.direction, fDist)) {
				if (fDist < 10) {
					if (minDistance > fDist) {
						minDistance = fDist;
						body[weapon[i]].set_range(minDistance);
						index = 1000;
					}
				}
			}
		}

		//body공격판정
		for (int n = 1; n < 200; n++) {
			if (body[n].get_isExist() && (abs(x - body[n].get_x()) + abs(z - body[n].get_z()) < 15) && (weapon[i] != n)) {
				ray = body[weapon[i]].attackRay();
				if (body[n].get_collisionBox()->obox.Intersects(ray.origin, ray.direction, fDist)) {
					if (fDist < 10) {
						if (minDistance > fDist) {
							minDistance = fDist;
							body[weapon[i]].set_range(minDistance);
							index = n;
						}
					}
				}
			}
		}
		body[weapon[i]].setFire();
		if (index > 0) {
			if (index < 200) {
				body[index].decreaseHP(2);	//어떤 파트가 제거 된 경우, 그 파트의 주인의 목록에서 제거 해 준다.

			}
			else if (index < 1000) {
				int enemyCoreInd = index - 200;
				enemy[enemyCoreInd].decreaseHP(2);

				enemyLocation[enemyCoreInd].decreseHP(2);  //6.20 윤승희 추가 
				if (enemy[enemyCoreInd].get_HP() <= 0) {
					enemyLocation[enemyCoreInd].Exist = false; //6.20 윤승희 추가 . enemyLocation 사망 정보 동기화

					int leafContain[5];
					for (int i = 1; i < 6; i++)
					{
						leafContain[i] = enemy[enemyCoreInd].get_Index(i);
					}
					enemy[enemyCoreInd].detachBody(1);
					enemy[enemyCoreInd].detachBody(2);
					enemy[enemyCoreInd].detachBody(3);
					enemy[enemyCoreInd].detachBody(4);
					enemy[enemyCoreInd].detachBody(5);
					for (int i = 1; i < 6; i++)
					{
						if (leafContain[i] != 0) {
							enemy[OwnEnemyInd].insertAnyWhere(leafContain[i]);
							enemy[OwnEnemyInd].setLeaves();
						}
					}
				}
			}
			else {
				core.decreaseHP(2);
			}
		}
	}
	enemy[OwnEnemyInd].weapon.clear();
}


