/**
 * @file pdraw_impl.cpp
 * @brief Parrot Drones Awesome Video Viewer Library - interface implementation
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

#include "pdraw_impl.hpp"
#include "pdraw_demuxer_stream.hpp"
#include "pdraw_demuxer_record.hpp"
#include "pdraw_decoder.hpp"
#include "pdraw_avcdecoder.hpp"
#include "pdraw_renderer.hpp"
#include "pdraw_media_video.hpp"
#include "pdraw_filter_videoframe.hpp"

#include <unistd.h>
#include <sched.h>

#define ULOG_TAG libpdraw
#include <ulog.h>
ULOG_DECLARE_TAG(libpdraw);


namespace Pdraw
{


IPdraw *createPdraw()
{
    return new PdrawImpl;
}


PdrawImpl::PdrawImpl()
{
    mSetup = false;
    mPaused = false;
    mGotRendererParams = false;
    mUiHandler = NULL;
}


PdrawImpl::~PdrawImpl()
{

}


int PdrawImpl::setup(const std::string &canonicalName,
                     const std::string &friendlyName,
                     const std::string &applicationName)
{
    int ret = 0;

    if (mSetup)
    {
        ULOGE("Pdraw is already set up");
        return -1;
    }

    //TODO: renaming
    mSession.setup(friendlyName, canonicalName, applicationName);

    mSetup = true;

    return ret;
}


int PdrawImpl::open(const std::string &url)
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    return mSession.open(url);
}


int PdrawImpl::open(const std::string &srcAddr, const std::string &ifaceAddr,
                    int srcStreamPort, int srcControlPort,
                    int dstStreamPort, int dstControlPort, int qosMode)
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    return mSession.open(srcAddr, ifaceAddr,
                        srcStreamPort, srcControlPort,
                        dstStreamPort, dstControlPort, qosMode);
}


int PdrawImpl::start()
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    if (mSession.getDemuxer())
    {
        int ret = mSession.getDemuxer()->start();
        if (ret != 0)
        {
            ULOGE("Failed to start demuxer");
            return -1;
        }
        else
        {
            mPaused = false;
        }
    }
    else
    {
        ULOGE("Invalid demuxer");
        return -1;
    }

    return 0;
}


int PdrawImpl::pause()
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    if (mSession.getDemuxer())
    {
        int ret = mSession.getDemuxer()->pause();
        if (ret != 0)
        {
            ULOGE("Failed to pause demuxer");
            return -1;
        }
        else
        {
            mPaused = true;
        }
    }
    else
    {
        ULOGE("Invalid demuxer");
        return -1;
    }

    return 0;
}


bool PdrawImpl::isPaused()
{
    return mPaused;
}


int PdrawImpl::stop()
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    if (mSession.getDemuxer())
    {
        int ret = mSession.getDemuxer()->stop();
        if (ret != 0)
        {
            ULOGE("Failed to stop demuxer");
            return -1;
        }
        else
        {
            mPaused = false;
        }
    }
    else
    {
        ULOGE("Invalid demuxer");
        return -1;
    }

    return 0;
}


int PdrawImpl::seekTo(uint64_t timestamp)
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    if (mSession.getDemuxer())
    {
        int ret = mSession.getDemuxer()->seekTo(timestamp);
        if (ret != 0)
        {
            ULOGE("Failed to seek with demuxer");
            return -1;
        }
    }
    else
    {
        ULOGE("Invalid demuxer");
        return -1;
    }

    return 0;
}


int PdrawImpl::seekForward(uint64_t delta)
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    if (mSession.getDemuxer())
    {
        int ret = mSession.getDemuxer()->seekForward(delta);
        if (ret != 0)
        {
            ULOGE("Failed to seek with demuxer");
            return -1;
        }
    }
    else
    {
        ULOGE("Invalid demuxer");
        return -1;
    }

    return 0;
}


int PdrawImpl::seekBack(uint64_t delta)
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    if (mSession.getDemuxer())
    {
        int ret = mSession.getDemuxer()->seekBack(delta);
        if (ret != 0)
        {
            ULOGE("Failed to seek with demuxer");
            return -1;
        }
    }
    else
    {
        ULOGE("Invalid demuxer");
        return -1;
    }

    return 0;
}


int PdrawImpl::startRecorder(const std::string &fileName)
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    if ((mSession.getDemuxer()) && (mSession.getDemuxer()->getType() == DEMUXER_TYPE_STREAM))
    {
        return ((StreamDemuxer*)mSession.getDemuxer())->startRecorder(fileName);
    }
    else
    {
        ULOGE("Invalid demuxer");
        return -1;
    }
}


int PdrawImpl::stopRecorder()
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    if ((mSession.getDemuxer()) && (mSession.getDemuxer()->getType() == DEMUXER_TYPE_STREAM))
    {
        return ((StreamDemuxer*)mSession.getDemuxer())->stopRecorder();
    }
    else
    {
        ULOGE("Invalid demuxer");
        return -1;
    }
}


int PdrawImpl::startResender(const std::string &dstAddr, const std::string &ifaceAddr,
                             int srcStreamPort, int srcControlPort,
                             int dstStreamPort, int dstControlPort)
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    if ((mSession.getDemuxer()) && (mSession.getDemuxer()->getType() == DEMUXER_TYPE_STREAM))
    {
        return ((StreamDemuxer*)mSession.getDemuxer())->startResender(dstAddr, ifaceAddr,
                                                       srcStreamPort, srcControlPort,
                                                       dstStreamPort, dstControlPort);
    }
    else
    {
        ULOGE("Invalid demuxer");
        return -1;
    }
}


int PdrawImpl::stopResender()
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    if ((mSession.getDemuxer()) && (mSession.getDemuxer()->getType() == DEMUXER_TYPE_STREAM))
    {
        return ((StreamDemuxer*)mSession.getDemuxer())->stopResender();
    }
    else
    {
        ULOGE("Invalid demuxer");
        return -1;
    }
}


int PdrawImpl::startRenderer(int windowWidth, int windowHeight,
                             int renderX, int renderY,
                             int renderWidth, int renderHeight,
                             void *uiHandler)
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    mGotRendererParams = true;
    mUiHandler = uiHandler;
    mWindowWidth = windowWidth;
    mWindowHeight = windowHeight;
    mRenderX = renderX;
    mRenderY = renderY;
    mRenderWidth = renderWidth;
    mRenderHeight = renderHeight;

    if (!mSession.getRenderer())
    {
        int ret = mSession.enableRenderer();
        if (ret != 0)
        {
            ULOGE("Failed to enable renderer");
        }
    }

    if (mSession.getRenderer())
    {
        return mSession.getRenderer()->setRendererParams(mWindowWidth, mWindowHeight,
                                            mRenderX, mRenderY,
                                            mRenderWidth, mRenderHeight,
                                            mUiHandler);
    }
    else
    {
        ULOGE("Invalid renderer");
        return -1;
    }
}


int PdrawImpl::stopRenderer()
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    if (mSession.getRenderer())
    {
        int ret = mSession.disableRenderer();
        if (ret != 0)
        {
            ULOGE("Failed to disable renderer");
            return -1;
        }
    }
    else
    {
        ULOGE("Invalid renderer");
        return -1;
    }

    return 0;
}


int PdrawImpl::render(int timeout)
{
    if (!mSetup)
    {
        ULOGE("Pdraw is not set up");
        return -1;
    }

    if (mSession.getRenderer())
    {
        return mSession.getRenderer()->render(timeout);
    }
    else
    {
        ULOGE("Invalid renderer");
        return -1;
    }
}


int PdrawImpl::getMediaCount()
{
    return mSession.getMediaCount();
}


int PdrawImpl::getMediaInfo(unsigned int index, pdraw_media_info_t *info)
{
    if (!info)
    {
        ULOGE("Invalid info struct");
        return -1;
    }

    Media *media = mSession.getMedia(index);

    if (!media)
    {
        ULOGE("Invalid media index");
        return -1;
    }

    switch (media->getType())
    {
        case MEDIA_TYPE_VIDEO:
            info->type = PDRAW_MEDIA_TYPE_VIDEO;
            break;
        default:
            info->type = PDRAW_MEDIA_TYPE_UNKNOWN;
            break;
    }
    info->id = media->getId();

    return 0;
}


void *PdrawImpl::addVideoFrameFilterCallback(unsigned int mediaId, pdraw_video_frame_filter_callback_t cb, void *userPtr)
{
    Media *media = mSession.getMediaById(mediaId);

    if (!media)
    {
        ULOGE("Invalid media id");
        return NULL;
    }

    if (media->getType() != MEDIA_TYPE_VIDEO)
    {
        ULOGE("Invalid media type");
        return NULL;
    }

    VideoFrameFilter *filter = ((VideoMedia*)media)->addVideoFrameFilter(cb, userPtr);
    if (!filter)
    {
        ULOGE("Failed to create video frame filter");
        return NULL;
    }

    return (void*)filter;
}


int PdrawImpl::removeVideoFrameFilterCallback(unsigned int mediaId, void *filterCtx)
{
    Media *media = mSession.getMediaById(mediaId);

    if (!media)
    {
        ULOGE("Invalid media id");
        return -1;
    }

    if (media->getType() != MEDIA_TYPE_VIDEO)
    {
        ULOGE("Invalid media type");
        return -1;
    }

    if (!filterCtx)
    {
        ULOGE("Invalid context pointer");
        return -1;
    }

    VideoFrameFilter *filter = (VideoFrameFilter*)filterCtx;
    return ((VideoMedia*)media)->removeVideoFrameFilter(filter);
}


void *PdrawImpl::addVideoFrameProducer(unsigned int mediaId)
{
    Media *media = mSession.getMediaById(mediaId);

    if (!media)
    {
        ULOGE("Invalid media id");
        return NULL;
    }

    if (media->getType() != MEDIA_TYPE_VIDEO)
    {
        ULOGE("Invalid media type");
        return NULL;
    }

    VideoFrameFilter *filter = ((VideoMedia*)media)->addVideoFrameFilter();
    if (!filter)
    {
        ULOGE("Failed to create video frame filter");
        return NULL;
    }

    return (void*)filter;
}


int PdrawImpl::removeVideoFrameProducer(void *producerCtx)
{
    if (!producerCtx)
    {
        ULOGE("Invalid context pointer");
        return -1;
    }

    VideoFrameFilter *filter = (VideoFrameFilter*)producerCtx;
    VideoMedia *media = filter->getMedia();

    if (!media)
    {
        ULOGE("Invalid media");
        return -1;
    }

    return media->removeVideoFrameFilter(filter);
}


int PdrawImpl::getProducerLastFrame(void *producerCtx, pdraw_video_frame_t *frame, long waitUs)
{
    if (!producerCtx)
    {
        ULOGE("Invalid context pointer");
        return -1;
    }
    if (!frame)
    {
        ULOGE("Invalid frame structure pointer");
        return -1;
    }

    VideoFrameFilter *filter = (VideoFrameFilter*)producerCtx;

    return filter->getLastFrame(frame, waitUs);
}

}
