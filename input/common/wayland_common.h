/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2011-2017 - Daniel De Matteis
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WAYLAND_INPUT_COMMON_H__
#define WAYLAND_INPUT_COMMON_H__

#include <stdint.h>
#include <boolean.h>

#include <linux/input.h>
#include <wayland-client.h>
#include <wayland-cursor.h>

#include "../input_driver.h"

#ifdef HAVE_EGL
#include "../../gfx/common/egl_common.h"
#endif

#ifdef HAVE_VULKAN
#include "../../gfx/common/vulkan_common.h"
#endif

#define UDEV_KEY_MAX			0x2ff
#define UDEV_MAX_KEYS (UDEV_KEY_MAX + 7) / 8

#define MAX_TOUCHES             16

#define WL_ARRAY_FOR_EACH(pos, array, type) \
	for (pos = (type)(array)->data; \
	     (const char *) pos < ((const char *) (array)->data + (array)->size); \
	     (pos)++)

typedef struct
{
   bool active;
   int16_t x;
   int16_t y;
} wayland_touch_data_t;

typedef struct touch_pos
{
   bool active;
   int32_t id;
   unsigned x;
   unsigned y;
} touch_pos_t;

typedef struct output_info
{
   struct wl_output *output;
   uint32_t global_id;
   unsigned width;
   unsigned height;
   unsigned physical_width;
   unsigned physical_height;
   int refresh_rate;
   unsigned scale;
   struct wl_list link; /* wl->all_outputs */
} output_info_t;

typedef struct input_ctx_wayland_data
{
   /* Wayland uses Linux keysyms. */
   uint8_t key_state[UDEV_MAX_KEYS];
   bool keyboard_focus;

   struct
   {
      int last_x, last_y;
      int x, y;
      int delta_x, delta_y;
      bool last_valid;
      bool focus;
      bool left, right, middle;
   } mouse;

   struct wl_display *dpy;
   int fd;

   const input_device_driver_t *joypad;
   bool blocked;

   wayland_touch_data_t touches[MAX_TOUCHES];

} input_ctx_wayland_data_t;

typedef struct gfx_ctx_wayland_data
{
#ifdef HAVE_EGL
   egl_ctx_data_t egl;
   struct wl_egl_window *win;
#endif
   bool fullscreen;
   bool maximized;
   bool resize;
   bool configured;
   bool activated;
   unsigned prev_width;
   unsigned prev_height;
   unsigned width;
   unsigned height;
   struct wl_registry *registry;
   struct wl_compositor *compositor;
   struct wl_surface *surface;
   struct zxdg_surface_v6 *zxdg_surface;
   struct zxdg_shell_v6 *zxdg_shell;
   struct zxdg_toplevel_v6 *zxdg_toplevel;
   struct xdg_surface *xdg_surface;
   struct xdg_wm_base *xdg_shell;
   struct xdg_toplevel *xdg_toplevel;
   struct wl_keyboard *wl_keyboard;
   struct wl_pointer  *wl_pointer;
   struct wl_touch *wl_touch;
   struct wl_seat *seat;
   struct wl_shm *shm;
   struct zxdg_decoration_manager_v1 *deco_manager;
   struct zxdg_toplevel_decoration_v1 *deco;
   struct zwp_idle_inhibit_manager_v1 *idle_inhibit_manager;
   struct zwp_idle_inhibitor_v1 *idle_inhibitor;
   struct wl_list all_outputs;
   output_info_t *current_output;
   int swap_interval;
   bool core_hw_context_enable;

   unsigned last_buffer_scale;
   unsigned buffer_scale;

   struct
   {
      struct wl_cursor *default_cursor;
      struct wl_cursor_theme *theme;
      struct wl_surface *surface;
      uint32_t serial;
      bool visible;
   } cursor;

   input_ctx_wayland_data_t input;

#ifdef HAVE_VULKAN
   gfx_ctx_vulkan_data_t vk;
#endif
   int num_active_touches;
   touch_pos_t active_touch_positions[MAX_TOUCHES];
} gfx_ctx_wayland_data_t;

#ifdef HAVE_XKBCOMMON
/* FIXME: Move this into a header? */
int init_xkb(int fd, size_t size);
int handle_xkb(int code, int value);
void handle_xkb_state_mask(uint32_t depressed,
      uint32_t latched, uint32_t locked, uint32_t group);
void free_xkb(void);
#endif

extern const struct wl_keyboard_listener keyboard_listener;

extern const struct wl_pointer_listener pointer_listener;

extern const struct wl_touch_listener touch_listener;

extern const struct wl_seat_listener seat_listener;

#endif
