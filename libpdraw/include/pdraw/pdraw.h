/**
 * Parrot Drones Awesome Video Viewer Library
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

#ifndef _PDRAW_H_
#define _PDRAW_H_

#ifdef __cplusplus
extern "C"  {
#endif /* __cplusplus */

#include <inttypes.h>
#include "pdraw_defs.h"


/* Forward declaration */
struct pdraw;


enum pdraw_state {
	PDRAW_STATE_INVALID = 0,
	PDRAW_STATE_CREATED,
	PDRAW_STATE_OPENED,
	PDRAW_STATE_CLOSED,
};


struct pdraw_cbs {
	void (*state_changed)(
		struct pdraw *pdraw,
		enum pdraw_state state,
		void *userdata);

	void (*open_resp)(
		struct pdraw *pdraw,
		int status,
		void *userdata);

	void (*close_resp)(
		struct pdraw *pdraw,
		int status,
		void *userdata);

	void (*play_resp)(
		struct pdraw *pdraw,
		int status,
		uint64_t timestamp,
		void *userdata);

	void (*pause_resp)(
		struct pdraw *pdraw,
		int status,
		uint64_t timestamp,
		void *userdata);

	void (*seek_resp)(
		struct pdraw *pdraw,
		int status,
		uint64_t timestamp,
		void *userdata);

	void (*socket_created)(
		struct pdraw *pdraw,
		int fd,
		void *userdata);
};


const char *pdraw_state_str(
	enum pdraw_state val);


int pdraw_new(
	struct pomp_loop *loop,
	const struct pdraw_cbs *cbs,
	void *userdata,
	struct pdraw **ret_obj);


int pdraw_destroy(
	struct pdraw *pdraw);


int pdraw_open_url(
	struct pdraw *pdraw,
	const char *url);


int pdraw_open_url_mcast(
	struct pdraw *pdraw,
	const char *url,
	const char *ifaceAddr);


int pdraw_open_single_stream(
	struct pdraw *pdraw,
	const char *localAddr,
	uint16_t localStreamPort,
	uint16_t localControlPort,
	const char *remoteAddr,
	uint16_t remoteStreamPort,
	uint16_t remoteControlPort,
	const char *ifaceAddr);


int pdraw_open_url_mux(
	struct pdraw *pdraw,
	const char *url,
	struct mux_ctx *mux);


int pdraw_open_single_stream_mux(
	struct pdraw *pdraw,
	struct mux_ctx *mux);


int pdraw_open_sdp(
	struct pdraw *pdraw,
	const char *sdp,
	const char *ifaceAddr);


int pdraw_open_sdp_mux(
	struct pdraw *pdraw,
	const char *sdp,
	struct mux_ctx *mux);


int pdraw_close(
	struct pdraw *pdraw);


int pdraw_play(
	struct pdraw *pdraw);


int pdraw_play_with_speed(
	struct pdraw *pdraw,
	float speed);


int pdraw_pause(
	struct pdraw *pdraw);


int pdraw_is_paused(
	struct pdraw *pdraw);


int pdraw_previous_frame(
	struct pdraw *pdraw);


int pdraw_next_frame(
	struct pdraw *pdraw);


int pdraw_seek(
	struct pdraw *pdraw,
	int64_t delta,
	int exact);


int pdraw_seek_forward(
	struct pdraw *pdraw,
	uint64_t delta,
	int exact);


int pdraw_seek_back(
	struct pdraw *pdraw,
	uint64_t delta,
	int exact);


int pdraw_seek_to(
	struct pdraw *pdraw,
	uint64_t timestamp,
	int exact);


uint64_t pdraw_get_duration(
	struct pdraw *pdraw);


uint64_t pdraw_get_current_time(
	struct pdraw *pdraw);


