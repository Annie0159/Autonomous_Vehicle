#include "xil_printf.h"
#include "xparameters.h"
#include "PmodDHB1.h"
#include "PmodMAXSONAR.h"
#include "sleep.h"
#include <xgpio_l.h>

#define LS1_BASEADDR        XPAR_XGPIO_2_BASEADDR           // JA 
#define SONAR_BASEADDR      XPAR_PMOD_DUAL_MAXSONAR_0_BASEADDR


#define DHB1_GPIO_BASEADDR  0x44A10000  
#define DHB1_PWM_BASEADDR   0x44A30000  
#define LED_BASEADDR    0x40000000

#define LEFT_SENSOR_BIT     0x01    
#define RIGHT_SENSOR_BIT    0x02    

// Speed settings
#define BASE_SPEED          27
#define TURN_SPEED          7
#define OBSTACLE_STOP_CM    7

#define FORWARD   1
#define BACKWARD  0

PmodDHB1 motor;
PmodMAXSONAR sonar;

int main()
{
    // === LS1 
    volatile u32 *SensorData     = (u32 *)(LS1_BASEADDR + XGPIO_DATA_OFFSET);
    volatile u32 *SensorTristate = (u32 *)(LS1_BASEADDR + XGPIO_TRI_OFFSET);
    *SensorTristate = 0x0F;

    volatile u32 *LedData;


    LedData = (u32 *)(LED_BASEADDR + XGPIO_DATA_OFFSET);
    *(u32 *)(LED_BASEADDR + XGPIO_TRI_OFFSET) = 0x0;

    // === MOTOR INITIALIZATION 
    DHB1_begin(&motor,
               DHB1_GPIO_BASEADDR,   
               DHB1_PWM_BASEADDR,    
               XPAR_CPU_CORE_CLOCK_FREQ_HZ,
               100000);              

    DHB1_motorEnable(&motor);       

    // === SONAR 
    MAXSONAR_begin(&sonar, SONAR_BASEADDR, XPAR_CPU_CORE_CLOCK_FREQ_HZ);
    DHB1_setDirs(&motor, FORWARD, FORWARD);

    xil_printf("\n\nROBOT FULLY WORKING — MOTORS FIXED!\r\n");

    while (1)
    {
        u32 raw_value = *SensorData & 0x0F;
        int left_on   = (raw_value & LEFT_SENSOR_BIT);
        int right_on  = (raw_value & RIGHT_SENSOR_BIT);
        u32 dist_cm   = MAXSONAR_getDistance(&sonar);   

        // === OBSTACLE STOP ===
        if (dist_cm > 0 && dist_cm < OBSTACLE_STOP_CM) {
            *LedData = 0xF;
            // DHB1_motorDisable(&motor);
            DHB1_setMotorSpeeds(&motor, 0, 0);
            xil_printf("OBSTACLE %3d cm → STOP!     Raw=0x%01X\r\n", dist_cm, raw_value);
            usleep(50000);
            continue;
        }

        // === LINE FOLLOWING 
        xil_printf("Raw=0x%01X [L=%d R=%d] Dist=%3d cm → ", 
                   raw_value, left_on?1:0, right_on?1:0, dist_cm);

        if (left_on && !right_on) {
            DHB1_setDirs(&motor, FORWARD, FORWARD);
            // DHB1_setMotorSpeeds(&motor, TURN_SPEED, BASE_SPEED + 30);
            DHB1_setMotorSpeeds(&motor, TURN_SPEED, BASE_SPEED+5);
            xil_printf("TOO RIGHT → TURN LEFT\r\n");

        } else if (!left_on && right_on) {
            DHB1_setDirs(&motor, FORWARD, FORWARD);
            // DHB1_setMotorSpeeds(&motor, BASE_SPEED + 30, TURN_SPEED);
            DHB1_setMotorSpeeds(&motor, BASE_SPEED+5, TURN_SPEED);

            xil_printf("TOO LEFT  → TURN RIGHT\r\n");

        } else if (left_on && right_on) {
            DHB1_setMotorSpeeds(&motor, 0, 0);
            xil_printf("CENTERED → STRAIGHT\r\n");

        } else {
            DHB1_setMotorSpeeds(&motor, BASE_SPEED , BASE_SPEED);
            xil_printf("LINE LOST → SLOW\r\n");
        }
        xil_printf("Speed: %d\r\n");
        *LedData = 0x0;
        usleep(700);
    }
}