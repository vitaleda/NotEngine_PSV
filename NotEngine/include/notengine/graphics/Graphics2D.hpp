#ifndef GRAPHICS2D_HPP
#define	GRAPHICS2D_HPP

#pragma once
#include <psp2/types.h>
#include <psp2/gxm.h>
#include <glm/glm.hpp>

#include "../system/Singleton.hpp"
#include "Texture2D.hpp"
#include "Sprite.hpp"
#include "IndiceBuffer.hpp"
#include "D2Buffer.hpp"
#include "SpriteBuffer.hpp"

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
				const SceGxmProgramParameter* mShaderTextureEnableUnif;
				const SceGxmProgramParameter* mShaderTRSEnableUnif;

				SceGxmVertexProgram* m2dVertexProgram;
				SceGxmFragmentProgram* m2dFragmentProgram;

				IndiceBuffer* mSpriteIndiceBuffer;
				IndiceBuffer* mShapeIndiceBuffer;

				/// Objects to use for clear operation
				SpriteBuffer* mClearBuffer;
				Sprite mClearSprite;
				Texture2D* mClearTexture;

				/// Disallow public instanciating
				Graphics2D ();

			public:
				// 12288 vertices for shapes
				static const unsigned int MAX_VERTICES_PER_BATCH = 12288;
				// 2048 sprites
				static const unsigned int MAX_SPRITES_PER_BATCH = MAX_VERTICES_PER_BATCH/6;

				virtual ~Graphics2D ();

				/* INIT **************************************************/
				/// Initialize 2d subsystem
				int initialize();
				/// free the 2d subsystem
				void finalize();
				/// Call it before start drawing with g2d
				void use();

				/* RENDER ************************************************/
				/// Clear the screen. be warned: this function actually unbind the current texture and buffer! Should be avoided if possible or use your own "clear sprite" !!!
				void clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
				/// Set the current texture to use for render operation
				void setTexture(const Texture2D* texture) const;
				/// Render all sprite added in the buffer since the last call to draw the batch
				void render(const glm::mat4& projection, SpriteBuffer* spriteBuffer);
				/// Render all shapes added in the buffer since the last draw (optional texture)
				int render(const glm::mat4& projection, SceGxmPrimitiveType type, D2Buffer* vertices, bool texture);
				/// Render all shapes added in the buffer since the last draw using an index buffer (optional texture)
				int render(const glm::mat4& projection, SceGxmPrimitiveType type, IndiceBuffer* indices, D2Buffer* vertices, bool texture, int startIndice, int indiceCount);

				enum ERROR {
					NO_ERROR = 0,
					VERTEX_SCEGXM_PROGRAM_CHECK = 1,
					FRAGMENT_SCEGXM_PROGRAM_CHECK = 2,
					VERTEX_SCEGXM_REGISTER_PROGRAM = 3,
					FRAGMENT_SCEGXM_REGISTER_PROGRAM = 4,
					VERTEX_SCEGXM_CREATE_PROGRAM = 5,
					FRAGMENT_SCEGXM_CREATE_PROGRAM = 6,
					SPRITES_INDICES_GPU_ALLOC = 7,
					SHAPES_INDICES_GPU_ALLOC = 8,
					CLEAR_TEXTURE_INITIALIZE = 9,
					CLEAR_BUFFER_INITIALIZE = 10,
					WRONG_VERTICES_COUNT = 11,
					UNSUPPORTED_FEATURE = 12
				};

		};

	} // namespace Graphics

} // namespace NotEngine

#endif	/* GRAPHICS2D_HPP */
