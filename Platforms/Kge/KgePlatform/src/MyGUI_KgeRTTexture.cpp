/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#include "MyGUI_KgeRTTexture.h"
#include "MyGUI_KgeRenderManager.h"
#include <Device.h>
#include <gfx/Texture.h>
#include <gfx/Renderer.h>

namespace MyGUI
{

	KgeRTTexture::KgeRTTexture(kge::Device* _device, kge::gfx::Texture* _texture) :
		mpKGEDevice(_device),
		mpTexture(_texture)
	{
		int width = mpTexture->GetWidth();
		int height = mpTexture->GetHeight();

		mRenderTargetInfo.maximumDepth = 0.0f;
		mRenderTargetInfo.hOffset = -0.5f / float(width);
		mRenderTargetInfo.vOffset = -0.5f / float(height);
		mRenderTargetInfo.aspectCoef = float(height) / float(width);
		mRenderTargetInfo.pixScaleX = 1.0f / float(width);
		mRenderTargetInfo.pixScaleY = 1.0f / float(height);
	}

	KgeRTTexture::~KgeRTTexture()
	{
	}

	void KgeRTTexture::begin()
	{
		mpKGEDevice->GetRenderer()->SetRenderTarget(0, mpTexture);
		mpKGEDevice->GetRenderer()->BeginRendering(true, true, true);
	}

	void KgeRTTexture::end()
	{
		mpKGEDevice->GetRenderer()->EndRendering();
		mpKGEDevice->GetRenderer()->SetRenderTarget();
	}

	void KgeRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		KgeRenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

} // namespace MyGUI
