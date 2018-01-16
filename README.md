# We Are Symphony

A theatre performance incorporating a sensor network used by performers to control lighting and sound. ESP8266 based sensors are used to output OSC messages to a PC over wifi. Lighting software on the PC then converts this OSC into Artnet lighting commands to control fixtures, LED matrices and sound.


## Sensors

### Analog input:

[gpio_osc_sender](https://github.com/cpethica/We-Are-Symphony/tree/master/gpio_osc_sender)

Sends any changes in output of the analog pin as an integer over osc with the address "/sensor".

### Magnetometer:

[gpio_osc_sender_magnetometer](https://github.com/cpethica/We-Are-Symphony/tree/master/gpio_osc_sender_magnetometer)

Outputs a compass heading from a HMC5883L compass.

### Microphone:


### Ultrasonic ranging:
