/**
 * Parrot Drones Awesome Video Viewer Library
 * OpenGL ES 2.0 renderer
 *
 * Copyright (c) 2016 Aurelien Barre
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PDRAW_RENDERER_GLES2_HPP_
#define _PDRAW_RENDERER_GLES2_HPP_

#ifdef USE_GLES2

#include <pthread.h>
#include "pdraw_renderer.hpp"
#include "pdraw_gles2_video.hpp"
#include "pdraw_gles2_hud.hpp"
#include "pdraw_gles2_hmd.hpp"

namespace Pdraw {


class Gles2Renderer : public Renderer {
public:
	Gles2Renderer(
		Session *session);

	~Gles2Renderer(
		void);

	int open(
		unsigned int windowWidth,
		unsigned int windowHeight,
		int renderX,
		int renderY,
		unsigned int renderWidth,
		unsigned int renderHeight,
		bool hud,
		bool hmdDistorsionCorrection,
		bool headtracking,
		struct egl_display *eglDisplay);

	int close(
		void);

	int render(
		int renderX,
		int renderY,
		unsigned int renderWidth,
		unsigned int renderHeight,
		uint64_t timestamp);

	int addInputSource(
		Media *media);

	int removeInputSource(
		Media *media);

	int getInputSourceQueue(
		Media *media,
		struct vbuf_queue **queue);

	Session *getSession(
		void) {
		return mSession;
	}

	Media *getMedia(
		void) {
		return mMedia;
	}

	VideoMedia *getVideoMedia(
		void) {
		return (VideoMedia *)mMedia;
	}

protected:
	virtual int loadVideoFrame(
		const uint8_t *data,
		struct avcdecoder_output_buffer *frame,
		enum gles2_video_color_conversion colorConversion);

	pthread_mutex_t mMutex;
	bool mRunning;
	struct vbuf_queue *mQueue;
	struct vbuf_buffer *mCurrentBuffer;
	unsigned int mWindowWidth;
	unsigned int mWindowHeight;
	bool mHud;
	bool mHmdDistorsionCorrection;
	bool mHeadtracking;
	Gles2Hmd *mGles2Hmd;
	unsigned int mGles2HmdFirstTexUnit;
	Gles2Video *mGles2Video;
	unsigned int mGles2VideoFirstTexUnit;
	Gles2Hud *mGles2Hud;
	unsigned int mGles2HudFirstTexUnit;
	GLuint mFbo;
	GLuint mFboTexture;
	GLuint mFboRenderBuffer;
};

} /* namespace Pdraw */

#endif /* USE_GLES2 */

#endif /* !_PDRAW_RENDERER_GLES2_HPP_ */
