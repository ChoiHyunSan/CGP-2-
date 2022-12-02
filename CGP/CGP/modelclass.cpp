////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"
#include "Scene.h"
#include "SceneMgr.h"
#include "Collider.h"


ModelClass::ModelClass()
	:m_faceCount(0),
	m_vertexBuffer(0),
	m_Texture1(0),
	m_textureCount(0),
	m_normalCount(0),
	m_model(0),
	m_vertexCount(0),
	m_instanceBuffer(0)
{
	
}


ModelClass::ModelClass(const ModelClass& other)
	:m_faceCount(0),
	m_vertexBuffer(0),
	m_Texture1(0),
	m_textureCount(0),
	m_normalCount(0),
	m_model(0),
	m_vertexCount(0),
	m_instanceBuffer(0)
{

}

ModelClass::ModelClass(ID3D11Device* device, GROUP_TYPE type)
	:m_faceCount(0),
	m_vertexBuffer(0),
	m_Texture1(0),
	m_textureCount(0),
	m_normalCount(0),
	m_model(0),
	m_vertexCount(0),
	m_instanceBuffer(0),
	m_rotate(XMMatrixRotationY(0.f)),
	m_scale(XMMatrixScaling(1.f,1.f,1.f)),
	m_type(type),
	m_textureFilename1(0),
	m_Pos()
{
	Initialize(device, type);

	CreateCollider();
	GetCollider()->SetOffsetPos(Pos(0.f, 0.f, 0.f));
	GetCollider()->SetScale(Pos(1.0f, 1.0f, 1.0f));
}

ModelClass::ModelClass(ID3D11Device* device, GROUP_TYPE type, Pos pos)
	:m_faceCount(0),
	m_vertexBuffer(0),
	m_Texture1(0),
	m_textureCount(0),
	m_normalCount(0),
	m_model(0),
	m_vertexCount(0),
	m_instanceBuffer(0),
	m_rotate(XMMatrixRotationY(0.f)),
	m_scale(XMMatrixScaling(1.f, 1.f, 1.f)),
	m_type(type),
	m_textureFilename1(0),
	m_Pos(pos)
{
	m_Pos = pos;
	Initialize(device, type);
	SetName(L"Default");

	CreateCollider();
	GetCollider()->SetOffsetPos(Pos(0.f, 0.f, 0.f));
	GetCollider()->SetScale(Pos(1.0f, 1.0f, 1.0f));
}


ModelClass::~ModelClass()
{

}


bool ModelClass::Initialize(ID3D11Device* device, GROUP_TYPE type)
{
	bool result;

	setInstPosInfo();
	setFileInfo();

	// Load in the model data,
	result = LoadModel(m_modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, m_textureFilename1);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	if (m_pCollider != nullptr)
		delete m_pCollider;

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture1->GetTexture();
}


void ModelClass::finalUpdate()
{
	if (m_pCollider)
	{
		m_pCollider->finalUpdate();
	}
}

void ModelClass::CreateCollider()
{
	m_pCollider = new Collider;
	m_pCollider->m_pOwnerModel = this;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
    InstanceType* instances;
	D3D11_BUFFER_DESC vertexBufferDesc, instanceBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, instanceData;
	HRESULT result;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	// 인스턴스 수 설정
	m_instanceCount = 1;

	// 인스턴스 배열 생성
	instances = new InstanceType[m_vInstancePos.size()];
	if (!instances)
	{
		return false;
	}

	// 인스턴스 배열에 데이터를 넣습니다.
	for (int i = 0; i < m_vInstancePos.size(); i++)
	{
		instances[i].position = m_vInstancePos[i];
	}

	// 인스턴스 버퍼의 디스크립션입니다.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_vInstancePos.size();
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// 서브리소스 구조체에 인스턴스 데이터의 포인터를 설정합니다.
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// 인스턴스 버퍼를 생성합니다.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 인스턴스 버퍼가 생성되었으므로 인스턴스 배열의 할당을 해제합니다.
	delete[] instances;
	instances = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	// Set vertex buffer stride and offset.
	strides[0] = sizeof(VertexType); 
	strides[1] = sizeof(InstanceType); 

	offsets[0] = 0;
	offsets[1] = 0;

	// 정점과 인스턴스 버퍼를 가리키는 포인터 배열을 설정합니다.
	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool ModelClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture1 = new TextureClass;
	if(!m_Texture1)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture1->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture1)
	{
		m_Texture1->Shutdown();
		delete m_Texture1;
		m_Texture1 = 0;
	}

	return;
}

bool ModelClass::LoadModel(const WCHAR* filename)
{
	ReadFileCounts(filename);

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

int ModelClass::GetVertexCount()
{
	return m_vertexCount;
}

int ModelClass::GetInstanceCount()
{
	return m_vInstancePos.size();
}

GROUP_TYPE ModelClass::GetType()
{
	return m_ObjType;
}

void ModelClass::correctPos()
{
	for (int i = -MAPSIZE/2; i < MAPSIZE / 2; ++i)
	{
		if (i - 0.5f <= m_Pos.x && m_Pos.x <= i + 0.5f)
			m_Pos.x = i;
	}
	for (int i = 0; i < MAPSIZE; ++i)
	{
		if (i - 0.5f <= m_Pos.z && m_Pos.z <= i + 0.5f)
			m_Pos.z = i;
	}
}

bool ModelClass::ReadFileCounts(const WCHAR* filename)
{
	ifstream fin;
	char input;
	// Initialize the counts.
	int vertexCount = 0;
	int textureCount = 0;
	int normalCount = 0;
	int faceCount = 0;
	// Open the file.
	fin.open(filename);
	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}
	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}
	// Close the file.
	fin.close();

	LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);

	return true;
}

