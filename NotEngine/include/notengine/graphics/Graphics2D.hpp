#ifndef GRAPHICS2D_HPP
#define	GRAPHICS2D_HPP

#pragma once
#include <psp2/types.h>
#include <psp2/gxm.h>
#include <glm/glm.hpp>

#include "../system/Singleton.hpp"
#include "Texture2D.hpp"
#include "Sprite.hpp"
#include "SpriteBuffer.hpp"

using namespace NotEngine;

namespace NotEngine {

	namespace Graphics {

		/**
		 * Base class that handle 2d graphics stuff.
		 */
		class Graphics2D : public System::Singleton<Graphics2D> {
			friend class System::Singleton<Graphics2D>;

			private:
				/// Disallow copy
				Graphics2D(const Graphics2D& copy);
				void operator=(Graphics2D const&);

				SceGxmShaderPatcherId m2dVertexProgramId;
				SceGxmShaderPatcherId m2dFragmentProgramId;

				const SceGxmProgramParameter* mShaderPositionAttr;
				const SceGxmProgramParameter* mShaderTextureAttr;
				const SceGxmProgramParameter* mShaderColorAttr;
				const SceGxmProgramParameter* mShaderAngleAttr;
				const SceGxmProgramParameter* mShaderTranslationAttr;
				const SceGxmProgramParameter* mShaderMatrixProjUnif;

				SceGxmVertexProgram* m2dVertexProgram;
				SceGxmFragmentProgram* m2dFragmentProgram;

				unsigned short* mBatchIndices;
				SceUID mBatchIndicesUID;

				/// Objects to use for clear operation
				SpriteBuffer* mClearBuffer;
				Sprite mClearSprite;
				Texture2D* mClearTexture;

				/// Disallow public instanciating
				Graphics2D ();

			public:
				static const unsigned int MAX_SPRITES_PER_BATCH = 8192;

				virtual ~Graphics2D ();

				/* INIT **************************************************/
				/// Initialize 2d subsystem
				bool initialize();
				/// free the 2d subsystem
				void finalize();
				/// Call it before start drawing with g2d
				void use();
				/// Call it after drawing
				void unuse();

				/* RENDER ************************************************/
				/// Clear the screen. be warned: this function actually unbind the current texture! Should be avoided if possible!!!
				void clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
				/// Set the current texture to use for render operation
				void setTexture(unsigned int unit, const Graphics::Texture2D* texture);
				/// Render all sprite added since startBatch sprite sprites
				void render(const glm::mat4* projection, const Graphics::SpriteBuffer* spriteBuffer);
		};

	} // namespace Graphics

} // namespace NotEngine

#endif	/* GRAPHICS2D_HPP */
