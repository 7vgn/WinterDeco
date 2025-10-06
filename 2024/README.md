# Winter Decoration 2024

The 2024 winter-themed decoration is Simon the Snowman. Simon has five touch-sensitive buttons. A long press of the lower right button turns him on of off. When turned on, a short press on each of the five buttons switches to a unique program. Simon also has a hidden secret game. Can you find it without looking at the code?

The touch sensors are implemented using the [PIC18F14Q41](https://www.microchip.com/en-us/product/pic18f14q41)'s capacitive voltage divider (CVD) feature. You can learn more about CVD in Microchip's Application Notes [1478](https://ww1.microchip.com/downloads/en/appnotes/01478A.pdf) and [2934](https://ww1.microchip.com/downloads/en/AppNotes/AN2934-Capacitive-Touch-Sensor-Design-Guidelines-00002934A.pdf). 

## Bill of Materials

| Ref | Qty | Part | Value | Links[^links]
| --- | ---:| --- |
| - | 1 | PCB | 2-layer, 1.6mm thickness, white soldermask, black silkscreen, ENIG[^enig] | [J](https://jlcpcb.com/)
| BT1 | 1 | Battery Clip | CR2032, SMD mount | [M](https://mouser.com/ProductDetail/?qs=sGAEpiMZZMvxqoKe%252BDjhrhkd9JgmX6KPIe2Fc6FHFcOAlHTLYwGDJA%3D%3D) [D](https://www.digikey.com/en/products/detail/te-connectivity-linx/BAT-HLD-003-SMT-TR/14301788?s=N4IgTCBcDaIEIEEAqBaAEgGQCIoAy4GYUBlAWVSQCUQBdAXyA) [R](https://www.reichelt.de/shop/produkt/knopfzellenclip_fuer_20_mm_smd-56574)
| C1 | 1 | Capacitor | SMD 1206, 100µF, X5R | [M](https://mouser.com/ProductDetail/?qs=pUKx8fyJudDzdyEEakAKPA%3D%3D) [D](https://www.digikey.com/en/products/detail/murata-electronics/GRM31CR60J107KEA8L/16286000?s=N4IgTCBcDaIOICUCyBmAjAYQQNgAwCk1cB2AaQFEBBADgBkQBdAXyA) [R](https://www.reichelt.de/shop/produkt/mlcc_1206_100_f_6_3_v_-10_x5r-409219)
| D* | 12 | LED | SMD 1206, yellow | [M](https://mouser.com/ProductDetail/?qs=sGAEpiMZZMv0DJfhVcWlK%252BdbpgZMtcHOeXAaxJ3bARo%3D) [D](https://www.digikey.com/en/products/detail/liteon/LTST-C150YKT/269217?s=N4IgTCBcDaIDIBUDKCC0BhAjAVgAwE0BpBEAXQF8g) [R](https://www.reichelt.de/shop/produkt/led_smd_1206_gelb_150_mcd_120_-361548)
| D* | 6 | LED | SMD 1206, red (mouth) | [M](https://mouser.com/ProductDetail/?qs=sGAEpiMZZMv0DJfhVcWlK%252BdbpgZMtcHOPtGkXd2H%2F2o%3D) [D](https://www.digikey.com/en/products/detail/liteon/LTST-C150KRKT/386761?s=N4IgTCBcDaIDIBUDKCC0BhAjAVgAwGkAlfBEAXQF8g) [R](https://www.reichelt.de/de/de/shop/produkt/led_smd_1206_rot_150_mcd_120_-361547)
| D* | 5 | LED | SMD 1206, green (buttons) | [M](https://mouser.com/ProductDetail/?qs=sGAEpiMZZMv0DJfhVcWlK%252BdbpgZMtcHOivY63VReAjk%3D) [D](https://www.digikey.com/en/products/detail/liteon/LTST-C150KGKT/365085?s=N4IgTCBcDaIDIBUDKCC0BhAjAVgAwGkBxfBEAXQF8g) [R](https://www.reichelt.de/de/de/shop/produkt/led_smd_1206_gruen_500_mcd_120_-361549)
| D* | 1 | LED | SMD 1206, orange (nose) | [M](https://mouser.com/ProductDetail/?qs=sGAEpiMZZMv0DJfhVcWlK%252BdbpgZMtcHO1szBFqLoZwU%3D) [D](https://www.digikey.com/en/products/detail/liteon/LTST-C150KFKT/386758?s=N4IgTCBcDaIDIBUDKCC0BhAjAVgAwGkAxfBEAXQF8g) [R](https://www.reichelt.de/de/de/shop/produkt/smd-led_superhell_orange_200_mcd_120_3_2_x_1_6_mm_1206_-231710)
| Q1[^polprot] | 1 | P-MOSFET | IRLML6402, SOT-23 | [M](https://mouser.com/ProductDetail/?qs=9%252BKlkBgLFf0HuZuONx2Ewg%3D%3D) [D](https://www.digikey.com/en/products/detail/infineon-technologies/IRLML6402TRPBF/811437) [R](https://www.reichelt.de/shop/produkt/mosfet_p-ch_-20v_-3_7a_0_065r_sot-23-108743)
| R1..4 | 4 | Resistor | SMD 1206, 68Ω | [M](https://mouser.com/ProductDetail/?qs=CteSnpDdeuDJTICysoGhNw%3D%3D) [D](https://www.digikey.com/en/products/detail/yageo/RC1206JR-0768RL/729330?s=N4IgTCBcDaIEoGECMYAMA2AUnAtKg7OgBxwAyIAugL5A) [R](https://www.reichelt.de/shop/produkt/smd-widerstand_1206_68_ohm_250_mw_5_-18364)
| R5 | 1 | Resistor | SMD 1206, 3.3kΩ | [M](https://mouser.com/ProductDetail/?qs=CteSnpDdeuDJAlQtfVL%252BIw%3D%3D) [D](https://www.digikey.com/en/products/detail/yageo/RC1206JR-073K3L/729267?s=N4IgTCBcDaIEoGECMYAMA2AUnAtKg7AMwDShAMiALoC%2BQA) [R](https://www.reichelt.de/shop/produkt/smd-widerstand_1206_3_3_kohm_250_mw_5_-18302)
| R6 | 1 | Resistor | SMD 1206, 4.7kΩ | [M](https://mouser.com/ProductDetail/?qs=CteSnpDdeuDgOu3Rsm1uRA%3D%3D) [D](https://www.digikey.com/en/products/detail/yageo/RC1206JR-074K7L/729295?s=N4IgTCBcDaIEoGECMYAMA2AUnAtKg7ACwDS%2BAMiALoC%2BQA) [R](https://www.reichelt.de/shop/produkt/smd-widerstand_1206_4_7_kohm_250_mw_5_-18330)
| U1 | 1 | Microcontroller | PIC18F14Q41-I/SO, SOIC-14 | [M](https://mouser.com/ProductDetail/?qs=W%2FMpXkg%252BdQ5jNCMT7LkJWg%3D%3D) [D](https://www.digikey.com/en/products/detail/microchip-technology/PIC18F14Q41-I-SO/13415037?s=N4IgTCBcDaIA4EsDGBGAHAMxQFgI7ZQFoEB6AZwHsQBdAXyA) [R](https://www.reichelt.de/shop/produkt/8-bit-picmicro_mikrocontroller_16_kb_64_mhz_so-20-311092)
| - | 1 | Piece of String | |
| - | 1 | Coin Cell | CR2032 |

[^links]: Links are just examples. Other sellers are available and similar parts can be substituted.  
[^enig]: ENIG is not required, but at JLCPCB the combination of white soldermask and lead-free HASL comes with a surcharge which makes ENIG the cheaper option.  
[^polprot]: Q1 protects against reverse polarity and is optional. If left out, the solder jumper JP1 must be bridged instead. 

Required tools and materials: soldering iron, lead-free solder, tweezers, flux, isopropyl alcohol for cleaning, PIC programmer
