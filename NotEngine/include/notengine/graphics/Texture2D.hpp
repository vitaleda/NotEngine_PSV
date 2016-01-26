#ifndef TEXTURE2D_HPP
#define	TEXTURE2D_HPP

#pragma once
#include <psp2/types.h>
#include <psp2/gxm.h>

namespace NotEngine {

	namespace Graphics {

		/**
		 * Base class that handle 2d graphics stuff.
		 */
		class Texture2D {
			friend class Graphics2D;

			private:
				/// Disallow copy
				Texture2D(const Texture2D& copy);
				void operator=(Texture2D const&);

				SceGxmTexture mTexture;
				SceUID mTextureMemoryUID;

				unsigned int mWidth;
				unsigned int mHeight;
				SceGxmTextureFormat mFormat;

			public:
				Texture2D ();
				virtual ~Texture2D ();

				/* INIT **************************************************/
				/// Initialize a texture
				bool initialize(unsigned int w, unsigned int h, SceGxmTextureFormat format);
				/// finalize a texture
				void finalize();
				/// Return a pointer to the datas
				void* getDataPtr();
				/// return the size of one pixel of the current format
				static unsigned int getStorageSize(SceGxmTextureFormat format);
		};

	} // namespace Graphics

} // namespace NotEngine

#endif	/* TEXTURE2D_HPP */
