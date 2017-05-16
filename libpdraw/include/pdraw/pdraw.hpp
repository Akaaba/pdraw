/**
 * @file pdraw.hpp
 * @brief Parrot Drones Awesome Video Viewer Library
 * @date 05/11/2016
 * @author aurelien.barre@akaaba.net
 *
 * Copyright (c) 2016 Aurelien Barre <aurelien.barre@akaaba.net>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 * 
 *   * Neither the name of the copyright holder nor the names of the
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PDRAW_HPP_
#define _PDRAW_HPP_

#include <inttypes.h>
#include <string>
#include "pdraw_defs.h"

namespace Pdraw
{


class IPdraw
{
public:

    virtual ~IPdraw() {};

    virtual int setup
            (const std::string &canonicalName,
             const std::string &friendlyName,
             const std::string &applicationName) = 0;

    virtual int open
            (const std::string &url) = 0;

    virtual int open
            (const std::string &srcAddr,
             const std::string &ifaceAddr,
             int srcStreamPort,
             int srcControlPort,
             int dstStreamPort,
             int dstControlPort,
             int qosMode) = 0;

    virtual int start() = 0;

    virtual int pause() = 0;

    virtual bool isPaused() = 0;

    virtual int stop() = 0;

    virtual int seekTo
            (uint64_t timestamp) = 0;

    virtual int seekForward
            (uint64_t delta) = 0;

    virtual int seekBack
            (uint64_t delta) = 0;

    virtual int startRecorder
            (const std::string &fileName) = 0;

    virtual int stopRecorder() = 0;

    virtual int startResender
            (const std::string &dstAddr,
             const std::string &ifaceAddr,
             int srcStreamPort,
             int srcControlPort,
             int dstStreamPort,
             int dstControlPort) = 0;

    virtual int stopResender() = 0;

    virtual int startRenderer
            (int windowWidth,
             int windowHeight,
             int renderX,
             int renderY,
             int renderWidth,
             int renderHeight,
             void *uiHandler) = 0;

    virtual int stopRenderer() = 0;

    virtual int render
            (int timeout) = 0;

    virtual int getMediaCount() = 0;

    virtual int getMediaInfo(unsigned int index, pdraw_media_info_t *info) = 0;

    virtual void *addVideoFrameFilterCallback(unsigned int mediaId, pdraw_video_frame_filter_callback_t cb, void *userPtr) = 0;

    virtual int removeVideoFrameFilterCallback(unsigned int mediaId, void *filterCtx) = 0;

    virtual void *addVideoFrameProducer(unsigned int mediaId) = 0;

    virtual int removeVideoFrameProducer(void *producerCtx) = 0;

    /*
     * get last frame
     *
     * waitUs : time in microseconds to wait a frame
     *  0: don't wait
     * -1: wait forever
     * >0: wait time
     */
    virtual int getProducerLastFrame(void *producerCtx, pdraw_video_frame_t *frame, long waitUs = 0) = 0;
};

IPdraw *createPdraw();

}

#endif /* !_PDRAW_HPP_ */
