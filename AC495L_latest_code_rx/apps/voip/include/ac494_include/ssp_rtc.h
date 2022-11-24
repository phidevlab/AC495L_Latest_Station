/* RTC defines */
#define RTC_ADDR			0x68

#define RTC_SECONDS_REG		0x00
#define RTC_MINUTES_REG		0x01
#define RTC_HOURS_REG		0x02
#define RTC_DAY_REG			0x03
#define RTC_DATE_REG		0x04
#define RTC_MONTH_REG		0x05
#define RTC_YEAR_REG		0x06
#define RTC_CTRL_REG		0x07

#define RTC_HOUR_REG_12HR_MODE      0x40
#define RTC_HOUR_REG_AM_PM          0x20

#define RTC_CMD_SET         0
#define RTC_CMD_SHOW        1

typedef volatile struct RTC_HOURS_BITS
{
	unsigned char 			: 1;
	unsigned char hour_mode	: 1;
	unsigned char am_pm		: 1;
	unsigned char hour		: 5;
}T_RTC_HOURS_BITS;

typedef volatile struct RTC_HOURS_STRUCT
{
	union
	{
		T_RTC_HOURS_BITS	bit;
		unsigned char				all;
	} reg;
}T_RTC_HOURS;

int write_rtc(unsigned char addr, unsigned char data);
int read_rtc(unsigned char addr, unsigned char* data);
unsigned char int_to_bcd(unsigned char integer);
unsigned char bcd_to_int(unsigned char bcd);
void rtc_action(int cmd);


typedef struct {

	int day;
	int date;
	int month;
	int year;
	int	hours;
	int minutes;
	int	seconds;
	int	am;

} RTC_INFO_STCT;


#define Help_RTC "wrong command"
