/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef __MYGUI_KGE_VERTEX_BUFFER_H__
#define __MYGUI_KGE_VERTEX_BUFFER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_KgeRenderManager.h"

struct IDirect3DDevice9;
struct IDirect3DVertexBuffer9;

namespace MyGUI
{

	class KgeVertexBuffer : public IVertexBuffer
	{
	public:
		KgeVertexBuffer(IDirect3DDevice9* _device, KgeRenderManager* _pRenderManager);
		virtual ~KgeVertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual size_t getVertexCount();

		virtual Vertex* lock();
		virtual void unlock();

	/*internal:*/
		virtual bool setToStream(size_t stream);

	private:
		bool create();
		void destroy();
		void resize();

	private:
		IDirect3DDevice9* mpD3DDevice;
		IDirect3DVertexBuffer9* mpBuffer;
		KgeRenderManager* pRenderManager;

		size_t mVertexCount;
		size_t mNeedVertexCount;
	};

} // namespace MyGUI

#endif // __MYGUI_KGE_VERTEX_BUFFER_H__
