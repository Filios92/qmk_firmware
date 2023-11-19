# TODO
config.h, mcuconf.h and halconf.h can be deleted from here.
The only values that are not in `platforms/chibios/boards/QMK_PM2040/configs/` is:
- `#define SERIAL_PIO_USE_PIO1`
and it shouldn't have much impact. After these files removal, compilation fails, because PINs in QMK_PM2040 are for AVR and not RP2040. (need somehow to go around that)

And the only config to be added, which is in coverter to rp2040_ce and is not in QMK_PM2040 is:
- `OPT_DEFS += -DUSB_VBUS_PIN=19U`
