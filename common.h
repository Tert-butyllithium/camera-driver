#ifndef CAMDRIVER_COMMON_H
#define CAMDRIVER_COMMON_H
#define __MY_KMOD__ 1

#ifdef __MY_KMOD__
#include <linux/io.h>
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/module.h> /* Needed by all modules */
#include <linux/string.h>

#define printf printk

#else
#include <stdio.h>
#include <string.h>
#endif

#endif