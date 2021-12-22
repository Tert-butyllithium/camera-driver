#pragma once

#ifdef __KERNEL__
#include <linux/io.h>
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/module.h> /* Needed by all modules */

#define printf printk

#else
#include <stdio.h>
#endif
