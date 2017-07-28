#ifndef	__ADC_H__
#define	__ADC_H__


#define	ADC_BASE		0x126C0000

#define	ADCCON_OFFSET	0x0000
#define	ADCDAT_OFFSET	0x000C
#define	ADCMUX_OFFSET	0x001C


#define	ADCCON_ADDR		(ADC_BASE + ADCCON_OFFSET)
#define	ADCCON_10BIT_CONV		0x0	// [16]
#define	ADCCON_12BIT_CONV		0x1
#define	ADCCON_CONV_PROC		0x0	// [15]
#define	ADCCON_CONV_END			0x1
#define	ADCCON_CONV_PRESC_DISABLE	0x0	// [14]
#define	ADCCON_CONV_PRESC_ENABLE	0x1
#define	ADCCON_CONV_PRESC_DATA_MASK		(((0x1<<8)-1)<<6)	// [13:6]
#define	ADCCON_CONV_NORMAL		0x0	// [2]
#define	ADCCON_CONV_STANDBY		0x1
#define	ADCCON_CONV_READ_START_DISABLE	0	// [1]
#define	ADCCON_CONV_READ_START_ENABLE	1
#define	ADCCON_CONV_START_ENABLE		1	// [0]

#define	ADCDAT_ADDR		(ADC_BASE + ADCDAT_OFFSET)
#define	ADCDAT_DATA_MASK		0xFFF	// [11:0]
#define	ADCDAT_10BIT			10
#define	ADCDAT_12BIT			12



#define	ADCMUX_ADDR		(ADC_BASE + ADCMUX_OFFSET)
#define	ADCMUX_AIN0		0x0	// AIN0
#define	ADCMUX_AIN1		0x1	// AIN0
#define	ADCMUX_AIN2		0x2	// AIN0
#define	ADCMUX_AIN3		0x3	// AIN0
#define	ADCMUX_AIN_BIT	4	// [3:0]


#define	ADC_CFG_ADDR	0x10010118
#define	ADC_GENERAL		0x0	// [16]
#define	ADC_MTCADC_ISP	0x1



#endif	/* __ADC_H__ */
