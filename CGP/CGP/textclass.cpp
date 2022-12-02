///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "textclass.h"
#include "graphicsclass.h"
#include "d3dclass.h"
#include <string>
#include "GameMgr.h"
#include "systemclass.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "TimeMgr.h"
#include "pch.h"
using namespace std;

TextClass::TextClass():
	m_Font(0),
	m_FontShader(0),
	m_playTime(0),
	m_playerLife(0),
	m_playScore(0),
	m_Cpu(0),
	m_Fps(0),
	m_CameraMode(0),
	m_GameStart(0),
	m_delayCount(0.f),
	m_TitleName(0)
{

}


TextClass::TextClass(const TextClass& other)
{
}


TextClass::~TextClass()
{
}


bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, 
	int screenWidth, int screenHeight, XMMATRIX baseViewMatrix)
{
	bool result;


	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;

	// Create the font object.
	m_Font = new FontClass;
	if(!m_Font)
	{
		return false;
	}

	// Initialize the font object.
	result = m_Font->Initialize(device, L"./data/fontdata.txt", L"./data/font.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_playTime, 16, device);
	if(!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_playerLife, 16, device);
	if(!result)
	{
		return false;
	}

	result = InitializeSentence(&m_playScore, 16, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&m_Cpu, 16, device);
	if (!result)
	{
		return false;
	}	

	result = InitializeSentence(&m_Fps, 16, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&m_CameraMode, 32, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&m_GameStart, 32, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&m_TitleName, 32, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&m_GameResult, 32, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&m_GameReplay, 32, device);
	if (!result)
	{
		return false;
	}

	return true;
}


void TextClass::Shutdown()
{
	// Release the first sentence.
	ReleaseSentence(&m_playTime);

	// Release the second sentence.
	ReleaseSentence(&m_playerLife);

	ReleaseSentence(&m_playScore);

	ReleaseSentence(&m_Cpu);

	ReleaseSentence(&m_Fps);

	ReleaseSentence(&m_CameraMode);

	ReleaseSentence(&m_GameStart);

	ReleaseSentence(&m_TitleName);

	ReleaseSentence(&m_GameResult);

	ReleaseSentence(&m_GameReplay);

	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if(m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}


bool TextClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX orthoMatrix)
{
	bool result;

	if (SceneMgr::GetInst()->GetCurScene()->GetName() != L"Title Scene"
		&& SceneMgr::GetInst()->GetCurScene()->GetName() != L"GameOver_Scene"
		&& SceneMgr::GetInst()->GetCurScene()->GetName() != L"GameClear_Scene")
	{
		// Draw the first sentence.
		result = RenderSentence(deviceContext, m_playTime, worldMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}

		// Draw the second sentence.
		result = RenderSentence(deviceContext, m_playerLife, worldMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}

		result = RenderSentence(deviceContext, m_playScore, worldMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}

		result = RenderSentence(deviceContext, m_CameraMode, worldMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}
	}

	// 타이틀 씬의 경우에는 따로 하나의 텍스트를 만들어 띄운다.
	if (SceneMgr::GetInst()->GetCurScene()->GetName() == L"Title Scene")
	{
		result = RenderSentence(deviceContext, m_TitleName, worldMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}

		if (m_bUIDelayOn)
		{
			result = RenderSentence(deviceContext, m_GameStart, worldMatrix, orthoMatrix);
			if (!result)
			{
				return false;
			}
		}
	}

	// 게임 오버 씬의 경우에는 게임 결과창에 해당하는 UI를 출력한다.
	if (SceneMgr::GetInst()->GetCurScene()->GetName() == L"GameOver_Scene" 
		|| SceneMgr::GetInst()->GetCurScene()->GetName() == L"GameClear_Scene")
	{
		result = RenderSentence(deviceContext, m_GameResult, worldMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}

		if (m_bUIDelayOn)
		{
			result = RenderSentence(deviceContext, m_GameReplay, worldMatrix, orthoMatrix);
			if (!result)
			{
				return false;
			}
		}
	}

	// 기본적인 UI는 무조건 출력을 해준다.
	result = RenderSentence(deviceContext, m_Cpu, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_Fps, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

void TextClass::update()
{
	ID3D11DeviceContext* deviceContext = GraphicsClass::GetInst()->GetD3D()->GetDeviceContext();

	// GameMgr 정보를 const char* 형식으로 변환
	string s_playTime = "Time : " + to_string(GameMgr::GetInst()->getPlayTime());
	string s_playerLife = "Life : " + to_string(GameMgr::GetInst()->getPlayerLife());
	string s_playScore = "Score : " + to_string(GameMgr::GetInst()->getPlayScore());

	string s_Cpu = "Cpu : " + to_string(SystemClass::GetInst()->GetCpu()) + "%";
	string s_Fps = "Fps : " + to_string(SystemClass::GetInst()->GetFps());

	string s_CameraModeFix	= (string)"Camera Mode(F1) : Fix";
	string s_CameraModeMove = (string)"Camera Mode(F1) : Move";

	string s_TitleName = (string)" Bomb Game";
	string s_GameStart = (string)"- Press S Key To Start -";

	string s_GameClear	= "Game Clear";
	string s_GameOver	= "Game Over";
	string s_GameReplay = "Press R to Replay";


	const char* temp_playerTime = s_playTime.c_str();
	const char* temp_playerLife = s_playerLife.c_str();
	const char* temp_playScore = s_playScore.c_str();
	const char* temp_Cpu = s_Cpu.c_str();
	const char* temp_Fps = s_Fps.c_str();

	const char* temp_CameraModeFix = s_CameraModeFix.c_str();
	const char* temp_CameraModeMove = s_CameraModeMove.c_str();

	const char* temp_TitleName = s_TitleName.c_str();
	const char* temp_GameStart = s_GameStart.c_str();

	const char* temp_GameClear = s_GameClear.c_str();
	const char* temp_GameOver = s_GameOver.c_str();
	const char* temp_GameReplay = s_GameReplay.c_str();

	// 스테이지 UI
	UpdateSentence(m_playTime, temp_playerTime, 170, 550, 1.0f, 1.0f, 1.0f, deviceContext);
	UpdateSentence(m_playerLife, temp_playerLife, 70, 550, 1.0f, 1.0f, 1.0f, deviceContext);
	UpdateSentence(m_playScore, temp_playScore, 340, 550, 1.0f, 1.0f, 1.0f, deviceContext);

	// 기본 UI
	UpdateSentence(m_Cpu, temp_Cpu, 50, 50, 1.0f, 1.0f, 1.0f, deviceContext);
	UpdateSentence(m_Fps, temp_Fps, 50, 70, 1.0f, 1.0f, 1.0f, deviceContext);
	UpdateSentence(m_CameraMode, temp_CameraModeFix, 50, 90, 1.0f, 1.0f, 1.0f, deviceContext);

	if (GameMgr::GetInst()->GetCameraMode())
		UpdateSentence(m_CameraMode, temp_CameraModeFix, 50, 90, 1.0f, 1.0f, 1.0f, deviceContext);
	else
		UpdateSentence(m_CameraMode, temp_CameraModeMove, 50, 90, 1.0f, 1.0f, 1.0f, deviceContext);

	// 타이틀 씬 UI
	UpdateSentence(m_TitleName, temp_TitleName, 352, 280, 1.0f, 1.0f, 1.0f, deviceContext);
	UpdateSentence(m_GameStart, temp_GameStart, 297, 320, 1.0f, 0.83f, 0.0f, deviceContext);

	// 게임 결과 씬 UI
	if (SceneMgr::GetInst()->GetCurScene()->GetName() == L"GameOver_Scene")
	{
		UpdateSentence(m_GameResult, temp_GameOver, 352, 280, 1.0f, 1.0f, 1.0f, deviceContext);
	}
	else if(SceneMgr::GetInst()->GetCurScene()->GetName() == L"GameClear_Scene")
	{
		UpdateSentence(m_GameResult, temp_GameClear, 352, 280, 1.0f, 1.0f, 1.0f, deviceContext);
	}
	UpdateSentence(m_GameReplay, temp_GameReplay, 297, 320, 1.0f, 0.83f, 0.0f, deviceContext);

	// UI 깜빡거림 딜레이
	if (m_delayCount >= 0.5f)
	{
		m_bUIDelayOn = false;
	}
	else if (m_delayCount < 0.f)
	{
		m_bUIDelayOn = true;
	}

	if (m_bUIDelayOn)
	{
		m_delayCount += fDT;
	}
	else
	{
		m_delayCount -= fDT;
	}
}


bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create a new sentence object.
	*sentence = new SentenceType;
	if(!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if(!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for(i=0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete [] indices;
	indices = 0;

	return true;
}


bool TextClass::UpdateSentence(SentenceType* sentence, const char* text, int positionX, int positionY, 
	float red, float green, float blue, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}


void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if(*sentence)
	{
		// Release the sentence vertex buffer.
		if((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}


bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, XMMATRIX worldMatrix, 
							   XMMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;


	// Set vertex buffer stride and offset.
    stride = sizeof(VertexType); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);


	worldMatrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f);

	if (sentence == m_GameStart)
	{
		worldMatrix *= XMMatrixScaling(3.0f, 3.0f, 3.0f);
	}
	else if (sentence == m_TitleName)
	{
		worldMatrix *= XMMatrixScaling(7.0f, 7.0f,7.0f);
	}
	else if (sentence == m_GameResult)
	{
		worldMatrix *= XMMatrixScaling(7.0f, 7.0f, 7.0f);
	}
	else if (sentence == m_GameReplay)
	{
		worldMatrix *= XMMatrixScaling(3.0f, 3.0f, 3.0f);
	}

	// Render the text using the font shader.
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, 
		orthoMatrix, m_Font->GetTexture(), pixelColor);
	if(!result)
	{
		false;
	}

	return true;
}