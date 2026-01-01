#include "rc522_debug.h"
#include "rc522.h"
#include <stdint.h>
#include "stm32f446xx.h"

void rc522_debug(void){

    volatile uint8_t command = rc522_read_reg(RC522_COMMAND_REG);
    (void)command;
    
    volatile uint8_t comirq = rc522_read_reg(RC522_COMIRQ_REG);
    (void)comirq;
    
    volatile uint8_t comien = rc522_read_reg(RC522_COMIEN_REG);
    (void)comien;
    
    volatile uint8_t error = rc522_read_reg(RC522_ERROR_REG);
    (void)error;
    
    volatile uint8_t divirq = rc522_read_reg(RC522_DIVIRQ_REG);
    (void)divirq;
    
    volatile uint8_t divien = rc522_read_reg(RC522_DIVIEN_REG);
    (void)divien;
    
    volatile uint8_t status1 = rc522_read_reg(RC522_STATUS1_REG);
    (void)status1;
    
    volatile uint8_t status2 = rc522_read_reg(RC522_STATUS2_REG);
    (void)status2;
    
    volatile uint8_t fifolevel = rc522_read_reg(RC522_FIFOLEVEL_REG);
    (void)fifolevel;
    
    volatile uint8_t bitframing = rc522_read_reg(RC522_BITFRAMING_REG);
    (void)bitframing;
    
    volatile uint8_t mode = rc522_read_reg(RC522_MODE_REG);
    (void)mode;
    
    volatile uint8_t txmode = rc522_read_reg(RC522_TXMODE_REG);
    (void)txmode;
    
    volatile uint8_t rxmode = rc522_read_reg(RC522_RXMODE_REG);
    (void)rxmode;
    
    volatile uint8_t txcontrol = rc522_read_reg(RC522_TXCONTROL_REG);
    (void)txcontrol;
    
    volatile uint8_t txask = rc522_read_reg(RC522_TXASK_REG);
    (void) txask;

    volatile uint8_t tmode = rc522_read_reg(RC522_T_MODE_REG);
    (void)tmode;
    
    volatile uint8_t tprescaler = rc522_read_reg(RC522_T_PRESCALER_REG);
    (void)tprescaler;
    
    volatile uint8_t treloadh = rc522_read_reg(RC522_T_RELOAD_H_REG);
    (void)treloadh;
    
    volatile uint8_t treloadl = rc522_read_reg(RC522_T_RELOAD_L_REG);
    (void)treloadl;
    
    volatile uint8_t tcountervalh = rc522_read_reg(RC522_T_COUNTERVAL_H_REG);
    (void)tcountervalh;
    
    volatile uint8_t tcountervall = rc522_read_reg(RC522_T_COUNTERVAL_L_REG);
    (void)tcountervall;
}
