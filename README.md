# ppm2joystick
A RadioControl to USB Joystick interface, to play scale plane simulators with you favorite remote.
It is made to run on Teensy controllers, using a single pin.

The code is taken from several examples, but I wanted to have it smaller and simplier.
Compared to the PPM library of Teensy, it has the advantage to not use timers, and above
all, to be very small and easy to understand.

Moreover, I added a autorange, since the peaks sampling shows variations from one peak to another.
