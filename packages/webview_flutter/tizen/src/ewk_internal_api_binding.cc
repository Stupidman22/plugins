// Copyright 2022 Samsung Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ewk_internal_api_binding.h"

#include <EWebKit.h>
#include <dlfcn.h>

#include <string>

EwkInternalApiBinding::EwkInternalApiBinding() {
  handle_ = dlopen("libchromium-ewk.so", RTLD_LAZY);
}

EwkInternalApiBinding::~EwkInternalApiBinding() {
  if (handle_) {
    dlclose(handle_);
  }

  if (handle_impl_) {
    dlclose(handle_impl_);
  }
}

void EwkInternalApiBinding::EwkImpleDlopen() {
  std::string path[6] = {
      "/usr/apps/org.tizen.chromium-efl",
      "/opt/usr/globalapps/org.tizen.chromium-efl",
      "/opt/usr/globalapps/org.tizen.chromium-efl-upgrade",
      "/usr/share/chromium-efl/update",
      "/usr/share/chromium-efl/",
      "/usr/share/chromium-efl/upgrade",
  };

  for (int i = 0; i < 6; i++) {
    std::string p = path[i] + "/lib/libchromium-impl.so";
    handle_impl_ = dlopen(p.c_str(), RTLD_LAZY);
    if (!handle_impl_) {
      log += "\n load fail " + p + "\n";
    } else {
      log += "\n load success " + p + "\n";
      break;
    }
  }
}

void* EwkInternalApiBinding::EwkImplDlsym(const char* function_name) {
  if (!handle_impl_) return nullptr;

  void* function_addr = dlsym(handle_impl_, function_name);

  return function_addr;
}

bool EwkInternalApiBinding::Initialize() {
  if (!handle_) {
    return false;
  }

  // ewk_view
  view.SetBackgroundColor = reinterpret_cast<EwkViewBgColorSetFnPtr>(
      dlsym(handle_, "ewk_view_bg_color_set"));
  view.FeedTouchEvent = reinterpret_cast<EwkViewFeedTouchEventFnPtr>(
      dlsym(handle_, "ewk_view_feed_touch_event"));
  view.SendKeyEvent = reinterpret_cast<EwkViewSendKeyEventFnPtr>(
      dlsym(handle_, "ewk_view_send_key_event"));
  view.OffscreenRenderingEnabledSet =
      reinterpret_cast<EwkViewOffscreenRenderingEnabledSetFnPtr>(
          dlsym(handle_, "ewk_view_offscreen_rendering_enabled_set"));
  view.ImeWindowSet = reinterpret_cast<EwkViewImeWindowSetFnPtr>(
      dlsym(handle_, "ewk_view_ime_window_set"));
  view.KeyEventsEnabledSet = reinterpret_cast<EwkViewKeyEventsEnabledSetFnPtr>(
      dlsym(handle_, "ewk_view_key_events_enabled_set"));
  view.SupportVideoHoleSet = reinterpret_cast<EwkViewSupportVideoHoleSetFnPtr>(
      dlsym(handle_, "ewk_view_set_support_video_hole"));
  view.AddWithContext = reinterpret_cast<EwkViewAddWithContextFnPtr>(
      dlsym(handle_, "ewk_view_add_with_context"));

  // ewk_main
  main.SetArguments = reinterpret_cast<EwkSetArgumentsFnPtr>(
      dlsym(handle_, "ewk_set_arguments"));

  main.SetVersionPolicy = reinterpret_cast<EwkSetVersionPolicyFnPtr>(
      dlsym(handle_, "ewk_set_version_policy"));

  // ewk_settings
  settings.ImePanelEnabledSet =
      reinterpret_cast<EwkSettingsImePanelEnabledSetFnPtr>(
          dlsym(handle_, "ewk_settings_ime_panel_enabled_set"));

  settings.ContextDefaultGet = reinterpret_cast<EwkContextDefaultGetFnPtr>(
      dlsym(handle_, "ewk_context_default_get"));

  settings.ContextNew =
      reinterpret_cast<EwkContextNewFnPtr>(dlsym(handle_, "ewk_context_new"));

  // ewk_console_message
  console_message.LevelGet = reinterpret_cast<EwkConsoleMessageLevelGetFnPtr>(
      dlsym(handle_, "ewk_console_message_level_get"));
  console_message.TextGet = reinterpret_cast<EwkConsoleMessageTextGetFnPtr>(
      dlsym(handle_, "ewk_console_message_text_get"));
  console_message.LineGet = reinterpret_cast<EwkConsoleMessageLineGetFnPtr>(
      dlsym(handle_, "ewk_console_message_line_get"));
  console_message.SourceGet = reinterpret_cast<EwkConsoleMessageSourceGetFnPtr>(
      dlsym(handle_, "ewk_console_message_source_get"));

  return view.SetBackgroundColor && view.FeedTouchEvent && view.SendKeyEvent &&
         view.OffscreenRenderingEnabledSet && view.ImeWindowSet &&
         view.KeyEventsEnabledSet && view.SupportVideoHoleSet &&
         view.AddWithContext && main.SetArguments && main.SetVersionPolicy &&
         settings.ImePanelEnabledSet && console_message.LevelGet &&
         console_message.TextGet && console_message.LineGet &&
         console_message.SourceGet;
}
