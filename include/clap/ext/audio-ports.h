﻿#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../clap.h"

#define CLAP_EXT_AUDIO_PORTS "clap/audio-ports"

typedef enum clap_audio_port_channel_mapping {
   CLAP_AUDIO_PORT_UNSPECIFIED = 0,
   CLAP_AUDIO_PORT_MONO = 1,

   // left, right
   CLAP_AUDIO_PORT_STEREO = 2,

   // front left, front right, center, low, surround left, surround right
   // surround back left, surround back right
   CLAP_AUDIO_PORT_SURROUND = 3,
} clap_audio_port_channel_mapping;

typedef struct clap_audio_port_info {
   char name[CLAP_NAME_SIZE]; // displayable name
   bool is_main;              // there can only be 1 main input and output
   bool is_cv;                // control voltage
   bool supports_64_bits;     // 32 bit support is mandatory, the host chooses
                              // between 32 and 64.
   bool supports_in_place; // if true the daw can use the same buffer for input
                           // and output, only for main input to main output
   int32_t                         channel_count;
   clap_audio_port_channel_mapping channel_mapping;
} clap_audio_port_info;

// The audio ports scan has to be done while the plugin is deactivated.
typedef struct clap_plugin_audio_ports {
   // number of ports, for either input or output
   // [main-thread]
   int32_t (*get_count)(clap_plugin *plugin, bool is_input);

   // get info about about an audio port.
   // [main-thread]
   void (*get_info)(clap_plugin *         plugin,
                    int32_t               index,
                    bool                  is_input,
                    clap_audio_port_info *info);

   void (*activate_port)(clap_plugin *plugin,
                         int32_t      index,
                         bool         is_input,
                         bool         use_64);
   void (*deactivate_port)(clap_plugin *plugin, int32_t index, bool is_input);

   // Returns the port latency.
   // [main-thread]
   int32_t (*get_latency)(clap_plugin *plugin, int32_t index, bool is_input);
} clap_plugin_audio_ports;

typedef struct clap_host_audio_ports {
   // Tell the host that the plugin ports has changed.
   // The host shall deactivate the plugin and then scan the ports again.
   // [main-thread]
   void (*changed)(clap_host *host, clap_plugin *plugin);

   // Tell the host that the latency changed.
   // The host should call get_port_latency on each ports.
   // [main-thread]
   void (*latency_changed)(clap_host *host, clap_plugin *plugin);
} clap_host_audio_ports;

#ifdef __cplusplus
}
#endif
