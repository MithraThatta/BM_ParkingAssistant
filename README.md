# BM_ParkingAssistant
![image](https://github.com/user-attachments/assets/5749df04-4a13-42c1-bf78-e10563743b3b)



**Summary**

An HC-SR04 ultrasonic distance sensor is paired with a simple LED strip and an STM-F103RB MCU to create a parking assistant for parking in tight, constrained spaces, like a garage. The ultrasonic distance sensor, when mounted at the end of the parking space, records and transmits the distance between the car and the end of the parking space, which is then used to turn on the appropriate LEDs, indicating when the driver can move forward and when the driver must stop. Additionally, when no movement is detected for more than 5 seconds, the LED strip is powered off, and remains off until movement is detected in order to save power. The system also features UART support for debugging and testing.

This project was a fully bare-metal project. No libraries were used outside of the cmsis library, which was used to make accessing hardware registers easier.


**Purpose**

* Develop a strong sense of familiarity with the interaction between code and hardware by abstracting away all libraries and relying solely on the lowest level code.
* Get a fundamental understanding of interrupts and how they behave with the hardware.
* Getting used to working with core components like USART and timers in a practical use scenario.
* Understanding how to read and understand key documentation, such as the reference manual, datasheet, and cortex generic user guide in order to write functions and familairize myself with the hardware.


**Features**
* Interrupt-based UART for real time troubleshooting with any device that supports serial communication.
* Input capture mode with a general purpose timer used to avoid CPU overhead causing an incorrect time to be read. 
* A simple LED strip to provide a clear indicator of how close the driver is to the end of the parking space.
* A lightweight interrupt handler and minimal memory usage to allow for further development with few constraints.


**Hardware Summary**


![image](https://github.com/user-attachments/assets/73c87c51-f1a4-4afe-b430-891da82f7eae)




