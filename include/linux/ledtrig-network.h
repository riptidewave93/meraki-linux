#ifndef _LEDTRIG_NETWORK_H_
#define _LEDTRIG_NETWORK_H_

extern char led_network_activity;
void record_led_network_activity(char port_mask) {
    led_network_activity |= port_mask;
}

#endif
