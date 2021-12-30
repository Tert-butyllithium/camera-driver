#ifndef CAMDRIVER_COMMON_H
#define CAMDRIVER_COMMON_H

typedef unsigned char byte;

#ifdef __MY_KMOD__
#include <linux/io.h>
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/module.h> /* Needed by all modules */
#include <linux/string.h>
#include <linux/types.h>
#include <linux/delay.h>


#define printf printk

#else
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
#endif

#endif