# Guiding Glow: Lightweight Emotional Communication Device




## Table of Contents
- [Introduction](#introduction)
  - [Background](#background)
  - [Motivation](#motivation)
  - [Aims and Objectives](#aims-and-objectives)
  - [Existing Products and Design Space](#existing-products-and-design-space)
- [Literature Review](#literature-review)
- [Appearance Design & Iteration](#appearance-design--iteration)
  - [First Prototype: Cylindrical Box with Heart Motifs](#first-prototype-cylindrical-box-with-heart-motifs)
  - [Second Prototype: Cloud-Shaped Design](#second-prototype-cloud-shaped-design)
  - [Final Concept: Lighthouse and Boat](#final-concept-lighthouse-and-boat)
  - [Final Design Enhancements](#final-design-enhancements)
- [Methodology](#methodology)
  - [Introduction](#introduction)
  - [Software Design](#software-design)
  - [Hardware Material Selection and System Design](#hardware-material-selection-and-system-design)
- [Conclusions and Future Work](#conclusions-and-future-work)
- [References](#references)

---
word counts: 2116

> GitHub: [CASA0021Group5]( https://github.com/CASA0021Group5/GPP)

---

## 1. Introduction

### 1.1 Background

As globalisation continues to deepen, more young people are choosing to study or reside overseas. Evidence of this deepening globalisation can be seen in the rising number of international students in the United Kingdom. Research by Bolton, Hubble and Gower (2024) reveals a 10th record high of 760,000 Internataional students during 2022/2023 academic year. However, such decisions inevitably lead to prolonged separation between family members, posing significant challenges to emotional bonds, communication and mental well-being.

International students frequently experience loneliness, anxiety, and social isolation when adapting to unfamiliar environments, largely due to cultural differences, language barriers, and limited local support (Zhang & Goodson, 2011). Meanwhile, parents frequently suffer from constant worry due to the lack of regular updates about their children’s daily lives, particularly concerning travel safety (Ye, 2006). While digital tools like instant messaging and video calls make cross-border communication more accessible, they often fail to provide a true sense of presence. Time-zone gaps, schedule mismatches, and emotional fatigue can hinder sustained interaction. Furthermore, the pressure of cosntant active communication can impose an emotional burden on both parties, potentially leading to interaction fatigue. Therefore, there is a growing need for lightweight, low-effort communication methods that convey a sense of emotional presence while integrating seamlessly into everyday life.


### 1.2 Motivation

Surveys indicate that parents generally prefer simple and intuitive ways to stay informed about their children's safety. Whereas young adults prefer to avoid the psychological burden caused by frequent, active communication (Neustaedter & Greenberg, 2012). These practical needs have shaped the design toward a communication system that minimises intrusive interaction and integrates naturally into daily routines, enabling both sides to subtly maintain emotional bonds.

<p align="center">
  <img src="/Final Report/figures/F1.png"/>
   <br>
  <em>Fig. 1 - storyboard</em>
</p>

### 1.3 Aims and Objectives

The primary aim of Guiding Glow is to create a tangible device that fosters emotional closeness among physically separated family members. Using the metaphor of a boat and a lighthouse, the system creates an intuitive and natural interaction: when a user returns home and places their key on the boat element, the lighthouse on the paired device lights up, offering a non-verbal yet reassuring signal of presence.

From a technical perspective, the project aims to develop a low-cost, low-power system that enables real-time bidirectional communication. Its interaction is based on simple, habitual actions and emphasizes usability, visual expression, and seamless integration into everyday routines, with a particular focus on non-verbal emotional communication. Within the broader context of Connected Environments, the project aims to create a screen- free, feasible and scalable prototype for emotionally responsive interaction design. By using a physically interactive Force Sensitive Resistor (FSR) and LEDs our allows for subtle, non-verbal and ambient communication. 


### 1.4 Existing Products and Design Space

Most emotional communication devices on the market are designed for intimate relationships, particularly between couples or close friends. Examples include the Bond Touch bracelet, which simulates touch through vibrations. While it is effective in facilitating remote emotional expression, these products share two common limitations: reliance on smartphone connectivity and explicit interaction patterns that are not easily integrated into daily life.

In contrast, emotionally supportive technologies tailored to family relationships—especially intergenerational ties such as those between parents and children—remain significantly underexplored. Research suggests that family members often prefer low-intervention communication methods that naturally embed into daily life, helping to reduce the psychological strain associated with constant updates (Bacigalupe & Brauninger, 2017).

Guiding Glow responds to this gap by focusing on familial emotional connection through metaphorical physical elements—a “boat” and a “lighthouse”—which evoke themes of care, guidance, and belonging. The device adopts a deliberately simple and unobtrusive interaction model that aligns with routine behaviours, aiming to foster ambient emotional awareness without requiring deliberate action. By taking this design approach, Guiding Glow differentiates itself from existing solutions, offering a low-effort, emotionally nuanced alternative for family communication.


<p align="center">
  <img src="/Final Report/figures/F2.png"/>
   <br>
  <em>Fig. 2 - workflow</em>
</p>

---

## 2. Literature Review

According to the Interlink Electronics (2010) datasheet, Figure 1 illustrates the output voltage response of the FSR406 sensor under different applied forces when configured with a 10 kΩ voltage divider resistor. Within the sub-300 g pressure range, particularly in the 0–100 g interval, the voltage output shows appreciable sensitivity, indicating good resolution in response to small force variations. This makes the configuration well-suited for detecting minor pressures exerted by lightweight objects such as keychains.

The comparative analysis of output voltage curves for varying load resistor values reveals that more minor resistances (e.g., 3 kΩ) yield lower initial voltages and broader dynamic ranges but suffer from reduced sensitivity in low-pressure regions. In contrast, larger resistance values (e.g., 100 kΩ) enhance sensitivity at lower force levels but risk early saturation under higher loads. The 10 kΩ resistor thus offers a balanced trade-off between sensitivity and measurable range, making it a recommended choice for detecting the gravitational force generated by small objects like keys.


<p align="center">
  <img src="/Final Report/figures/F3.png"/>
   <br>
  <em>Fig. 3 - Output Voltage vs. Applied Force for Different Load Resistor Values Using Interlink FSR.</em>
</p>

The Force-Sensitive Resistor (FSR) used in this project functions as an input sensor capable of producing digital output signals. Accordingly, the central processing unit must be capable of acquiring and interpreting digital signals while also supporting analog signal input for potential sensor expansion. Additionally, it must facilitate remote communication and be able to output digital control signals to drive a dashboard interface.

The ESP32-DROOM-32D module meets all these functional requirements. It provides analog input and digital output capabilities and supports dual-mode wireless communication through Wi-Fi (802.11 b/g/n) and Bluetooth (Classic and Low Energy). The module is powered via USB and is fully compatible with multiple development platforms, including Arduino IDE, ESP-IDF, and MicroPython, offering a flexible and well-supported environment for embedded system development (Espressif Systems, 2023)

---

## 3. Appearance Design & Iteration

Before finalising the lighthouse concept for Guiding Glow, several prototypes were developed and discarded. The initial approach attempted to link universal symbols to a parent–child relationship, but this lacked a meaningful story and compromised functionality and usability.

### 3.1 First Prototype: Cylindrical Box with Heart Motifs

The first prototype was a simple cylindrical box decorated with heart motifs. Although quickly abandoned for being too plain and generic, it introduced a key feature for future iterations — placing keys on top of the device, establishing its tabletop form factor.

### 3.2 Second Prototype: Cloud-Shaped Design

The second iteration explored a cloud-shaped design intended to incorporate text elements. However, the idea was dropped due to difficulties in execution and a lack of deeper conceptual connection. 

<p align="center">
  <img src="/Final Report/figures/F4.png"/>
   <br>
  <em>Fig. 4 - 1st and 2nd prototype</em>
</p>

### 3.3 Final Concept: Lighthouse and Boat

In the third iteration, the storytelling aspect was finalised. Drawing inspiration from Dr Kenneth Ginsburg’s Raising Kids to Thrive (Twinkl, 2025), the team adopted the lighthouse metaphor — parents as distant guides providing support when needed. This narrative aligned well with the idea of a student abroad being guided across the ocean by their parents.

The initial lighthouse prototype featured a wooden base, which was later removed to maintain material consistency between the lighthouse and the boat, and to reduce production complexity and cost. The boat element was refined as the platform for key placement, enhancing thematic immersion.


<p align="center">
  <img src="/Final Report/figures/F5.png"/>
   <br>
  <em>Fig. 5 - Final concept</em>
</p>

### 3.4 Final Design Enhancements

The lighthouse structure was scaled up by 1.5× to accommodate larger sets of keys. LED strips were integrated to simulate the sweeping light effect of a real lighthouse. A minimalist colour scheme was chosen to achieve a sleek, modern look. The enlarged boat size also addressed the practical needs of parents who often carry more keys. 

<p align="center">
  <img src="/Final Report/figures/F6.png"/>
   <br>
  <em>Fig. 6 - Final Design</em>
</p>
 Overall, the lighthouse design successfully bridged physical and emotional distance, turning the device into a symbolic, ambient guide between parent and child.

---

## 4. Methodology

### 4.1 Software Design

Development strategy, problems encountered, setbacks in the development cycle, methods used and technical details. Strength test and reliability test, test method and test results, preferably with data.

### 4.2 Software Design

This project implements a system based on the ESP32 board, integrating WI-FI configuration, MQTT communication, and functions such as pressure triggering and LED feedback, enabling real-time, long-distance communication.

**WI-FI Configuration Function**

To avoid reprogramming when network settings change, the system provides WI-FI configuration via a web interface. If WI-FI connection fails, the ESP32 enters AP mode, hosting a local server and using DNS redirection to present a user-friendly configuration page. Users can enter WI-FI credentials, which are stored in non-volatile storage, allowing automatic reconnection after reboot.

**MQTT Communication**

After connecting to WI-FI, the ESP32 initialises the MQTT client and connects to the broker with authentication. Each device publishes local sensor state changes to a topic and subscribes to updates from the other device, triggering corresponding LED animations. The system continuously monitors the MQTT connection and automatically reconnects if disconnected, ensuring stable communication over long distances.

**Sensor and LED Light Feedback**

Upon successful WI-FI and MQTT connection, the system monitors weight changes using an FSR sensor and a threshold value. The ESP32 reads and averages analogue voltages to improve accuracy. When pressure exceeds the threshold, an LED is triggered, and a status update is sent via MQTT, providing visual feedback that the state change was successfully transmitted. Upon receiving an update, the peer device activates a searchlight-style LED animation. To avoid message congestion, the system only publishes when a change in sensor state is detected.

### 4.3 Hardware Material Selection and System Design

The final hardware configuration adopted in this project comprises the ESP-WROOM-32D microcontroller unit (MCU) as the central controller, a pixel LED strip serving as the visual dashboard interface, and the FSR406B force-sensitive resistor as the primary input sensor.

The LED dashboard needs to convey two types of system status to the user:
Local Sensor Activation: When a key is placed on the sensor, the system identifies the applied force and transmits the information to the controller. Upon successful signal acquisition, a designated LED on the strip is illuminated to confirm data reception and processing by the central unit.

Remote Event Notification: When a remote sensor (on another device) is triggered, and the corresponding data is transmitted via MQTT protocol, the receiving device responds by illuminating the latter segment of its LED strip. This indicates that an external sensor event has been received and registered through the communication network.

Upon power-up and successful Wi-Fi connection, the entire LED strip performs a brief flash sequence. This startup indicator confirms that the system is active and ready for operation.

As determined through the earlier Literature Review, the sensing component utilises the FSR406B configured with a 10 kΩ voltage divider resistor, forming a potential divider circuit powered by the 3.3 V output on ESP32. This configuration balances measurement sensitivity and maximum detectable load, enabling accurate detection of lightweight objects such as keychains while avoiding premature sensor saturation.


<p align="center">
  <img src="/Final Report/figures/F7.png"/>
   <br>
  <em>Fig. 7 - ESP32 FireBeetle Dev Board Features.</em>
</p>

The central controller selected for this project is the FireBeetle ESP32 Development Board by DFRobot, which is equipped with the ESP-WROOM-32 dual-core chip.  This module meets the software requirements by providing built-in Wi-Fi connectivity and local storage capability. Furthermore, it features both 3.3 V and VCC (5 V) power output pins, enabling simultaneous power supply to the sensor and dashboard components.

This design integrates signal acquisition, local feedback, and networked communication into a compact and energy-efficient system, meeting the functional and operational requirements of the embedded monitoring task.

The PCB design of this project has two versions. The first version of the circuit design uses a 10kΩ pull-down resistor.

<p align="center">
  <img src="/Final Report/figures/F8.png"/>
   <br>
  <em>Fig. 8 - The first version of circuit design: pull-down Resistor circuit</em>
</p>

After an initial prototype, the second version of the circuit employed a pull-up resistor configuration.

<p align="center">
  <img src="/Final Report/figures/F9.png"/>
   <br>
  <em>Fig. 9 - Circuit design after iteration.</em>
</p>

This design change was motivated by the observation that the GPIO pins of ESP32 could not maintain a stable high-level output, leading to fluctuations in the Force Sensitive Resistor (FSR) readings when using a pull-down resistor. The input voltage was stabilised by implementing a pull-up configuration connected to a 3.3 V supply, thereby improving the FSR measurements' reliability.

---

## 5. Conclusions and Future Work

### 5.1 Conclusions

Guiding Glow presents a lightweight and emotionally resonant Internet of Things solution designed to support long-distance familial relationships, particularly between parents and children. Using the symbolic metaphor of a boat and a lighthouse, the system allows users to initiate a two-way emotional interaction by simply placing a key on the device, enabling screen-free, non-verbal communication. The current prototype has demonstrated bidirectional connectivity, intuitive operation, and a form factor that emphasises both emotional expressiveness and usability.

Despite the progress made, several limitations remain. The device currently relies on external power and stable Wi-Fi connectivity, which may restrict its portability and deployment in varied settings. To integrate more seamlessly into everyday life, future versions will need to enhance autonomy, energy efficiency, and environmental adaptability.


### 5.2 Future Work

The simple structural design and user-friendly nature of the device provide a strong foundation for the future scalability of this project. Future development could prioritise enhancing cultural inclusivity and adaptability of the product. For instance, a haptic feedback module could be integrated to support users with visual impairments, enabling them to perceive remote presence through variations in the surface temperature of the lighthouse. Replacing the ship and lighthouse imagery with forms more closely aligned with local cultures could improve adaptability for multicultural markets. 

From a technical standpoint, further user experience optimisation could be achieved by integrating built-in battery support, developing lower-power system architectures, and implementing automated network configuration technologies.

As the product undergoes further optimisation and expansion, Guiding Glow could be developed into a highly customisable and culturally resonant solution for fostering non-verbal emotional connections within home settings.


---

## References

Bakhurst, M., McGuire, A. and Halford, W.K. (2017). Trauma Symptoms, Communication, and Relationship Satisfaction in Military Couples. Family Process, 57(1), pp.241–252. doi:https://doi.org/10.1111/famp.12285.

Bolton, P., Hubble, S. and Gower, M. (2024) ‘International students in UK higher education’, commonslibrary.parliament.uk [Preprint]. Available at: https://commonslibrary.parliament.uk/research-briefings/cbp-7976/.

DFRobot (n.d.). FireBeetle ESP32 IOT Microcontroller (Supports Wi-Fi & Bluetooth with 15 ESP32 Projects - DFRobot. [online] www.dfrobot.com. Available at: https://www.dfrobot.com/product-1590.html.

Espressif Systems (2023). ESP32WROOM32D & ESP32WROOM32U Datasheet. [online] Available at: https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf.

Interlink Electronics (2010). FSR 406 Data Sheet Figure 1 -Typical Force Curve Industry Segments Interlink Electronics -Sensor Technologies FSR 400 Series Square Force Sensing Resistor. [online] Available at: https://cdn.sparkfun.com/assets/c/4/6/8/b/2010-10-26-DataSheet-FSR406-Layout2.pdf.

Neustaedter, C. and Greenberg, S. (2012). Intimacy in long-distance relationships over video chat. Proceedings of the 2012 ACM annual conference on Human Factors in Computing Systems - CHI ’12, pp.753–762. doi:https://doi.org/10.1145/2207676.2207785. 

Twinkl (2025). [online] Twinkl.co.uk. Available at: https://www.twinkl.co.uk/parenting-wiki/lighthouse-parenting.

Ye, J. (2006). Traditional and Online Support Networks in the Cross-Cultural Adaptation of Chinese International Students in the United States. Journal of Computer-Mediated Communication, [online] 11(3), pp.863–876. doi:https://doi.org/10.1111/j.1083-6101.2006.00039.x.

Zhang, J. and Goodson, P. (2011). Predictors of international students’ psychosocial adjustment to life in the United States: A systematic review. International Journal of Intercultural Relations, 35(2), pp.139–162. doi:https://doi.org/10.1016/j.ijintrel.2010.11.011.


---



**Contributors:**
- Zhiyu Cao (ID: 24067861)
- Zinan Ye (ID: 24224670)
- Emmanuel Gyamfi (ID: 24252619)
- Muchen Han (ID: 24100754)

---

*April 2025 - CASA0021 Final Project*
