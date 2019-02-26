

void twi_init();

#define TWIO ((NRF_TWI_REG*)0x40003000)

typedef struct {
    //tasks
    volatile uint32_t STARTRX;
    volatile uint32_t RESERVED0;
    volatile uint32_t STARTTX;
    volatile uint32_t RESERVED1[2];
    volatile uint32_t STOP;
    volatile uint32_t RESERVED2;
    volatile uint32_t SUSPEND;
    volatile uint32_t RESUME;
    volatile uint32_t RESERVED3[56];

    //events
    volatile uint32_t STOPPED;
    volatile uint32_t RXDREADY;
    volatile uint32_t RESERVED4[4];
    volatile uint32_t TXDSENT;
    volatile uint32_t RESERVED5;
    volatile uint32_t ERROR;
    volatile uint32_t RESERVED6[4];
    volatile uint32_t BB;
    volatile uint32_t RESERVED7[49];

    //registers
    volatile uint32_t SHORTS;
    volatile uint32_t RESERVED8[63];
    volatile uint32_t INTEN;
    volatile uint32_t INTENSET;
    volatile uint32_t INTENCLR;
    volatile uint32_t RESERVED9[110];
    volatile uint32_t ERRORSRC;
    volatile uint32_t RESERVED9[14];
    volatile uint32_t ENABLE;
    volatile uint32_t RESERVED10;
    volatile uint32_t PSELSCL;
    volatile uint32_t PSELSDA;
    volatile uint32_t RESERVED11[2];
    volatile uint32_t RXD;
    volatile uint32_t TXD;
    volatile uint32_t RESERVED12;
    volatile uint32_t FREQUENCY;
    volatile uint32_t RESERVED13[24];
    volatile uint32_t ADDRESS;




} NRF_TWI_REG;