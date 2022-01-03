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
Avg hashrate: 251 H/s<br>
Connect power to 3.3V or 5V or USB, UART TX to PD6, RX to PD5. LED connected to PB5<br>

![STM8S103F3](https://user-images.githubusercontent.com/33000089/147915952-c62dfed8-b33f-4e80-bde3-2dce7908239d.png)<br>