bool ModelClass::LoadDataStructures(const WCHAR* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	XMFLOAT3* vertices, * texcoords, * normals;
	FaceType* faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	// Initialize the four data structures.
	vertices = new XMFLOAT3[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new XMFLOAT3[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new XMFLOAT3[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >>
					vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;
				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >>
					normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >>
					input2 >> faces[faceIndex].nIndex3 >> faces[faceIndex].vIndex2 >> input2 >>
					faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2 >>
					faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >>
					faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	//// Close the file.
	//fin.close();
	//// Open the output file.
	//fout.open("model.txt");
	//// Write out the file header that our model format uses.
	//fout << "Vertex Count: " << (faceCount * 3) << endl;
	//fout << endl;
	//fout << "Data:" << endl;
	//fout << endl;

	m_vertexCount = faceCount * 3;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;
		//fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
		//	<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
		//	<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		m_model[i * 3].x = vertices[vIndex].x;
		m_model[i * 3].y = vertices[vIndex].y;
		m_model[i * 3].z = vertices[vIndex].z;

		m_model[i * 3].tu = texcoords[tIndex].x;
		m_model[i * 3].tv = texcoords[tIndex].y;

		m_model[i * 3].nx = normals[nIndex].x;
		m_model[i * 3].ny = normals[nIndex].y;
		m_model[i * 3].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;
		//fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
		//	<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
		//	<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		m_model[i * 3 + 1].x = vertices[vIndex].x;
		m_model[i * 3 + 1].y = vertices[vIndex].y;
		m_model[i * 3 + 1].z = vertices[vIndex].z;

		m_model[i * 3 + 1].tu = texcoords[tIndex].x;
		m_model[i * 3 + 1].tv = texcoords[tIndex].y;

		m_model[i * 3 + 1].nx = normals[nIndex].x;
		m_model[i * 3 + 1].ny = normals[nIndex].y;
		m_model[i * 3 + 1].nz = normals[nIndex].z;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;
		//fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
		//	<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
		//	<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		m_model[i * 3 + 2].x = vertices[vIndex].x;
		m_model[i * 3 + 2].y = vertices[vIndex].y;
		m_model[i * 3 + 2].z = vertices[vIndex].z;

		m_model[i * 3 + 2].tu = texcoords[tIndex].x;
		m_model[i * 3 + 2].tv = texcoords[tIndex].y;

		m_model[i * 3 + 2].nx = normals[nIndex].x;
		m_model[i * 3 + 2].ny = normals[nIndex].y;
		m_model[i * 3 + 2].nz = normals[nIndex].z;
	}

	//// Close the output file.
	//fout.close();

	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}

	if (normals)
	{
		delete[] normals;
		normals = 0;
	}

	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}

void ModelClass::setInstPosInfo()
{
	switch (m_type)
	{
	case GROUP_TYPE::DEFAULT:
		// 맵 생성을 다른 방식으로 제작해야함 (인스턴싱 사용 안할 계획)
		m_vInstancePos.push_back(XMFLOAT3(0, 0, 0));
		break;

	case GROUP_TYPE::PLAYER:
		m_vInstancePos.push_back(XMFLOAT3(0, 0.5, 0));
		break;

	case GROUP_TYPE::BOMB:
	{
		m_Pos = SceneMgr::GetInst()->GetCurScene()->getPlayerPos();

		// Player Pos에 관한 보정이 필요함
		correctPos();

		m_vInstancePos.push_back(XMFLOAT3(0.f, 0.5f, 0.f));
		break;
	}
	case GROUP_TYPE::ENEMY:
		m_vInstancePos.push_back(XMFLOAT3(0, 0.5f, 0));
		break;
}
	}

void ModelClass::setFileInfo()
{
	switch (m_type)
	{
	case GROUP_TYPE::DEFAULT:
		m_modelFilename = (WCHAR*)L"./data/cube.obj";
		m_textureFilename1 = (WCHAR*)L"./data/Block.dds";
		break;

	case GROUP_TYPE::PLAYER:
		m_modelFilename = (WCHAR*)L"./data/player.obj";
		m_textureFilename1 = (WCHAR*)L"./data/player.dds";
		break;

	case GROUP_TYPE::BOMB:
		m_modelFilename = (WCHAR*)L"./data/bomb.obj";
		m_textureFilename1 = (WCHAR*)L"./data/bomb.dds";
		break;

	case GROUP_TYPE::ENEMY:
		m_modelFilename = (WCHAR*)L"./data/creeper.obj";
		m_textureFilename1 = (WCHAR*)L"./data/creeper.dds";
		break;
	}
	
}

void ModelClass::Update()
{

}

void ModelClass::OnCollision(Collider* _pOther)
{
}

void ModelClass::OnCollisionEnter(Collider* _pOther)
{
}

void ModelClass::OnCollisionExit(Collider* _pOther)
{
}

void ModelClass::setPos(float x, float y, float z)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;
}

void ModelClass::addPos(float x, float y, float z)
{
	m_Pos.x += x;
	m_Pos.y += y;
	m_Pos.z += z;
}

void ModelClass::setRotate(char dir,float rotation)
{
	// 초기 지정된 위치값만큼 원점으로 이동 후 회전 
	m_rotate = XMMatrixTranslation(0,0,0);
	switch (dir)
	{
	case 'x':
		m_rotate *= XMMatrixRotationX(rotation);
		break;
	case 'y':
		m_rotate *= XMMatrixRotationY(rotation);
		break;
	case'z':
		m_rotate *= XMMatrixRotationZ(rotation);
		break;
	}
	m_rotate *= XMMatrixTranslation(0, 0, 0);
}

void ModelClass::setScale(float xScale, float yScale, float zScale)
{
	m_scale = XMMatrixScaling(xScale, yScale, zScale);
}

