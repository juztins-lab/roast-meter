<!--
repo name: roast-meter
description: DIY roast meter guide
github name:  Juztins-Lab
link: https://github.com/juztins-lab/roast-meter
logo path: assets/logo.png
screenshot: 
twitter: your_username
email: justin@skitlabs.net
-->

<!-- PROJECT SHIELDS -->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
<!-- [![LinkedIn][linkedin-shield]][linkedin-url] -->



<!-- PROJECT LOGO -->
<br />
<p align="center">
    <a href="https://github.com/juztins-lab/roast-meter">
        <img src="assets/logo.png" alt="Logo" width="80" height="80">
    </a>
<h3 align="center"><a href="https://github.com/juztins-lab/roast-meter">Roast Meter</a></h3>
    <p align="center">
        An awesome DIY projects that will help you get better consistent roasts by measuring roast color between batches
        <br />
        <br />
        <a href="https://github.com/juztins-lab/roast-meter/issues">Report Bug</a>
        •
        <a href="https://github.com/juztins-lab/roast-meter/issues">Request Feature</a>
    </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

- [Table of Contents](#table-of-contents)
- [About The Project](#about-the-project)
  - [Built With](#built-with)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)



<!-- ABOUT THE PROJECT -->
## About The Project

Starting into home roasting I've quickly realised the importance of a roast meter to evaluate the color of the coffee, this helps you better target different end temperature/time but retain the same level of color and see how it influences taste.


Here's why:
* Commercial grade roast meters are very expensive
* Chips and sensors are are readily available/cheap and easy to get going
* At the end of the guide you will have an ~100$ roast meter, that is consistent and can help you get the most out of your roast.

A list of commonly used resources that I find helpful are listed in the acknowledgements.

### Built With
* [arduino]()


<!-- GETTING STARTED -->
## Getting Started
* Download the source files
* Get your hardware
* Print your 3d case

### Prerequisites

## Hardware
* [Artemis Thing Plus](https://www.sparkfun.com/products/15574)
* [MAX 30101 Sensor](https://www.sparkfun.com/products/16474)
* [Micro OLED](https://www.sparkfun.com/products/14532)
* 2x [50mm QWIIC Cables](https://www.sparkfun.com/products/14426)
* 1x [100mm QWIIC Cables](https://www.sparkfun.com/products/14427)
* [USB-C to PC Cable](https://www.sparkfun.com/products/14743)

## Software
* [Arduino IDE](https://www.arduino.cc/en/software)
* [Micro OLED Library](https://github.com/sparkfun/SparkFun_Micro_OLED_Arduino_Library/archive/refs/heads/main.zip)
* [MAX30101 Library](https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/archive/refs/heads/master.zip)

## Tools and Parts Required
* [Lab slides, 1mm thick and 25.4 mm wide](https://www.amazon.com/AmScope-BS-72P-100S-22-Pre-Cleaned-Microscope-Coverslips/dp/B00L1S93PS/ref=sr_1_6?crid=363P8HPSXU7IV&keywords=lab+slides+7105&qid=1678640185&sprefix=lab+slides+7105%2Caps%2C251&sr=8-6)
* Super Glue 
* A Stanley knife with a new blade to cut the glass (not pretty but it works after a few attempts
* M3 screws in 4mm and 8mm length
* a ruler
* Pliers and other normal hand-tools that most of you would have at hand

If you are building the battery powered version:
* [LiPo Battery with JST Connector no bigger then 35 x 25 mm](https://www.amazon.com/Battery-Rechargeable-Lithium-Polymer-Connector/dp/B07BTWMM8H/ref=sr_1_1?crid=3606UD51RUMWA&keywords=lipo+3.7+400mah+jx&qid=1678641789&refresh=1&sprefix=lipo+3.7+400mah+jx%2Caps%2C260&sr=8-1)
* [5mm slide switch (SS12F15)](https://www.amazon.com/dp/B08H594X92/ref=redir_mobile_desktop?_encoding=UTF8&aaxitk=6791b0967d3006c0d39f902ecaf5c135&content-id=amzn1.sym.7dd77237-72be-4809-b5b5-d553eab7ad9d%3Aamzn1.sym.7dd77237-72be-4809-b5b5-d553eab7ad9d&hsa_cr_id=3406948240101&pd_rd_plhdr=t&pd_rd_r=20d6625e-d387-4249-9877-2d476d60e8de&pd_rd_w=7eRSz&pd_rd_wg=pbMm2&qid=1678642098&ref_=sbx_be_s_sparkle_mcd_asin_1_img&sr=1-2-9e67e56a-6f64-441f-a281-df67fc737124)
* Cables
* Soldering iron and soldering tools

### Installation

## Assembly
Assembling the roast meter is realtively simple and anyone with a bit of basic tinkering skills should be able to make the roast meter.
The hardest part is probably cutting the glass, but since you have most likely bought an ample amount then you can afford to break some.

First choose the right 3D print models.
The base is the same for both the battery and non-battery model.
For the top make sure you choose the model which is correct for your requirements.
NOTE: There is no difference in the measurement performance of the battery vs. non-battery version.
The only difference is that with the non-battery version you need to power the meter from an external power source, whereas the battery powered version is powered by the build in LiPo battery cell.
If you are not confident at soldering maybe stick to the non-battery version as it is then pretty plug and play.

Step 1: Connect a 50mm QWICC cable to the Artemis board - These cables cannot be mounted wrong and will slide in easily. Be carefull not to break the connectors though.

Step 2: Mount the artemis baord in the base - It is pretty self-explanatory, but [here](https://github.com/juztins-lab/roast-meter/blob/main/assets/Case%20with%20board%20mounted.jpg) is a picture 


Step 3: Prepare the glass for the case.
- Take a lab slide and measure 25 mm length and mark with a permanent marker on the glass.
- Place the glass on a hard surface and place a ruler (Best is metal) on the glass and scrape a line in the glass with the stanley knife.
- You can now break the glass off so that you end up with a piece that is app. 25 x 25mm
- Use the stanley knife and the ruler to nip of a couple of mm's from one of the corners.

Verify the glass fits in the 3d printed top.

Step 4: It is time to glue
- Take your super glue and place 4 tiny drops of glue on the 3D printed to, where the glass needs to go.
- With glue less is more - we are really talking about a tiny drop 
- Plase the glass carefully so that it is tight on the top corner of the chamfer made for the glass.
- Wait for the glue to cure and make sure you clean the glass properly for any residue of glue

Step 5: Wire up the sensor QWICC port to the display QWICC port with a 50mm cable

Step 6: Mount the sensors using M3 x 4mm screws

Step 7: Connect the QWICC port on the micro-display to the artemis board

Step 8: Assemble case



## Script install
* Install the Arduino IDE software that you just downloaded.
* Open Arduino IDE
* Load the "roast_meter.ino" file from the src folder.
* Add the board manager url in preferences [guide](https://learn.sparkfun.com/tutorials/artemis-development-with-arduino?_ga=2.2886777.830565863.1676798314-1161989038.1673857417)
* Load the 2 libriaries downloaded above using this [guide] (https://learn.sparkfun.com/tutorials/installing-an-arduino-library)
* At this point you should have already selected the board as "Artemis Thing Plus" and be able to deploy it to your hooked up device.
* Press "Verify" button on the Arduino IDE and if all went well, press "Upload" and your all done, the display should start up and you can begin measuring your roast level.


<!-- USAGE EXAMPLES -->
## Usage
* Guide coming soon


<!-- ROADMAP -->
## Roadmap
* ~~Calibration against Agtron Gourmet Scale~~ (DONE)
* Add battery, on/off button

See the [open issues](https://github.com/juztins-lab/roast-meter/issues) for a list of proposed features (and known issues).



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



<!-- LICENSE -->
## License

Distributed under the GNU License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

Juztin - roastmeter@skitlabs.net

Project Link: [https://github.com/juztins-lab/roast-meter](https://github.com/juztins-lab/roast-meter)



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/juztins-lab/roast-meter.svg?style=flat-square
[contributors-url]: https://github.com/juztins-lab/roast-meter/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/juztins-lab/roast-meter.svg?style=flat-square
[forks-url]: https://github.com/juztins-lab/roast-meter/network/members
[stars-shield]: https://img.shields.io/github/stars/juztins-lab/roast-meter.svg?style=flat-square
[stars-url]: https://github.com/juztins-lab/roast-meter/stargazers
[issues-shield]: https://img.shields.io/github/issues/juztins-lab/roast-meter.svg?style=flat-square
[issues-url]: https://github.com/juztins-lab/roast-meter/issues
[license-shield]: https://img.shields.io/github/license/juztins-lab/roast-meter.svg?style=flat-square
[license-url]: https://github.com/juztins-lab/roast-meter/blob/master/LICENSE.txt
[product-screenshot]: images/screenshot.png
