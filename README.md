Starting from the problem that children's bionic-hands are not currently on the market, this program was designed to produce bionic-hands using EMG sensors.

This Arduino program reads signals from an EMG (electromyography) sensor to control a servo motor. When the program starts, it measures the sensor values for about 5 seconds while the muscle is relaxed and calculates the average. It then sets a threshold by adding 40 to this average value. The servo motor will respond whenever the sensor reading goes above this threshold.

After calibration, when you contract your muscle and the sensor value exceeds the threshold, the servo motor smoothly moves between 0 degrees and 180 degrees, switching its position. To reduce noise, the program uses a moving average filter that averages the last 10 sensor readings. It also includes a debounce delay of 500 milliseconds to prevent the servo from moving too frequently.

The servo’s position toggles between 0 and 180 degrees depending on its current angle, moving in small steps for smooth motion. When the sensor value falls back below the threshold minus a small margin, the system resets to wait for the next muscle contraction.

This project is useful for creating simple muscle-controlled devices using EMG sensors and servo motors. The hardware connections are straightforward: connect the EMG sensor output to Arduino’s analog input A0 and the servo signal wire to digital pin 9. The serial monitor shows real-time sensor values and system status, which helps with debugging.

Since bionic-hands using this program can be produced by 3D printing, it can be used by all ages, and the price is expected to be cheaper than other bionic-hands on the market. In addition, depending on the type of filament used for 3D printing, effects such as waterproof, dustproof, and insulation can be expected.
