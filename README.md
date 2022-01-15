# STM8_DUCO_miner
STM8S firmware for mining DUCO.<br>

<a href="https://duinocoin.com">
    <img src="https://github.com/revoxhere/duino-coin/blob/master/Resources/ducobanner.png?raw=true" width="430px" />
</a><br><br>

Compile with SDCC: http://sdcc.sourceforge.net/<br>
IDE: Code::Blocks https://www.codeblocks.org/<br>
The compiled firmware for STM8S003/STM8S103 is here: <a href="https://github.com/BBS215/STM8_DUCO_miner/blob/main/bin/Release/STM8_DUCO_miner.hex">STM8_DUCO_miner.hex</a><br>
Program with STLink or other programmer supporting SWIM protocol.<br>
ST Visual Programmer: https://www.st.com/en/development-tools/stvp-stm8.html

Tested on STM8S003F3P6 development board: https://www.amazon.com/Development-Minimum-System-STM8S003F3P6-Module/dp/B07L178DKC<br>
Avg hashrate: 260 H/s<br>
Connect power to 3.3V or 5V or USB, UART TX to PD6, RX to PD5. LED connected to PB5<br>
Increase the avr_timeout setting in Settings.cfg from 4 to 10 seconds.<br>

How to enable UART on Raspberry Pi 4/3/Zero W: https://di-marco.net/blog/it/2020-06-06-raspberry_pi_3_4_and_0_w_serial_port_usage/<br>

![RasPi_connect](https://user-images.githubusercontent.com/33000089/147982746-e581bddf-83df-4fcf-b022-40b286468f01.png)

![PL2303HX_connect](https://user-images.githubusercontent.com/33000089/147985176-451eb2e6-d214-4649-9d42-6ca4f2a50795.png)
