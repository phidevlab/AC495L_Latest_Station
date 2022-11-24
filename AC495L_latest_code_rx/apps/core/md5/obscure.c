
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "obscure.h"
#include "md5.h"

#ifndef DIGEST
extern void get_license_key(char *license_key);
#endif /*DIGEST*/

#define	SIZE_OF_KEY	8

#define	VALIDITY_KEY 	"4*7#6&8!"
#define	EMPTY_KEY 		"Please copy your license key here"

#define SECRET_KEY { 0x08, 0x41, 0xa3, 0x5c, 0x2b, 0xfe, 0x2b, 0x44, 0x44, 0x67, 0x0D, 0x4d, 0x02, 0xe2, 0xc3, 0x4c }

#define SECRET_KEY_LEN 16


static unsigned char str_mangle(unsigned char data, unsigned char key, unsigned char prev);
static unsigned char str_unmangle(unsigned char data, unsigned char key, unsigned char prev);

/* compute next key = md5(previous key) */
static void next_key(unsigned char *key)
{
    MD5_CTX ctx;
    
    MD5_Init(&ctx);
    MD5_Update(&ctx, key, SECRET_KEY_LEN);
    MD5_Final(key, &ctx);
}

/* process the data with the given mangle function
 * will process len bytes, or up to a first \0 if len=0
 */
typedef unsigned char (*mangle_func_t)(unsigned char, unsigned char, unsigned char);

static void process(unsigned char *data, int len, mangle_func_t mangle)
{
    unsigned char key[SECRET_KEY_LEN] = SECRET_KEY;
    unsigned char newData, prev = 0;
    int i;

    for (i=0; (len && i<len) || (!len && data[i]); i++)
    {
	if (0 == i%SECRET_KEY_LEN)
	    next_key(key);
	
	newData = mangle(data[i], key[i%SECRET_KEY_LEN], prev);
	if(mangle == str_mangle)
		prev = newData;
	else
		prev = data[i];
	data[i] = newData; 
   }
}


int hex2int(char *str)
{
    return (int)strtol(str, NULL, 16);
}

/* Obscuring binary data is done by XORing with the key */
static unsigned char xor_mangle(unsigned char data, unsigned char key, unsigned char prev)
{
    return data ^ key;
}

void obscure_bin(unsigned char *data, int len)
{
    process(data, len, xor_mangle);
}

void unobscure_bin(unsigned char *data, int len)
{
    /* since we only use XOR, we can use same function for both
     * encripting and decripting
     */
    process(data, len, xor_mangle);
}

/* 
 * P - input plain text byte (1 - 255)
 * K - input key sequence byte (0 - 255)
 * O - output obscure byte (1 - 255)
 *
 * Obscuring string data:
 * Ox = Px + Kx;
 * if (Ox > 255)
 *     Ox -= 255;
 *
 * Unobscuring:
 * Px = Ox - Kx;
 * if (Px < 1)
 *    Px += 255;
 */
static unsigned char str_mangle(unsigned char data, unsigned char key, unsigned char prev)
{
    return(data + key + prev);
}

static unsigned char str_unmangle(unsigned char data, unsigned char key, unsigned char prev)
{
    return(data - key - prev);
}

void obscure_str(char *data)
{
    process((unsigned char *)data, 0, str_mangle);
}

void unobscure_str(char *data)
{
    process((unsigned char *)data, 0, str_unmangle);
}

/* Search for special characters and code to hexadecimal value in the format
 * '(' --> &28;
 * If buf == NULL, return the len needed for coded string, else
 * write the coded string to buf.
 */
int set_write_special_chars(char *str, char *buf)
{
    int special, len;

    for (special = 0, len = 0; *str ; str++, len++)
    {
	if (build_is_special_char(*str, SPECIAL_CHAR_TO_AMP_HEX))
	{
	    special++;
	    if (buf)
	    {
		    sprintf(buf, "&%02x;", (unsigned char)*str);
		    buf+= SPECIAL_CHAR_SIZE + 1;
	    }
	}
	else
	{
	    if (buf)
		*buf++ = *str;
	}
    }
    len += SPECIAL_CHAR_SIZE * special;

    return len;
}

/* Search for special coded characters and decode them in the format
 */
