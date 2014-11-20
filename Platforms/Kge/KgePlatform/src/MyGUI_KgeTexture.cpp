/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#include <d3dx9.h>
#include "MyGUI_KgeTexture.h"
#include "MyGUI_KgeDataManager.h"
#include "MyGUI_KgeRTTexture.h"
#include "MyGUI_KgeDiagnostic.h"

namespace MyGUI
{

	KgeTexture::KgeTexture(const std::string& _name, IDirect3DDevice9* _device) :
		mName(_name),
		mpD3DDevice(_device),
		mpTexture(NULL),
		mNumElemBytes(0),
		mLock(false),
		mRenderTarget(nullptr),
		mInternalPool(D3DPOOL_MANAGED),
		mInternalFormat(D3DFMT_UNKNOWN),
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
		mInternalFormat = D3DFMT_UNKNOWN;

		mSize.set(_width, _height);
		mTextureUsage = _usage;
		mPixelFormat = _format;
		mInternalPool = D3DPOOL_MANAGED;

		if (mTextureUsage == TextureUsage::RenderTarget)
		{
			mInternalUsage |= D3DUSAGE_RENDERTARGET;
			mInternalPool = D3DPOOL_DEFAULT;
		}
		else if (mTextureUsage == TextureUsage::Dynamic)
			mInternalUsage |= D3DUSAGE_DYNAMIC;
		else if (mTextureUsage == TextureUsage::Stream)
			mInternalUsage |= D3DUSAGE_DYNAMIC;

		if (mPixelFormat == PixelFormat::R8G8B8A8)
		{
			mInternalFormat = D3DFMT_A8R8G8B8;
			mNumElemBytes = 4;
		}
		else if (mPixelFormat == PixelFormat::R8G8B8)
		{
			mInternalFormat = D3DFMT_R8G8B8;
			mNumElemBytes = 3;
		}
		else if (mPixelFormat == PixelFormat::L8A8)
		{
			mInternalFormat = D3DFMT_A8L8;
			mNumElemBytes = 2;
		}
		else if (mPixelFormat == PixelFormat::L8)
		{
			mInternalFormat = D3DFMT_L8;
			mNumElemBytes = 1;
		}
		else
		{
			MYGUI_PLATFORM_EXCEPT("Creating texture with unknown pixel formal.");
		}

		HRESULT result = mpD3DDevice->CreateTexture(mSize.width, mSize.height, 1, mInternalUsage, mInternalFormat, mInternalPool, &mpTexture, NULL);
		if (FAILED(result))
		{
			MYGUI_PLATFORM_EXCEPT("Failed to create texture (error code " << result <<"): size '" << mSize <<
				"' internal usage '" << mInternalUsage <<
				"' internal format '" << mInternalFormat << "'."
				);
		}

	}

	void KgeTexture::loadFromFile(const std::string& _filename)
	{
		destroy();
		mTextureUsage = TextureUsage::Default;
		mPixelFormat = PixelFormat::R8G8B8A8;
		mNumElemBytes = 4;

		std::string fullname = KgeDataManager::getInstance().getDataPath(_filename);

		D3DXIMAGE_INFO info;
		D3DXGetImageInfoFromFile(fullname.c_str(), &info);

		if (info.Format == D3DFMT_A8R8G8B8)
		{
			mPixelFormat = PixelFormat::R8G8B8A8;
			mNumElemBytes = 4;
		}
		else if (info.Format == D3DFMT_R8G8B8)
		{
			mPixelFormat = PixelFormat::R8G8B8;
			mNumElemBytes = 3;
		}
		else if (info.Format == D3DFMT_A8L8)
		{
			mPixelFormat = PixelFormat::L8A8;
			mNumElemBytes = 2;
		}
		else if (info.Format == D3DFMT_L8)
		{
			mPixelFormat = PixelFormat::L8;
			mNumElemBytes = 1;
		}

		mSize.set(info.Width, info.Height);
		HRESULT result = D3DXCreateTextureFromFile(mpD3DDevice, fullname.c_str(), &mpTexture);
		if (FAILED(result))
		{
			MYGUI_PLATFORM_EXCEPT("Failed to load texture '" << _filename <<
				"' (error code " << result <<
				"): size '" << mSize <<
				"' format '" << info.Format << "'."
				);
		}
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
			int nNewRefCount = mpTexture->Release();

			if (nNewRefCount > 0)
			{
				MYGUI_PLATFORM_EXCEPT("The texture object failed to cleanup properly.\n"
					"Release() returned a reference count of '" << nNewRefCount << "'."
					);
			}

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
		D3DLOCKED_RECT d3dlr;
		int lockFlag = (_access == TextureUsage::Write) ? D3DLOCK_DISCARD : D3DLOCK_READONLY;

		HRESULT result = mpTexture->LockRect(0, &d3dlr, NULL, lockFlag);
		if (FAILED(result))
		{
			MYGUI_PLATFORM_EXCEPT("Failed to lock texture (error code " << result << ").");
		}

		mLock = true;
		return d3dlr.pBits;
	}

	void KgeTexture::unlock()
	{
		HRESULT result = mpTexture->UnlockRect(0);
		if (FAILED(result))
		{
			MYGUI_PLATFORM_EXCEPT("Failed to unlock texture (error code " << result << ").");
		}

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
			mRenderTarget = new KgeRTTexture(mpD3DDevice, mpTexture);

		return mRenderTarget;
	}

	void KgeTexture::deviceLost()
	{
		if (mInternalPool == D3DPOOL_DEFAULT)
		{
			destroy();
		}
	}

	void KgeTexture::deviceRestore()
	{
		if (mInternalPool == D3DPOOL_DEFAULT)
		{
			HRESULT result = mpD3DDevice->CreateTexture(mSize.width, mSize.height, 1, mInternalUsage, mInternalFormat, D3DPOOL_DEFAULT, &mpTexture, NULL);
			if (FAILED(result))
			{
				MYGUI_PLATFORM_EXCEPT("Failed to recreate texture on device restore (error code " << result << ").");
			}
		}
	}

} // namespace MyGUI
