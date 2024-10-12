/*
** Copyright (C) 2022 Optek. All rights reserved.
**
** The information and source code contained herein is the exclusive
** property of Mediamatics and may  not be disclosed, examined or
** reproduced in whole or in part without explicit written authorization
** from the company.
*/

/*
** Author:optek
**
** Date: 2023-10-27
**
** Description: Simple version tracking.
** 
*/


#include "sdk_ver.h"

const char sdk_ver_date[] = __DATE__;
const char sdk_time_date[] = __TIME__;

/*********date: 2024-04-16*********/
const char sdk_version[] = "0.0.0.a20"; //modify 2024-0829_1620

const char *sdk_version_get(void)
{
    return sdk_version;
}
const char *sdk_date_get(void)
{
    return sdk_ver_date;
}

const char *sdk_time_get(void)
{
    return sdk_time_date;
}