int set_read_special_chars(char *buf, int len, char *special_chars)
{
    int special, i, new_len;
    char *index, *tmp;
    char amp_hex_buf[SPECIAL_CHAR_SIZE];

    for (special = 0, i = 0; i < len; i++)
    {
	if (build_is_special_char(buf[i], SPECIAL_AMP_HEX_TO_CHAR))
	    special++;
    }
    amp_hex_buf[SPECIAL_CHAR_SIZE - 1] = 0;
    new_len = len - SPECIAL_CHAR_SIZE * special;

	/* Nirm, 12072006, the returned string contained too many chars */ 
    
    for (index = special_chars, tmp = buf + len; buf < tmp; buf++, index++)
    {
	if (build_is_special_char(*buf, SPECIAL_AMP_HEX_TO_CHAR))
	{
	   	buf++; /* To avoid '&' */
	   	strncpy(amp_hex_buf, buf, SPECIAL_CHAR_SIZE - 1); 
		
	   	special = hex2int(amp_hex_buf);
	   	if (special < 0)
	   	{
			return 0;
	   	}

	   	*index = special;
		
	   	buf += SPECIAL_CHAR_SIZE - 1;
	}
	else
		*index = *buf;

   }

   *index = 0; 
   	return 1;
}




int build_is_special_char(int c, int flag)
{
    switch (flag)
    {
    case SPECIAL_CHAR_TO_AMP_HEX:
	if (c > 126 || c < 32 || c == '(' || c == ')' || c == '&' || c == ';'
	    || c == '\'' || c == '\"' || c == '\\')
	    return 1;
	break;
    case SPECIAL_AMP_HEX_TO_CHAR:
	if (c == '&')
	    return 1;
	break;
    }
    
    return 0;
}

#ifndef DIGEST

int check_license_key_validity(char *key_to_rv)
{
	int 		i, j;
	char		licese_key[512], buf[512], license_key_free_buf[512];
	char	 	companyName[512], receivedCheckSum[512], currentCheckSum[512];
	char		checkSum = 0;
	char		validity_key[SIZE_OF_KEY+1]= "";

	memset(buf, 0, sizeof(buf));
	memset(licese_key, 0, sizeof(licese_key));
	memset(license_key_free_buf, 0, sizeof(license_key_free_buf));
	memset(receivedCheckSum, 0, sizeof(receivedCheckSum));
	memset(currentCheckSum, 0, sizeof(currentCheckSum));

	get_license_key(licese_key);

	if(!strcmp(licese_key, EMPTY_KEY)){
		printf("\nNo valid license key found.\n");
		printf("Please copy the license you received from AudioCodes to the appropriate file.\n");
		printf("For details please contact AudioCodes.\n\n");
		return -1; 
	}

	set_read_special_chars(licese_key, strlen(licese_key), buf);

	unobscure_str(buf);

	for(i=0; i<SIZE_OF_KEY; i++)
	{
		validity_key[i] = buf[i*2];
	}

	if(!strcmp(validity_key, VALIDITY_KEY)){
		for(i=0; i<SIZE_OF_KEY; i++)
		{
			license_key_free_buf[i] = buf[(i*2)+1];
		}

		for(j=(i*2); j<strlen(buf); j++, i++)
		{
			license_key_free_buf[i] = buf[j];
		}

		strcpy(companyName, license_key_free_buf);
		companyName[i-2] = '\0';

		license_key_free_buf[i+1] = '\0';

		strcpy(receivedCheckSum, license_key_free_buf+strlen(license_key_free_buf)-2);
	
		for(i=0; companyName[i]!= 0; i++)
		{
			checkSum ^= companyName[i];
		}

		sprintf(currentCheckSum, "%02x", checkSum);

		if(!strcmp(receivedCheckSum, currentCheckSum)){
#if defined(P_PLATFORM)||defined(B_PLATFORM)||defined(I_PLATFORM)||defined(GROCX_PLATFORM)
			printf("\nThis copy of AC48x CPE VoIP Toolkit is licensed to : %s\n", companyName);
#elif defined(S_PLATFORM)
			printf("\nThis copy of AC494 Linux SDK is licensed to : %s\n", companyName);
#elif defined(B_CEVA_PLATFORM)
			printf("\nThis copy of VoIPerfect is licensed to : %s\n", companyName);
#else
			printf("\nUndefined Platform\n");
			exit(0);
#endif
		}
		else{
			return 0; /*rvFalse*/
		}

	}
	else{
		return 0; /*rvFalse*/
	}

	strcpy(key_to_rv, validity_key);

	return 1;
	
}

#endif /*DIGEST*/
