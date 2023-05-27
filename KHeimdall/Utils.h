#pragma once
#include "stdafx.h"
#include "Heimdall.h"

#define	BREAKABLE_SECTION_START	do {
#define BREAKABLE_SECTION_END	} while (FALSE);

#define HmdlAllocatePool(p, s) ExAllocatePoolWithTag(p, s, HMDL_POOL)
#define HmdlFreePool(p) ExFreePoolWithTag(p, HMDL_POOL)