int pdraw_start_video_renderer(
	struct pdraw *pdraw,
	unsigned int windowWidth,
	unsigned int windowHeight,
	int renderX,
	int renderY,
	unsigned int renderWidth,
	unsigned int renderHeight,
	int enableHud,
	int enableHmdDistorsionCorrection,
	int enableHeadtracking);


int pdraw_start_video_renderer_egl(
	struct pdraw *pdraw,
	unsigned int windowWidth,
	unsigned int windowHeight,
	int renderX,
	int renderY,
	unsigned int renderWidth,
	unsigned int renderHeight,
	int enableHud,
	int enableHmdDistorsionCorrection,
	int enableHeadtracking,
	struct egl_display *eglDisplay);


int pdraw_stop_video_renderer(
	struct pdraw *pdraw);


int pdraw_render_video(
	struct pdraw *pdraw,
	int renderX,
	int renderY,
	unsigned int renderWidth,
	unsigned int renderHeight,
	uint64_t timestamp);


enum pdraw_session_type pdraw_get_session_type(
	struct pdraw *pdraw);


uint16_t pdraw_get_single_stream_local_stream_port(
	struct pdraw *pdraw);


uint16_t pdraw_get_single_stream_local_control_port(
	struct pdraw *pdraw);


char *pdraw_get_self_friendly_name(
	struct pdraw *pdraw);


int pdraw_set_self_friendly_name(
	struct pdraw *pdraw,
	const char *friendlyName);


char *pdraw_get_self_serial_number(
	struct pdraw *pdraw);


int pdraw_set_self_serial_number(
	struct pdraw *pdraw,
	const char *serialNumber);


char *pdraw_get_self_software_version(
	struct pdraw *pdraw);


int pdraw_set_self_software_version(
	struct pdraw *pdraw,
	const char *softwareVersion);


int pdraw_is_self_pilot(
	struct pdraw *pdraw);


int pdraw_set_self_pilot(
	struct pdraw *pdraw,
	int isPilot);


int pdraw_get_self_location(
	struct pdraw *pdraw,
	struct vmeta_location *loc);


int pdraw_set_self_location(
	struct pdraw *pdraw,
	const struct vmeta_location *loc);


int pdraw_get_self_controller_battery_level(
	struct pdraw *pdraw);


int pdraw_set_self_controller_battery_level(
	struct pdraw *pdraw,
	int batteryLevel);


int pdraw_get_self_controller_orientation_quat(
	struct pdraw *pdraw,
	struct vmeta_quaternion *quat);


int pdraw_get_self_controller_orientation_euler(
	struct pdraw *pdraw,
	struct vmeta_euler *euler);


int pdraw_set_self_controller_orientation_quat(
	struct pdraw *pdraw,
	const struct vmeta_quaternion *quat);


int pdraw_set_self_controller_orientation_euler(
	struct pdraw *pdraw,
	const struct vmeta_euler *euler);


int pdraw_get_self_head_orientation_quat(
	struct pdraw *pdraw,
	struct vmeta_quaternion *quat);


int pdraw_get_self_head_orientation_euler(
	struct pdraw *pdraw,
	struct vmeta_euler *euler);


int pdraw_set_self_head_orientation_quat(
	struct pdraw *pdraw,
	const struct vmeta_quaternion *quat);


int pdraw_set_self_head_orientation_euler(
	struct pdraw *pdraw,
	const struct vmeta_euler *euler);


int pdraw_get_self_head_ref_orientation_quat(
	struct pdraw *pdraw,
	struct vmeta_quaternion *quat);


int pdraw_get_self_head_ref_orientation_euler(
	struct pdraw *pdraw,
	struct vmeta_euler *euler);


int pdraw_set_self_head_ref_orientation_quat(
	struct pdraw *pdraw,
	const struct vmeta_quaternion *quat);


int pdraw_set_self_head_ref_orientation_euler(
	struct pdraw *pdraw,
	const struct vmeta_euler *euler);


int pdraw_reset_self_head_ref_orientation(
	struct pdraw *pdraw);


