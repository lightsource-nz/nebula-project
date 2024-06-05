#include <nebula.h>
#include <module/mod_light_pulse.h>
#include <light_platform.h>

//#include "nebula_internal.h"

static void nebula_event(const struct light_module *module, uint8_t event);
static uint8_t nebula_task_main(struct light_application *app);
static void nebula_set_frame_rate(uint32_t frame_rate);

void __nebula_hardware_init();

// app: nebula_po13
// defines two pico-oled-1.3 displays, one attached to the default pins
// for the display board, and one using spi port 0 and pins 16-20

Light_Application_Define(nebula, nebula_event, nebula_task_main,
                                &light_pulse,
                                &light_platform,
                                &light_framework);

static uint32_t last_run;
static uint32_t next_frame;
static uint32_t frame_interval_ms;
static uint32_t frame_counter;

static uint8_t seq_counter;
static uint8_t seq_wrap;

void main()
{
        light_framework_init();
        light_framework_run();

}

static void nebula_event(const struct light_module *module, uint8_t event)
{
        switch(event) {
        case LF_EVENT_LOAD:;
                nebula_init();
        break;
        // TODO implement unregister for event hooks
        case LF_EVENT_UNLOAD:
        break; 
        }
}
static uint8_t nebula_task_main(struct light_application *app)
{
        return nebula_main(app);
}

static void nebula_set_frame_rate(uint32_t frame_rate)
{
        if(frame_rate > 0)
                frame_interval_ms = 1000 / frame_rate;
}