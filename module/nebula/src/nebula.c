#include <nebula.h>
#include <light_platform.h>

//#include "nebula_internal.h"

static void nebula_set_frame_rate(uint32_t frame_rate);

static uint8_t nebula_pin_id[] = {
        NEBULA_PIN_ID_RGB_RED,
        NEBULA_PIN_ID_RGB_GREEN,
        NEBULA_PIN_ID_RGB_BLUE,
        NEBULA_PIN_ID_SOMETHING,
        NEBULA_PIN_ID_PURPLE
};

struct pulse_pin *nebula_pin[NEBULA_PIN_COUNT];

#if(NEBULA_PIN_COUNT > NEBULA_PIN_RGB_RED)
        struct pulse_pin *pin_rgb_red;
#endif
#if(NEBULA_PIN_COUNT > NEBULA_PIN_RGB_GREEN)
        struct pulse_pin *pin_rgb_green;
#endif
#if(NEBULA_PIN_COUNT > NEBULA_PIN_RGB_BLUE)
        struct pulse_pin *pin_rgb_blue;
#endif
#if(NEBULA_PIN_COUNT > NEBULA_PIN_SOMETHING)
        struct pulse_pin *pin_something;
#endif
#if(NEBULA_PIN_COUNT > NEBULA_PIN_PURPLE)
        struct pulse_pin *pin_rgb_purple;
#endif

static uint32_t last_run;
static uint32_t next_frame;
static uint32_t frame_interval_ms;
static uint32_t frame_counter;

static uint8_t seq_counter;
static uint8_t seq_wrap;

static uint8_t _nebula_state;
static uint8_t _nebula_pending_state;
static volatile uint8_t _nebula_flag_state_changed;

uint8_t nebula_state()
{
        return _nebula_state;
}
#define STATE_ALLOW             1
#define STATE_DENY              0
// right now all possible state transitions are permitted
static uint8_t _state_transition_permitted(uint8_t old_state, uint8_t new_state)
{
        switch (old_state) {
        case NEBULA_STATE_OFF:
                return STATE_ALLOW;
        case NEBULA_STATE_RUN:
                return STATE_ALLOW;
        case NEBULA_STATE_PAUSE:
                return STATE_ALLOW;
        }
        return STATE_DENY;
}
static void _state_change_commit(uint8_t new_state)
{
        uint8_t old_state = _nebula_state;
        _nebula_state = new_state;
        _nebula_flag_state_changed = false;

        switch (new_state)
        {
        case NEBULA_STATE_RUN:
                for(uint8_t i = 0; i < NEBULA_PIN_COUNT; i++) {
                        if(old_state == NEBULA_STATE_OFF) {
                                pulse_pin_enable(nebula_pin[i]);
                        }
                        pulse_pin_resume(nebula_pin[i]);
                }
                gpio_init(25);
                gpio_set_dir(25, true);
                gpio_put(25, true);
                break;
        case NEBULA_STATE_PAUSE:
                for(uint8_t i = 0; i < NEBULA_PIN_COUNT; i++) {
                        pulse_pin_pause(nebula_pin[i]);
                }
                break;
        case NEBULA_STATE_OFF:
                for(uint8_t i = 0; i < NEBULA_PIN_COUNT; i++) {
                        pulse_pin_disable(nebula_pin[i]);
                }
                break;

        }
}
static void _do_state_transition()
{
        if(_nebula_flag_state_changed) {
                // if new state is invalid, pending state change is canceled
                if(!_state_transition_permitted(_nebula_state, _nebula_pending_state)) {
                        _nebula_pending_state = _nebula_state;
                        _nebula_flag_state_changed = false;
                        return;
                }
                _state_change_commit(_nebula_pending_state);
        }
}
void nebula_init()
{
        _nebula_state = NEBULA_STATE_OFF;
        for(uint8_t i = 0; i < NEBULA_PIN_COUNT; i++) {
                nebula_pin[i] = pulse_pin_new(nebula_pin_id[i]);
        }
        
#if(NEBULA_PIN_COUNT > NEBULA_PIN_RGB_RED)
        pin_rgb_red = nebula_pin[NEBULA_PIN_RGB_RED];
#endif
#if(NEBULA_PIN_COUNT > NEBULA_PIN_RGB_GREEN)
        pin_rgb_green = nebula_pin[NEBULA_PIN_RGB_GREEN];
#endif
#if(NEBULA_PIN_COUNT > NEBULA_PIN_RGB_BLUE)
        pin_rgb_blue = nebula_pin[NEBULA_PIN_RGB_BLUE];
#endif
#if(NEBULA_PIN_COUNT > NEBULA_PIN_SOMETHING)
        pin_something = nebula_pin[NEBULA_PIN_SOMETHING];
#endif
#if(NEBULA_PIN_COUNT > NEBULA_PIN_PURPLE)
        pin_rgb_purple = nebula_pin[NEBULA_PIN_PURPLE];
#endif

        light_debug("passing control to fx driver setup function","");
        __nebula_fx_init();
        light_info("fx driver setup complete","");

        nebula_set_state(NEBULA_STATE_RUN);
}
void nebula_set_state(uint8_t state)
{
        _nebula_pending_state = state;
        _nebula_flag_state_changed = 1;
}
uint8_t nebula_state_change_is_pending()
{
        return _nebula_flag_state_changed;
}
uint8_t nebula_get_pending_state()
{
        return _nebula_pending_state;
}
uint8_t nebula_main(struct light_application *app)
{
        uint32_t now = light_platform_get_system_time_ms();
        light_info("enter nebula application task, time=%dms, time since last run=%dms", now, last_run - now);

        if(nebula_state_change_is_pending()) {
                _do_state_transition();
        }

        if(now >= next_frame) {
                next_frame += frame_interval_ms;
                frame_counter++;
                seq_counter++;
                seq_counter %= seq_wrap;
                for(uint8_t i = 0; i < NEBULA_PIN_COUNT; i++) {
                        
                }
        }

        last_run = now;
        return LF_STATUS_RUN;
}

static void nebula_set_frame_rate(uint32_t frame_rate)
{
        if(frame_rate > 0)
                frame_interval_ms = 1000 / frame_rate;
}
