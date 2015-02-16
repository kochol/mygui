/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#include "MyGUI_KgeTexture.h"
#include "MyGUI_KgeDataManager.h"
#include "MyGUI_KgeRTTexture.h"
#include "MyGUI_KgeDiagnostic.h"
#include <gfx/Texture.h>
#include <Device.h>
#include <gfx/Renderer.h>

namespace MyGUI
{

	KgeTexture::KgeTexture(const std::string& _name, kge::Device* _device) :
		mName(_name),
		mpKGEDevice(_device),
		mpTexture(NULL),
		mNumElemBytes(0),
		mLock(false),
		mRenderTarget(nullptr),
		mInternalUsage(0)
	{
	}

	KgeTexture::~KgeTexture()
	{
		destroy();
	}

	const std::string& KgeTexture::getName() const
	{
		return mName;
	}

	void KgeTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		destroy();

		mInternalUsage = 0;

		mSize.set(_width, _height);
		mTextureUsage = _usage;
		mPixelFormat = _format;
		bool Dynamic = false;

		if (mTextureUsage == TextureUsage::Dynamic)
			Dynamic = true;
		else if (mTextureUsage == TextureUsage::Stream)
			Dynamic = true;

		kge::gfx::TextureFormat tf;
		if (mPixelFormat == PixelFormat::R8G8B8A8)
		{
			tf = kge::gfx::ETF_A8R8G8B8;
			mNumElemBytes = 4;
		}
		else if (mPixelFormat == PixelFormat::R8G8B8)
		{
			tf = kge::gfx::ETF_R8G8B8;
			mNumElemBytes = 3;
		}
		else if (mPixelFormat == PixelFormat::L8A8)
		{
			tf = kge::gfx::ETF_A8L8;
			mNumElemBytes = 2;
		}
		else if (mPixelFormat == PixelFormat::L8)
		{
			tf = kge::gfx::ETF_A8;
//			mInternalFormat = D3DFMT_L8;
			mNumElemBytes = 1;
		}
		else
		{
			MYGUI_PLATFORM_EXCEPT("Creating texture with unknown pixel formal.");
		}

		if (mTextureUsage == TextureUsage::RenderTarget)
		{
			mpKGEDevice->GetRenderer()->CreateRenderableTexture(&mpTexture, _width, _height, kge::gfx::ETF_A8R8G8B8);
		}
		else
		{
			static int i = 0;
			i++;
			std::string str = "MyGUITexture" + i;
			int ihandle = mpKGEDevice->GetTextureManager()->Add(NULL, NULL, str.c_str());
			mpTexture = mpKGEDevice->GetTextureManager()->GetResource(ihandle);
			mpTexture->CreateTexture(_width, _height, tf, 0, false, Dynamic);
		}		
	}

	void KgeTexture::loadFromFile(const std::string& _filename)
	{
		destroy();
		mTextureUsage = TextureUsage::Default;
		mPixelFormat = PixelFormat::R8G8B8A8;
		mNumElemBytes = 4;

		std::string fullname = KgeDataManager::getInstance().getDataPath(_filename);

		int ihandle = mpKGEDevice->GetTextureManager()->Add(fullname.c_str(), NULL, NULL);
		mpTexture = mpKGEDevice->GetTextureManager()->GetResource(ihandle);
		kge::gfx::TextureFormat tf = mpTexture->GetFormat();
		if (tf == kge::gfx::ETF_A8R8G8B8)
		{
			mPixelFormat = PixelFormat::R8G8B8A8;
			mNumElemBytes = 4;
		}
		else if (tf == kge::gfx::ETF_R8G8B8)
		{
			mPixelFormat = PixelFormat::R8G8B8;
			mNumElemBytes = 3;
		}
		else if (tf == kge::gfx::ETF_A8L8)
		{
			mPixelFormat = PixelFormat::L8A8;
			mNumElemBytes = 2;
		}
		else if (tf == kge::gfx::ETF_A8)
		{
			mPixelFormat = PixelFormat::L8;
			mNumElemBytes = 1;
		}
		
		mSize.set(mpTexture->GetWidth(), mpTexture->GetHeight());
	}

	void KgeTexture::destroy()
	{
		if (mRenderTarget != nullptr)
		{
			delete mRenderTarget;
			mRenderTarget = nullptr;
		}

		if (mpTexture != nullptr)
		{
			mpTexture->DecRef();

			mpTexture = nullptr;
		}
	}

	int KgeTexture::getWidth()
	{
		return mSize.width;
	}

	int KgeTexture::getHeight()
	{
		return mSize.height;
	}

	void* KgeTexture::lock(TextureUsage _access)
	{
		
//		int lockFlag = (_access == TextureUsage::Write) ? D3DLOCK_DISCARD : D3DLOCK_READONLY;

		mLock = true;
		return mpTexture->Lock();
	}

	void KgeTexture::unlock()
	{
		mpTexture->UnLock();

		mLock = false;
	}

	bool KgeTexture::isLocked()
	{
		return mLock;
	}

	PixelFormat KgeTexture::getFormat()
	{
		return mPixelFormat;
	}

	size_t KgeTexture::getNumElemBytes()
	{
		return mNumElemBytes;
	}

	TextureUsage KgeTexture::getUsage()
	{
		return mTextureUsage;
	}

	IRenderTarget* KgeTexture::getRenderTarget()
	{
		if (mpTexture == nullptr)
			return nullptr;

		if (mRenderTarget == nullptr)
			mRenderTarget = new KgeRTTexture(mpKGEDevice, mpTexture);

		return mRenderTarget;
	}

} // namespace MyGUI
