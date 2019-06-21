#include <ch.h>
#include <hal.h>

#include <chprintf.h>
#include <errno.h>

#define STEPPER_MOTOR_HRZ 1
#define STEPPER_MOTOR_VRT 2

#define STEPPER_MOTOR_DIRECTION_RIGTH 1
#define STEPPER_MOTOR_DIRECTION_LEFT 2

#define EOK 0



/*Controll setting*/
typedef struct
{

  uint8_t motor_index;
  uint8_t motor_direction;

  uint16_t value_of_step;
}motor_state;





/*Serial setting*/
static const SerialConfig sdcfg = {
  .speed = 115200,          /* baudrate, directly number */
  .cr1 = 0,
  .cr2 = 0,
  .cr3 = 0
};

SerialDriver *comm_dr = &SD2;

typedef struct
{

  uint8_t motor_index;
  uint8_t motor_direction;

  uint16_t value_of_step;
}pkg_serial;





void StepperMotorInit(void)
{
  palSetPadMode(GPIOA,3,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOC,0,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOC,3,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOF,3,PAL_MODE_OUTPUT_PUSHPULL);
}



void SerialCommInit()
{
  palSetPadMode( GPIOD, 5, PAL_MODE_ALTERNATE(7) );   // TX = PD_5
  palSetPadMode( GPIOD, 6, PAL_MODE_ALTERNATE(7) );   // RX = PD_6

  sdStart( comm_dr, &sdcfg );
}


int SerialCommGetPkg(pkg_serial *p_pkg)
{
  if (p_pkg == NULL) return EINVAL;

  msg_t msg = sdGetTimeout(comm_dr,MS2ST(10));
  if (msg<0)
  {
    return EIO;
  }

  char start_byte = msg;
  if (start_byte == '#')
  {
    uint8_t rcv_buffer[4];
    int32_t rcv_bytes = sizeof(rcv_buffer);

    msg = sdReadTimeout(comm_dr, rcv_buffer, rcv_bytes, MS2ST(10));

    if(msg != rcv_bytes)
    {
      return EIO;
    }

    p_pkg->motor_index = rcv_buffer[0];
    p_pkg->motor_direction = rcv_buffer[1];
    p_pkg->value_of_step = (rcv_buffer[2]<<8)|(rcv_buffer[3]);

  }

  return EOK;
}








void SetTheMotorValueHorizontally(motor_state horizontal_motor)
{
  if(horizontal_motor.motor_direction == STEPPER_MOTOR_DIRECTION_RIGTH)
  {
    palSetPad(GPIOF,3);
  }
  else if(horizontal_motor.motor_direction == STEPPER_MOTOR_DIRECTION_LEFT)
  {
    palClearPad(GPIOF,3);
  }

  for(int i=0;i<horizontal_motor.value_of_step;++i)
  {
    palSetPad(GPIOC,0);
    chThdSleepMilliseconds(1);
    palClearPad(GPIOC,0);
    chThdSleepMilliseconds(1);
  }

}



void SetTheMotorValueVertically(motor_state vertical_motor)
{
  if(vertical_motor.motor_direction == STEPPER_MOTOR_DIRECTION_RIGTH)
  {
    palSetPad(GPIOC,3);
  }
  else if(vertical_motor.motor_direction == STEPPER_MOTOR_DIRECTION_LEFT)
  {
    palClearPad(GPIOC,3);
  }

  for(int i=0;i<vertical_motor.value_of_step;++i)
  {
    palSetPad(GPIOA,3);
    chThdSleepMilliseconds(1);
    palClearPad(GPIOA,3);
    chThdSleepMilliseconds(1);
  }
}


void UpdateStateMotor(motor_state new_state)
{
  if(new_state.motor_index == STEPPER_MOTOR_HRZ)
  {
    SetTheMotorValueHorizontally(new_state);
  }
  else if(new_state.motor_index == STEPPER_MOTOR_VRT)
  {
    SetTheMotorValueVertically(new_state);
  }
}




static THD_WORKING_AREA(waThread, 128);
static THD_FUNCTION(Thread, arg) 
{
    arg = arg;

    while (true)
    {
        chThdSleepSeconds(1);
    }
}




int main(void)
{
    /* RT Core initialization */
    chSysInit();
    /* HAL (Hardware Abstraction Layer) initialization */
    halInit();

    chThdCreateStatic(waThread, sizeof(waThread), NORMALPRIO, Thread, NULL /* arg is NULL */);

    StepperMotorInit();
    motor_state now_state_motor;

    SerialCommInit();
    pkg_serial input_pkg;

    while (true)
    {
//        int result = SerialCommGetPkg(&input_pkg);
//
//        if (result != EOK)
//        {
//          continue;
//        }
///*
//        if(input_pkg.motor_index == STEPPER_MOTOR_HRZ && input_pkg.motor_direction == STEPPER_MOTOR_DIRECTION_RIGTH && input_pkg.value_of_step == 1000)
//        {
//          palToggleLine(LINE_LED1);
//        }
//
//        else if(input_pkg.motor_index == STEPPER_MOTOR_VRT && input_pkg.motor_direction == STEPPER_MOTOR_DIRECTION_LEFT && input_pkg.value_of_step == 300)
//        {
//          palToggleLine(LINE_LED2);
//        }

        now_state_motor.motor_index = 1;
        now_state_motor.motor_direction = 2;
        now_state_motor.value_of_step = 5;



        UpdateStateMotor(now_state_motor);
        chThdSleepMilliseconds(500);
    }
}
