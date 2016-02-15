#include "../../include/notengine/game/Director.hpp"

#include <stdio.h>
#include <psp2/rtc.h>

namespace NotEngine {

	namespace Game {

		Director::Director() : System::Singleton<Director>() {
			printf("Director()\n");
		}

		Director::~Director() {
			printf("~Director()\n");
		}

		bool Director::initialize(std::map<std::string, GameState*> states, std::string start) {
			mGameStates = states;

			if (mGameStates.find(start) == mGameStates.end()) {
				printf("Director cannot find first game state: %s\n", start.c_str());
				return false;
			}

			mCurrentState = states[start];
			if(! mCurrentState->enter()) {
				printf("Director GameState failed to initialize\n");
				return false;
			}

			return true;
		}

		void Director::finalize() {
		}

		void Director::update() {
			const float tickRate = 1.0f / sceRtcGetTickResolution();

			sceRtcGetCurrentTick(&mCurrentTicks);
			mElapsed = (mCurrentTicks - mLastTicks) * tickRate;
			mLastTicks = mCurrentTicks;

			if((mCurrentTicks - mLastFpsTicks) * tickRate >= 1) {
				mLastFpsTicks = mCurrentTicks;
				mFps = mFrames;
				mFrames = 0;
			}
			mFrames++;

			sceCtrlPeekBufferPositive(0, &mPadData, 1);
			mCurrentState->update(&mPadData, mElapsed);
		}

		bool Director::isRunning() {
			return !mQuit;
		}

		void Director::stop() {
			mCurrentState->exit();
			mQuit = true;
		}

		bool Director::changeState(const std::string name) {
			if (mGameStates.find(name) != mGameStates.end()) {
				mCurrentState->exit();
				mCurrentState = mGameStates[name];
				if (! mCurrentState->enter()) {
					printf("Director: GameState failed to initialize");
					return false;
				}

				return true;
			} else {
				return false;
			}
		}

		unsigned int Director::getFPS() {
			return mFps;
		}

	} // namespace Game

} // namespace NotEngine
