# Seven Segment Clock

This is the code for a seven-segment clock using servo motors to control each segment. Each digit is represented by a set of servo motors that can be positioned to display numbers from 0 to 9.


The clock was designed in Fusion 360, the model can be viewed [here](https://a360.co/4phq3kS). 

## Hardware

- Arduino Board - I'm using an Arduino Nano
- Two Servo Drivers - [PCA9685 16-channel 12-bit PWM/Servo Driver](https://www.adafruit.com/product/815)
- 28 Servos - I'm using [micro servos](https://www.amazon.com/Micro-Helicopter-Airplane-Remote-Control/dp/B072V529YD/ref=sr_1_1_sspa?dib=eyJ2IjoiMSJ9.xm7TmOlDajP7EM-GGCnwv6mwTrZqJCnsuODSG7ZGMEqelyTTt6MaN8Yy9F8nmpHEZpEdae8AKcNaTiFWZWl2IttJaAmm3WWafmeC23cXS4J2nYuWBos0VbsdskxRb1IPhVAoJCAYiKxifoURIOyVsPe5af0e0crdqgJLCxWknbem9dqDxmbCGui5aEk6v0PMsqox77Bb0mNtDbHs6tbZ0mDWpA0T2nausxnsCpMk1foKgRWRmbKSWM-1Yg2ExAOy9-9MmPMifHvXYzdnOheNAFt1ENlyB-OxKAM9gbugb0c.RvdrMCUl60S-_8XdF_T5-xsvajVLq-ItzRWcJB5U_EU&dib_tag=se&hvadid=557529005899&hvdev=c&hvexpln=0&hvlocphy=1028288&hvnetw=g&hvocijid=1105650523003058241--&hvqmt=e&hvrand=1105650523003058241&hvtargid=kwd-301515237560&hydadcr=4096_13167905&keywords=small+servo+motors&mcid=997a8602092c31189c4ef30dfa5cfb1a&qid=1766857561&sr=8-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1)
- Arduino Clock Module - [DS3231 Real Time Clock Module](https://www.adafruit.com/product/3013)
- PIR Motion Sensor - [PIR Motion Sensor](https://www.adafruit.com/product/189?srsltid=AfmBOooz8fwJoHQtVMkibmmiHgaTkzhLsghd1Quh0FGFbrsOfAKL8_XWZPE)

## 3D Prints

The 3D models for the seven-segment display and servo mounts can be found in the `stl` directory. You will need to print the following parts:

- `3_Holder.stl` - 4 pieces
- `4_Segment.stl` - 4 pieces
- `base.stl` - 28 pieces
- `rotate.stl` - 28 pieces
- `Dot.stl` - 2 pieces