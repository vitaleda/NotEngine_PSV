#include "../../include/notengine/graphics/FrameCatalog.hpp"
#include "../../include/notengine/system/Utils.hpp"

#include <jsonxx.h>

using namespace NotEngine::System;

namespace NotEngine {

	namespace Graphics {

		FrameCatalog::FrameCatalog() : mWidth(0), mHeight(0) {
		}

		FrameCatalog::~FrameCatalog() {
		}

		int FrameCatalog::initialize(const std::string jsonString) {
			if (jsonString.length() == 0) {
				return BAD_JSON;
			}

			jsonxx::Object json;
			if (!json.parse(jsonString + "\n")) {
				return BAD_JSON;
			}

			if (!json.has<jsonxx::Object>("frames") || !json.has<jsonxx::Object>("meta")
			|| !json.get<jsonxx::Object>("meta").has<jsonxx::Object>("size")
			|| !json.get<jsonxx::Object>("meta").has<jsonxx::String>("image")
			|| !json.get<jsonxx::Object>("meta").get<jsonxx::Object>("size").has<jsonxx::Number>("w")
			|| !json.get<jsonxx::Object>("meta").get<jsonxx::Object>("size").has<jsonxx::Number>("h")) {
				return BADFORMED_CATALOG;
			}

			mWidth = json.get<jsonxx::Object>("meta").get<jsonxx::Object>("size").get<jsonxx::Number>("w");
			mHeight = json.get<jsonxx::Object>("meta").get<jsonxx::Object>("size").get<jsonxx::Number>("h");
			mTextureName = json.get<jsonxx::Object>("meta").get<jsonxx::String>("image");

			std::map<std::string, jsonxx::Value*> framesMap = json.get<jsonxx::Object>("frames").kv_map();
			for(jsonxx::Object::container::const_iterator it = framesMap.begin(); it != framesMap.end(); ++it) {
				if (!it->second->get<jsonxx::Object>().has<jsonxx::Object>("frame") ||
					!it->second->get<jsonxx::Object>().has<jsonxx::Object>("sourceSize")) {
					return BADFORMED_CATALOG;
				}

				jsonxx::Object frameObject = it->second->get<jsonxx::Object>().get<jsonxx::Object>("frame");
				if (!frameObject.has<jsonxx::Number>("x") || !frameObject.has<jsonxx::Number>("y")
					|| !frameObject.has<jsonxx::Number>("w") || !frameObject.has<jsonxx::Number>("h")) {
					return BADFORMED_CATALOG;
				}

				FrameCoords spriteCoords = (FrameCoords) {
					.s = (float) frameObject.get<jsonxx::Number>("x") / mWidth,
					.t = (float) frameObject.get<jsonxx::Number>("y") / mHeight,
					.u = (float) (frameObject.get<jsonxx::Number>("x") + frameObject.get<jsonxx::Number>("w")) / mWidth,
					.v = (float) (frameObject.get<jsonxx::Number>("y") + frameObject.get<jsonxx::Number>("h")) / mHeight
				};
				FrameSize spriteSize = (FrameSize) {
					.w = (unsigned int) frameObject.get<jsonxx::Number>("w"),
					.h = (unsigned int) frameObject.get<jsonxx::Number>("h")
				};
				Frame frame = (Frame) {
					.size = spriteSize,
					.coords =  spriteCoords
				};

				mCatalog.insert(
					std::pair<std::string, Frame>(it->first, frame)
				);
			}

			return NO_ERROR;
		}

		FrameCatalog::Frame FrameCatalog::getFrame(const std::string name) {
			if (mCatalog.find(name) == mCatalog.end()) {
				//printf("Frame not found %s, throwing default frame.\n", name.c_str());
				return Frame();
			}

			return mCatalog[name];
		}

		void FrameCatalog::finalize() {
			mCatalog.clear();
			mWidth = 0;
			mHeight = 0;
		}

		unsigned int FrameCatalog::getWidth() const {
			return mWidth;
		}

		unsigned int FrameCatalog::getHeight() const {
			return mHeight;
		}

		std::string FrameCatalog::getTextureName() const {
			return mTextureName;
		}

	} // namespace Graphics

} // namespace NotEngine
