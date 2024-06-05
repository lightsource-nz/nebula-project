#ifndef _NEBULA_H
#define _NEBULA_H

#include <light.h>
#include <pulse.h>

#include <stdint.h>

// TODO implement version fields properly
#define NEBULA_VERSION_STR              "0.1.0"

#define NEBULA_INFO_STR                 "nebula v" NEBULA_VERSION_STR

#define _NEBULA_PIN(pin) NEBULA_PIN_ID_ ## pin
#define NEBULA_PIN(pin) _NEBULA_PIN(pin)

#define NEBULA_STATE_OFF                0
#define NEBULA_STATE_RUN                1
#define NEBULA_STATE_PAUSE              2

// TODO make display count configurable at runtime
#define NEBULA_PIN_COUNT                5

#define NEBULA_PIN_ID_RGB_RED           16
#define NEBULA_PIN_ID_RGB_GREEN         17
#define NEBULA_PIN_ID_RGB_BLUE          18
#define NEBULA_PIN_ID_SOMETHING         19
#define NEBULA_PIN_ID_PURPLE            20

#define NEBULA_PIN_RGB_RED              0
#define NEBULA_PIN_RGB_GREEN            1
#define NEBULA_PIN_RGB_BLUE             2
#define NEBULA_PIN_SOMETHING            3
#define NEBULA_PIN_PURPLE               4

extern struct pulse_pin *nebula_pin[NEBULA_PIN_COUNT];

#if(NEBULA_PIN_COUNT > NEBULA_PIN_RGB_RED)
        extern struct pulse_pin *pin_rgb_red;
#endif
#if(NEBULA_PIN_COUNT > NEBULA_PIN_RGB_GREEN)
        extern struct pulse_pin *pin_rgb_green;
#endif
#if(NEBULA_PIN_COUNT > NEBULA_PIN_RGB_BLUE)
        extern struct pulse_pin *pin_rgb_blue;
#endif
#if(NEBULA_PIN_COUNT > NEBULA_PIN_SOMETHING)
        extern struct pulse_pin *pin_something;
#endif
#if(NEBULA_PIN_COUNT > NEBULA_PIN_PURPLE)
        extern struct pulse_pin *pin_rgb_purple;
#endif

extern void __nebula_fx_init();

extern void nebula_init();
extern uint8_t nebula_get_state();
extern void nebula_set_state(uint8_t state);
extern uint8_t nebula_state_change_is_pending();
extern uint8_t nebula_get_pending_state();
extern uint8_t nebula_main(struct light_application *app);

#endif