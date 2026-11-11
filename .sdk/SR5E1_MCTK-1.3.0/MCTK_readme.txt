      SR5E1x Motor Control Toolkit 1.3.0 (Beta) 
The SR5E1x Motor Control tool provides developers with the configuration
tool (integrated in Stellar Studio tool) and firmware libraries
needed to build Applications that drive Permanent Magnets Synchronous
electric Motors with SR5E1x MCU.
Features:
- Single or simultaneous Dual (n/a) PMSM FOC sensorless / sensored
- Speed feedback:
- Sensorless State Observer, PLL rotor speed/angle computation
from B-EMF
- 60 or 120 degrees displaced Hall sensors decoding, rising/falling
edge responsiveness
- Quadrature incremental encoder
- Resolver
- Current sampling methods:
- ICS/Two shunts (selectable on the Legs or on the Phases)
- Single, common DC-link shunt resistor (ST patented). (N/A;
will be available in the next release)
- Three-shunt resistors placed on the bottom of the three inverter
legs. (N/A; will be available in the next release)
- Torque control mode, speed control mode
- Optimized I-PMSM and SM-PMSM drive
- Programmable speed ramps (parameters duration and final target)
- Programmable torque ramps (parameters duration and final target)
- Real-time fine tuning of:
- PID regulators
- Flux weakening algorithm. (N/A; will be available in the next
release)
- Start-up procedure (in case of Encoder or sensorless)
- Fault conditions management:
- Over-current (only from external sensor)
- Over and Under-voltage
- Over-temperature
- Speed feedback reliability error
- FOC algorithm execution overrun
- User Interface: Serial and CAN Communication support
- Easy customization of options, pin-out assignments, CPU clock
frequency through Stellar Studio GUI