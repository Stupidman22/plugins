// Copyright 2023 Samsung Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_PLUGIN_MEDIA_PLAYER_PROXY_H_
#define FLUTTER_PLUGIN_MEDIA_PLAYER_PROXY_H_

#include <player.h>

#define MAX_STRING_NAME_LEN 255
#define MMPLAYER_FOUR_CC_LEN 14
#define PLAYER_LANG_NAME_SIZE 10

typedef struct {
  char fourCC[MMPLAYER_FOUR_CC_LEN + 1]; /**< codec fourcc */
  char name[MAX_STRING_NAME_LEN]; /**< name: video/audio, it maybe not exit in
                                     some track*/
  /*dynamic infos in hls,ss,dash streams*/
  int width;    /**< resolution width */
  int height;   /**< resolution height */
  int bit_rate; /**< bitrate in bps */
} player_video_track_info_v2;

typedef struct {
  char fourCC[MMPLAYER_FOUR_CC_LEN + 1]; /**< codec fourcc */
  char language[PLAYER_LANG_NAME_SIZE];  /**< language info*/
  /*dynamic infos in hls,ss,dash streams*/
  int sample_rate; /**< sample rate in this track*/
  int channel;     /**< channel in this track*/
  int bit_rate;    /**< bitrate  in this track*/
} player_audio_track_info_v2;

typedef struct {
  char fourCC[MMPLAYER_FOUR_CC_LEN + 1]; /**< codec fourcc */
  char language[PLAYER_LANG_NAME_SIZE];  /**< language info*/
  int subtitle_type; /**< text subtitle = 0, picture subtitle = 1 */
} player_subtitle_track_info_v2;

typedef enum {
  PLAYER_DRM_TYPE_NONE = 0,
  PLAYER_DRM_TYPE_PLAYREADY,
  PLAYER_DRM_TYPE_MARLIN,
  PLAYER_DRM_TYPE_VERIMATRIX,
  PLAYER_DRM_TYPE_WIDEVINE_CLASSIC,
  PLAYER_DRM_TYPE_SECUREMEDIA,
  PLAYER_DRM_TYPE_SDRM,
  PLAYER_DRM_TYPE_VUDU,
  PLAYER_DRM_TYPE_WIDEVINE_CDM,
  PLAYER_DRM_TYPE_AES128,
  PLAYER_DRM_TYPE_HDCP,
  PLAYER_DRM_TYPE_DTCP,
  PLAYER_DRM_TYPE_SCSA,
  PLAYER_DRM_TYPE_CLEARKEY,
  PLAYER_DRM_TYPE_EME,
  PLAYER_DRM_TYPE_MAX_COUNT,
} player_drm_type_e;

typedef enum {
  CENC = 0,
  KEYIDS = 1,
  WEBM = 2,
} drm_init_data_type;

typedef bool (*security_init_complete_cb)(int* drmhandle, unsigned int length,
                                          unsigned char* psshdata,
                                          void* user_data);
typedef int (*set_drm_init_data_cb)(drm_init_data_type init_type, void* data,
                                    int data_length, void* user_data);

class MediaPlayerProxy {
 public:
  MediaPlayerProxy();
  ~MediaPlayerProxy();
  int player_set_ecore_wl_display(player_h player, player_display_type_e type,
                                  void* ecore_wl_window, int x, int y,
                                  int width, int height);
  int player_set_drm_handle(player_h player, player_drm_type_e drm_type,
                            int drm_handle);
  int player_set_drm_init_complete_cb(player_h player,
                                      security_init_complete_cb callback,
                                      void* user_data);
  int player_set_drm_init_data_cb(player_h player,
                                  set_drm_init_data_cb callback,
                                  void* user_data);
  int player_get_track_count_v2(player_h player, player_stream_type_e type,
                                int* pcount);
  int player_get_video_track_info_v2(player_h player, int index,
                                     player_video_track_info_v2** track_info);
  int player_get_audio_track_info_v2(player_h player, int index,
                                     player_audio_track_info_v2** track_info);
  int player_get_subtitle_track_info_v2(
      player_h player, int index, player_subtitle_track_info_v2** track_info);

 private:
  void* media_player_handle_ = nullptr;
};

#endif  // FLUTTER_PLUGIN_MEDIA_PLAYER_PROXY_H_
