#ifndef __amr_h__
#define __amr_h__

#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_common.h"
#include "audio_hal.h"
#include "fatfs_stream.h"
#include "i2s_stream.h"
#include "esp_peripherals.h"
#include "periph_sdcard.h"
#include "amrwb_encoder.h"
#include "amrnb_encoder.h"

struct amr_config
{
    int second_recorded;
    audio_event_iface_handle_t evt;
    audio_pipeline_handle_t pipeline;
	audio_element_handle_t amr_encoder;
	audio_element_handle_t i2s_stream_reader;
	audio_element_handle_t fatfs_stream_writer;
};

struct amr_config amr_setup(void);
bool amr_process(struct amr_config *config);
void amr_cleaning_up(struct amr_config *config);

#endif