char *pdraw_get_peer_friendly_name(
	struct pdraw *pdraw);


char *pdraw_get_peer_maker(
	struct pdraw *pdraw);


char *pdraw_get_peer_model(
	struct pdraw *pdraw);


char *pdraw_get_peer_model_id(
	struct pdraw *pdraw);


enum pdraw_drone_model pdraw_get_peer_drone_model(
	struct pdraw *pdraw);


char *pdraw_get_peer_serial_number(
	struct pdraw *pdraw);


char *pdraw_get_peer_software_version(
	struct pdraw *pdraw);


char *pdraw_get_peer_build_id(
	struct pdraw *pdraw);


char *pdraw_get_peer_title(
	struct pdraw *pdraw);


char *pdraw_get_peer_comment(
	struct pdraw *pdraw);


char *pdraw_get_peer_copyright(
	struct pdraw *pdraw);


char *pdraw_get_peer_run_date(
	struct pdraw *pdraw);


char *pdraw_get_peer_run_uuid(
	struct pdraw *pdraw);


char *pdraw_get_peer_media_date(
	struct pdraw *pdraw);


int pdraw_get_peer_takeoff_location(
	struct pdraw *pdraw,
	struct vmeta_location *loc);


int pdraw_set_peer_takeoff_location(
	struct pdraw *pdraw,
	const struct vmeta_location *loc);


int pdraw_get_peer_home_location(
	struct pdraw *pdraw,
	struct vmeta_location *loc);


int pdraw_set_peer_home_location(
	struct pdraw *pdraw,
	const struct vmeta_location *loc);


uint64_t pdraw_get_peer_recording_duration(
	struct pdraw *pdraw);


int pdraw_set_peer_recording_duration(
	struct pdraw *pdraw,
	uint64_t duration);


int pdraw_get_camera_orientation_for_headtracking(
	struct pdraw *pdraw,
	float *pan,
	float *tilt);


int pdraw_get_media_count(
	struct pdraw *pdraw);


int pdraw_get_media_info(
	struct pdraw *pdraw,
	unsigned int index,
	struct pdraw_media_info *info);


void *pdraw_add_video_frame_filter_callback(
	struct pdraw *pdraw,
	unsigned int mediaId,
	pdraw_video_frame_filter_callback_t cb,
	void *userPtr);


int pdraw_remove_video_frame_filter_callback(
	struct pdraw *pdraw,
	unsigned int mediaId,
	void *filterCtx);


void *pdraw_add_video_frame_producer(
	struct pdraw *pdraw,
	unsigned int mediaId,
	int frameByFrame);


int pdraw_remove_video_frame_producer(
	struct pdraw *pdraw,
	void *producerCtx);


int pdraw_get_producer_last_frame(
	struct pdraw *pdraw,
	void *producerCtx,
	struct pdraw_video_frame *frame,
	int timeout);


float pdraw_get_controller_radar_angle_setting(
	struct pdraw *pdraw);


int pdraw_set_controller_radar_angle_setting(
	struct pdraw *pdraw,
	float angle);


int pdraw_get_display_screen_settings(
	struct pdraw *pdraw,
	float *xdpi,
	float *ydpi,
	float *deviceMargin);


int pdraw_set_display_screen_settings(
	struct pdraw *pdraw,
	float xdpi,
	float ydpi,
	float deviceMargin);


int pdraw_get_hmd_distorsion_correction_settings(
	struct pdraw *pdraw,
	enum pdraw_hmd_model *hmdModel,
	float *ipd,
	float *scale,
	float *panH,
	float *panV);

int pdraw_set_hmd_distorsion_correction_settings(
	struct pdraw *pdraw,
	enum pdraw_hmd_model hmdModel,
	float ipd,
	float scale,
	float panH,
	float panV);

int pdraw_set_jni_env
	(struct pdraw *pdraw,
	 void *jniEnv);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_PDRAW_H_ */
