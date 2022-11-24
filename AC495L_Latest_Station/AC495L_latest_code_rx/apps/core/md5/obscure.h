
#ifndef _OBSCURE_H_
#define _OBSCURE_H_


#define SPECIAL_CHAR_TO_AMP_HEX 1
#define SPECIAL_AMP_HEX_TO_CHAR 2
#define SPECIAL_CHAR_SIZE 3



void obscure_bin(unsigned char *data, int len);
void unobscure_bin(unsigned char *data, int len);
void obscure_str(char *data);
void unobscure_str(char *data);

int set_write_special_chars(char *str, char *buf);
int set_read_special_chars(char *buf, int len, char *read_special_chars);
int build_is_special_char(int c, int flag);
int check_license_key_validity(char *key_to_rv);
#endif

