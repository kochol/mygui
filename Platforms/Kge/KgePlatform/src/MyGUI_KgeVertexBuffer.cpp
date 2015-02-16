/*!
@file
@author		Ali Akbar Mohammadi
@date		11/2014
*/

#include "MyGUI_KgeVertexBuffer.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_KgeDiagnostic.h"
#include <gfx/HardwareBuffer.h>
#include <Device.h>
#include <gfx/Renderer.h>

namespace MyGUI
{

	const size_t VERTEX_IN_QUAD = 6;
	const size_t RENDER_ITEM_STEEP_REALLOCK = 5 * VERTEX_IN_QUAD;

	KgeVertexBuffer::KgeVertexBuffer(kge::Device* _device, KgeRenderManager* _pRenderManager) :
		mNeedVertexCount(0),
		mVertexCount(RENDER_ITEM_STEEP_REALLOCK),
		mpKGEDevice(_device),
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
		bool result = mpBuffer->Lock(0, 0, (void**)&lockPtr, 0);
		if (!result)
		{
			MYGUI_PLATFORM_EXCEPT("Failed to lock vertex buffer.");
		}
		return reinterpret_cast<Vertex*>(lockPtr);
	}

	void KgeVertexBuffer::unlock()
	{
		bool result = mpBuffer->Unlock();
		if (!result)
		{
			MYGUI_PLATFORM_EXCEPT("Failed to unlock vertex buffer.");
		}
	}

	bool KgeVertexBuffer::setToStream(size_t stream)
	{
		mpKGEDevice->GetRenderer()->SetVertexBuffer(mpBuffer, stream);
		return true;
	}

	bool KgeVertexBuffer::create()
	{
		DWORD length = mNeedVertexCount * sizeof(MyGUI::Vertex);
		mpBuffer = mpKGEDevice->GetRenderer()->CreateVertexBuffer(NULL, mNeedVertexCount, sizeof(MyGUI::Vertex), true);
		return true;
	}

	void KgeVertexBuffer::destroy()
	{
		if (mpBuffer)
		{
			mpBuffer->DecRef();
			mpBuffer = nullptr;
		}
	}

	void KgeVertexBuffer::resize()
	{
		if (mpKGEDevice)
		{
			destroy();
			create();
		}
	}

} // namespace MyGUI
