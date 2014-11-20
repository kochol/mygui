/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#include <d3dx9.h>
#include "MyGUI_KgeVertexBuffer.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_KgeDiagnostic.h"

namespace MyGUI
{

	const size_t VERTEX_IN_QUAD = 6;
	const size_t RENDER_ITEM_STEEP_REALLOCK = 5 * VERTEX_IN_QUAD;

	KgeVertexBuffer::KgeVertexBuffer(IDirect3DDevice9* _device, KgeRenderManager* _pRenderManager) :
		mNeedVertexCount(0),
		mVertexCount(RENDER_ITEM_STEEP_REALLOCK),
		mpD3DDevice(_device),
		pRenderManager(_pRenderManager),
		mpBuffer(NULL)
	{
	}

	KgeVertexBuffer::~KgeVertexBuffer()
	{
		destroy();
	}

	void KgeVertexBuffer::setVertexCount(size_t _count)
	{
		if (_count != mNeedVertexCount)
		{
			mNeedVertexCount = _count;
			resize();
		}
	}

	size_t KgeVertexBuffer::getVertexCount()
	{
		return mNeedVertexCount;
	}

	Vertex* KgeVertexBuffer::lock()
	{
		void* lockPtr = nullptr;
		HRESULT result = mpBuffer->Lock(0, 0, (void**)&lockPtr, 0);
		if (FAILED(result))
		{
			MYGUI_PLATFORM_EXCEPT("Failed to lock vertex buffer (error code " << result << ").");
		}
		return reinterpret_cast<Vertex*>(lockPtr);
	}

	void KgeVertexBuffer::unlock()
	{
		HRESULT result = mpBuffer->Unlock();
		if (FAILED(result))
		{
			MYGUI_PLATFORM_EXCEPT("Failed to unlock vertex buffer (error code " << result << ").");
		}
	}

	bool KgeVertexBuffer::setToStream(size_t stream)
	{
		if (SUCCEEDED(mpD3DDevice->SetStreamSource(stream, mpBuffer, 0, sizeof(MyGUI::Vertex))))
			return true;
		return false;
	}

	bool KgeVertexBuffer::create()
	{
		DWORD length = mNeedVertexCount * sizeof(MyGUI::Vertex);
		if (SUCCEEDED(mpD3DDevice->CreateVertexBuffer(length, 0, 0, D3DPOOL_MANAGED, &mpBuffer, nullptr)))
			return false;
		return false;
	}

	void KgeVertexBuffer::destroy()
	{
		if (mpBuffer)
		{
			mpBuffer->Release();
			mpBuffer = nullptr;
		}
	}

	void KgeVertexBuffer::resize()
	{
		if (mpD3DDevice)
		{
			destroy();
			create();
		}
	}

} // namespace MyGUI
