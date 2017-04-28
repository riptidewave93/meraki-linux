/*
 * motor_ioctl.h
 *
 * Copyright (c) 2011 Vivotek, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* ============================================================================= */
#ifndef _MOTOR_IOCTL_H_
#define _MOTOR_IOCTL_H_

/* user 'M' as magic number M=0x4D */
#define MOTOR_MAGIC          'M'

/* Set the motor forward direction */
#define MOTOR_SET_FORWARD    _IO (MOTOR_MAGIC, 0)
/* Set the motor backward direction */
#define MOTOR_SET_BACKWARD      _IO (MOTOR_MAGIC, 1)

/* Set the speed value of motor */
#define MOTOR_SET_SPEED         _IOW (MOTOR_MAGIC, 2, unsigned int)
/* Set the walk steps value of motor */
#define MOTOR_WALK_STEPS     _IOW (MOTOR_MAGIC, 3, unsigned int)
/* Clear the current walk steps value of motor */
#define MOTOR_WALK_STEPS_CLR    _IO (MOTOR_MAGIC, 4)

/* Turn off motors for thermal issue */
#define MOTOR_TURN_OFF       _IO (MOTOR_MAGIC, 5)

/* Set the debug message level of driver status */
#define MOTOR_SET_MSG_LEVEL     _IOW (MOTOR_MAGIC, 11, unsigned int)
/* List the current system log of driver status */
#define MOTOR_SYS_LOG_LIST      _IOW (MOTOR_MAGIC, 12, unsigned int)

/* The max number of ioctl's */
#define MOTOR_MAXNR 13

#endif // _MOTOR_IOCTL_H_
