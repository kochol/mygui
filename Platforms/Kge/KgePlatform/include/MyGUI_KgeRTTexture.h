/*!
@file
@author		Ali Akbar Mohammadi
@date		11/2014
*/

#ifndef __MYGUI_KGE_RTTEXTURE_H__
#define __MYGUI_KGE_RTTEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

namespace kge
{
	class Device;
	namespace gfx
	{
		class Texture;

	} // gfx

} // kge

namespace MyGUI
{

	class KgeRTTexture :
		public IRenderTarget
	{
	public:
		KgeRTTexture(kge::Device* _device, kge::gfx::Texture* _texture);
		virtual ~KgeRTTexture();

		virtual void begin();
		virtual void end();

		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

		virtual const RenderTargetInfo& getInfo()
		{
			return mRenderTargetInfo;
		}

	private:
		kge::Device* mpKGEDevice;
		kge::gfx::Texture* mpTexture;		
		RenderTargetInfo mRenderTargetInfo;
	};

} // namespace MyGUI

#endif // __MYGUI_KGE_RTTEXTURE_H__
