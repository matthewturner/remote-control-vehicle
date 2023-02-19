# YARV - Yet Another Remote-controlled Vehicle

[![PlatformIO CI](https://github.com/matthewturner/remote-control-vehicle/actions/workflows/platformio.yml/badge.svg)](https://github.com/matthewturner/remote-control-vehicle/actions/workflows/platformio.yml)

The bluetooth remote-controlled car and boat

## Pinout

| Pin | PWM | Usage                     | Wire Colour |
| --- | --- | ---                       | ---         |
| 2   |     | TRANSMIT_PIN              | Yellow      |
| ~3  | Yes | RECEIVE_PIN               | Green       |
| 4   |     |                           |             |
| ~5  | Yes | SENSOR_SERVO_PIN          | Yellow      |
| ~6  | Yes | MOTOR_LEFT_ENABLE_PIN     | Green       |
| 7   |     | MOTOR_LEFT_DIRECTION_PIN  |             |
| 8   |     | MOTOR_RIGHT_DIRECTION_PIN |             |
| ~9  | Yes | MOTOR_RIGHT_ENABLE_PIN    | Yellow      |
| ~10 | Yes |                           |             |
| ~11 | Yes | LEFT_BUMPER_PIN           | Green       |
| 12  |     | RIGHT_BUMPER_PIN          | Yellow      |
| 13  |     |                           |             |
| A4  |     | SDA                       | Blue        |
| A5  |     | SCL                       | Green       |

## Demos

![car](https://user-images.githubusercontent.com/1783027/97975863-ab042000-1dc1-11eb-807a-c870f89b6db3.jpg)

![Video](https://media.giphy.com/media/VY4mH3ABTiCAt8fhKN/giphy.gif)

[Full Video](https://youtu.be/ySsLElAfF24)

## Components

* [Chassis/motors](https://www.amazon.co.uk/gp/product/B071CF3NXM/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1)
* [Elegoo Nano](https://www.amazon.co.uk/gp/product/B072BMYZ18/ref=ppx_yo_dt_b_asin_title_o08_s00?ie=UTF8&psc=1)
* [Bluetooth module](https://www.amazon.co.uk/gp/product/B07BHRGJLJ/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)
* [Motor driver](https://www.amazon.co.uk/gp/product/B01KBTNHS6/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)
* [Perf board](https://www.amazon.co.uk/gp/product/B0775BRB6B/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)
* [Battery](https://www.ebay.co.uk/itm/2-Pack-7-2V-1400mAh-Battery-Pack-SM-Plug-for-RC-Car-Spare-Parts-Accessories/392791460611?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649)
* [Power connectors](https://www.ebay.co.uk/itm/5-Pairs-2-Pin-SM-Plug-Connector-Cable-Wire-Male-Female-5-Of-Each-Connector/322057378908?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649)
* [Battery charger](https://www.amazon.co.uk/gp/product/B07V5H5GX7/ref=ppx_yo_dt_b_asin_title_o06_s00?ie=UTF8&psc=1)

* [Android App](https://github.com/matthewturner/Yarv.Android)
