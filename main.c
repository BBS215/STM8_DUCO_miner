#include "main.h"

#define MAX_DIFF    655
#define JOB_MAXSIZE 104 // 40+40+20+3 is the maximum size of a job
#define DUCOID_SIZE (6+16)
#define SERIAL_NUM_ADDR 0x4865 // STM8S - 12 bytes serial

uint8_t g_DUCOID[DUCOID_SIZE+1];
struct sha1_hasher_s g_hasher;
uint8_t g_job[JOB_MAXSIZE];


// DUCO-S1A hasher
uint16_t ducos1a(char * lastblockhash, uint8_t lastblockhash_len, char * newblockhash, uint8_t newblockhash_len, uint16_t difficulty)
{
    char str_buf[8];
    int ret;
    uint8_t i, j;
    uint8_t final_len;
    uint8_t *hash_bytes;
    uint16_t ducos1res;

    //toUpperCase
    for(i=0;i<newblockhash_len;i++) if ((newblockhash[i] >= 'a')&&(newblockhash[i] <= 'z')) newblockhash[i] = newblockhash[i] - 'a' + 'A';
    final_len = newblockhash_len >> 1;
    memset(g_job, 0, JOB_MAXSIZE);
    for (i = 0, j = 0; j < final_len; i += 2, j++)
        g_job[j] = ((((newblockhash[i] & 0x1F) + 9) % 25) << 4) + ((newblockhash[i + 1] & 0x1F) + 9) % 25;

    // Difficulty loop
    if (difficulty > MAX_DIFF) return 0; // If the difficulty is too high
    for (ducos1res = 0; ducos1res < ((difficulty * 100) + 1); ducos1res++) {
        ret = sprintf(str_buf, "%d", ducos1res);
        if (ret < 1) return 0; // error!
        sha1_hasher_init(&g_hasher);
        if (sha1_hasher_write(&g_hasher, lastblockhash, lastblockhash_len) != lastblockhash_len) return 0; // error
        if (sha1_hasher_write(&g_hasher, str_buf, ret) != ret) return 0; // error
        hash_bytes = sha1_hasher_gethash(&g_hasher);
        if (memcmp(hash_bytes, g_job, SHA1_HASH_LEN * sizeof(char)) == 0) {
        // If expected hash is equal to the found hash, return the result
            return ducos1res;
        }
    }
    return 0;
}

uint8_t g_resultstr[16+1+32+1+DUCOID_SIZE+2];
uint8_t g_resultstr_size;

uint8_t ducos_make_resultstr(uint8_t *resultstr, uint16_t ducos1result, uint32_t elapsedTime)
{
    uint8_t counter = 0, tmp1;
    int8_t i;

    for(i=15;i>=0;i--)
    {
        if (ducos1result & ((uint32_t)1 << i)) { // 1
            resultstr[counter] = '1';
            counter++;
        } else { // 0
            if (counter > 0) {
                resultstr[counter] = '0';
                counter++;
            }
        }
    }
    if (counter == 0) {
        resultstr[counter] = '0';
        counter++;
    }
    resultstr[counter] = ',';
    counter++;
    tmp1 = 0;
    for(i=31;i>=0;i--)
    {
        if (elapsedTime & ((uint32_t)1 << i)) { // 1
            resultstr[counter] = '1';
            counter++;
            tmp1 = 1;
        } else { // 0
            if (tmp1 > 0) {
                resultstr[counter] = '0';
                counter++;
            }
        }
    }
    if (tmp1 == 0) {
        resultstr[counter] = '0';
        counter++;
    }
    resultstr[counter] = ',';
    counter++;
    for(i=0;i<DUCOID_SIZE;i++) {
        resultstr[counter] = g_DUCOID[i];
        counter++;
    }
    resultstr[counter] = '\n';
    counter++;
    resultstr[counter] = 0;
    return counter;
}

uint8_t g_lastblockhash[64];
uint8_t g_lastblockhash_strsize;
uint8_t g_newblockhash[64];
uint8_t g_newblockhash_strsize;
uint8_t g_difficultystr[5];
uint8_t g_difficultystr_size;

void ducos_loop(void)
{
    uint16_t difficulty;
    uint16_t ducos1result;
    uint32_t elapsedTime;
    uint32_t startTime;

    if (UART_available()) {
        Led_setmode(LED_0, LED_ON); // Turn on the built-in led
        // Read last block hash
	    g_lastblockhash_strsize = UART_readStringUntil(g_lastblockhash, 64, ',');
	    if (g_lastblockhash_strsize == 0) goto exit;
        // Read expected hash
	    g_newblockhash_strsize = UART_readStringUntil(g_newblockhash, 64, ',');
	    if (g_newblockhash_strsize == 0) goto exit;
        // Read difficulty
	    g_difficultystr_size = UART_readStringUntil(g_difficultystr, 5, ',');
	    if (g_difficultystr_size == 0) goto exit;
        difficulty = UART_strtoul(g_difficultystr, g_difficultystr_size);
        // Start time measurement
        startTime = HAL_GetTick_us();
        // Call DUCO-S1A hasher
        ducos1result = ducos1a(g_lastblockhash, g_lastblockhash_strsize, g_newblockhash, g_newblockhash_strsize, difficulty);
        // Calculate elapsed time
        elapsedTime = (HAL_GetTick_us() - startTime);
        UART_Flush(); // Clearing the receive buffer before sending the result.
        g_resultstr_size = ducos_make_resultstr(g_resultstr, ducos1result, elapsedTime);
        UART_send(g_resultstr, g_resultstr_size);
exit:
        Led_setmode(LED_0, LED_OFF); // Turn off the built-in led
    }
}

void getClientId(uint8_t *p_ID)
{
    uint8_t i;
    if (!p_ID) return;
    sprintf(p_ID, "DUCOID");
    for(i=0;i<8;i++) sprintf(&(p_ID[6+(i*2)]), "%02X", ((uint8_t*)SERIAL_NUM_ADDR)[i+4]);
}

void main(void)
{
	disableInterrupts();
	Init_Clock();
	Init_GPIO();
	SYSTimer_Init();
	Leds_init();
	UART_init();
	getClientId(g_DUCOID);
	UART_Flush();
	enableInterrupts();

	while(1)
	{
	    ducos_loop();
	}
}



