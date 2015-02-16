/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef __MYGUI_KGE_TEXTURE_H__
#define __MYGUI_KGE_TEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_Types.h"

namespace kge
{
	namespace gfx
	{
		class Texture;

	} // gfx

	class Device;

} // kge

namespace MyGUI
{

	class KgeTexture : public ITexture
	{
	public:
		KgeTexture(const std::string& _name, kge::Device* _device);
		virtual ~KgeTexture();

		virtual const std::string& getName() const;

		virtual void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format);
		virtual void loadFromFile(const std::string& _filename);
		virtual void saveToFile(const std::string& _filename) { }

		virtual void destroy();

		virtual void* lock(TextureUsage _access);
		virtual void unlock();
		virtual bool isLocked();

		virtual int getWidth();
		virtual int getHeight();

		virtual PixelFormat getFormat();
		virtual TextureUsage getUsage();
		virtual size_t getNumElemBytes();

		virtual IRenderTarget* getRenderTarget();

		/*internal:*/
		kge::gfx::Texture* getKgeTexture()
		{
			return mpTexture;
		}

	private:
		kge::Device* mpKGEDevice;
		kge::gfx::Texture* mpTexture;
		IntSize mSize;
		TextureUsage mTextureUsage;
		PixelFormat mPixelFormat;
		size_t mNumElemBytes;
		bool mLock;
		std::string mName;
		IRenderTarget* mRenderTarget;
		unsigned long mInternalUsage;
	};

} // namespace MyGUI

#endif // __MYGUI_KGE_TEXTURE_H__
