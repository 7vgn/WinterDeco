# Winter Decoration 2025

The 2025 winter-themed decoration mimics the aesthetics of an old-fashioned 90s-style computer. True to form, it uses clicky buttons instead of touch sensors. The screen resolution of 8x8 pixels is also reminiscent of bygone days. Just like last year, there is a hidden game. Can you find it without looking at the code?

## Bill of Materials

| Ref | Qty | Part | Value | Links[^links]
| --- | ---:| --- |
| - | 1 | PCB | 2-layer, 1.6mm thickness, blue soldermask, lead-free HASL | [J](https://jlcpcb.com/)
| BT1 | 1 | Battery Clip | CR2032, SMD mount | [M](https://mouser.com/ProductDetail/?qs=sGAEpiMZZMvxqoKe%252BDjhrhkd9JgmX6KPIe2Fc6FHFcOAlHTLYwGDJA%3D%3D) [D](https://www.digikey.com/en/products/detail/te-connectivity-linx/BAT-HLD-003-SMT-TR/14301788?s=N4IgTCBcDaIEIEEAqBaAEgGQCIoAy4GYUBlAWVSQCUQBdAXyA) [R](https://www.reichelt.de/shop/produkt/knopfzellenclip_fuer_20_mm_smd-56574)
| C1 | 1 | Capacitor | SMD 1206, 100µF, X5R | [M](https://mouser.com/ProductDetail/?qs=pUKx8fyJudDzdyEEakAKPA%3D%3D) [D](https://www.digikey.com/en/products/detail/murata-electronics/GRM31CR60J107KEA8L/16286000?s=N4IgTCBcDaIOICUCyBmAjAYQQNgAwCk1cB2AaQFEBBADgBkQBdAXyA) [R](https://www.reichelt.de/shop/produkt/mlcc_1206_100_f_6_3_v_-10_x5r-409219)
| C2..3 | 2 | Capacitor | SMD 1206, 100nF, X7R | [M](https://mouser.com/ProductDetail/?qs=Jm2GQyTW%2Fbgjukwx9JJFzw%3D%3D) [D](https://www.digikey.com/en/products/detail/kyocera-avx/KGM31DR72A104KU/563639?s=N4IgTCBcDaIIxgAwDY4GE6ICwGkCCAKmHiALoC%2BQA) [R](https://www.reichelt.de/shop/produkt/mlcc_1206_100_nf_100_v_-10_x7r-409241)
| D* | 64 | LED | SMD 1206, yellow | [M](https://mouser.com/ProductDetail/?qs=sGAEpiMZZMv0DJfhVcWlK%252BdbpgZMtcHOeXAaxJ3bARo%3D) [D](https://www.digikey.com/en/products/detail/liteon/LTST-C150YKT/269217?s=N4IgTCBcDaIDIBUDKCC0BhAjAVgAwE0BpBEAXQF8g) [R](https://www.reichelt.de/shop/produkt/led_smd_1206_gelb_150_mcd_120_-361548)
| Q1[^polprot] | 1 | P-MOSFET | IRLML6402, SOT-23 | [M](https://mouser.com/ProductDetail/?qs=9%252BKlkBgLFf0HuZuONx2Ewg%3D%3D) [D](https://www.digikey.com/en/products/detail/infineon-technologies/IRLML6402TRPBF/811437) [R](https://www.reichelt.de/shop/produkt/mosfet_p-ch_-20v_-3_7a_0_065r_sot-23-108743)
| R1..4 | 4 | Resistor | SMD 1206, 68Ω | [M](https://mouser.com/ProductDetail/?qs=CteSnpDdeuDJTICysoGhNw%3D%3D) [D](https://www.digikey.com/en/products/detail/yageo/RC1206JR-0768RL/729330?s=N4IgTCBcDaIEoGECMYAMA2AUnAtKg7OgBxwAyIAugL5A) [R](https://www.reichelt.de/shop/produkt/smd-widerstand_1206_68_ohm_250_mw_5_-18364)
| SW1..3 | 3 | Push Buttons | B3FS-1000P | [M](https://mouser.com/ProductDetail/?qs=ImaqFqjHA4k6odVF2%2FXWpQ%3D%3D) [D](https://www.digikey.com/en/products/detail/omron-electronics-inc-emc-div/B3FS-1000P/277812?s=N4IgTCBcDaIEYGYBmBnAtARgAw4A4gF0BfIA) [R](https://www.reichelt.de/shop/produkt/taster_smd_1_schliesser_no_-243056)
| U1 | 1 | Microcontroller | PIC18F14Q41-I/SO, SOIC-14 | [M](https://mouser.com/ProductDetail/?qs=W%2FMpXkg%252BdQ5jNCMT7LkJWg%3D%3D) [D](https://www.digikey.com/en/products/detail/microchip-technology/PIC18F14Q41-I-SO/13415037?s=N4IgTCBcDaIA4EsDGBGAHAMxQFgI7ZQFoEB6AZwHsQBdAXyA) [R](https://www.reichelt.de/shop/produkt/8-bit-picmicro_mikrocontroller_16_kb_64_mhz_so-20-311092)
| U2 | 1 | Demultiplexer | 74HC154, SOIC-24 | [M](https://mouser.com/ProductDetail/?qs=YxwvVplHM%2FnR3vJab0%252BPQw%3D%3D) [D](https://www.digikey.com/en/products/detail/texas-instruments/CD74HC154M96/1506686) [R](https://www.reichelt.de/de/de/shop/produkt/decoder_4_to_16_2_6_v_so-24-18644)
| - | 1 | Piece of String | |
| - | 1 | Coin Cell | CR2032 |

[^links]: Links are just examples. Other sellers are available and similar parts can be substituted.  
[^polprot]: Q1 protects against reverse polarity and is optional. If left out, the solder jumper JP1 must be bridged instead. 

Required tools and materials: soldering iron, lead-free solder, tweezers, flux, isopropyl alcohol for cleaning, PIC programmer
