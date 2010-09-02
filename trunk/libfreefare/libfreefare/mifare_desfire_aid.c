/*-
 * Copyright (C) 2010, Romain Tartiere.
 * 
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 * 
 * $Id$
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <freefare.h>
#include "freefare_internal.h"

// FIXME Theorically, it should be an uint24_t ...
MifareDESFireAID
mifare_desfire_aid_new (uint32_t aid)
{
    MifareDESFireAID res;

    if ((res = malloc (sizeof (*res)))) {
        // XXX We may take care of endianess
        memcpy(res->data, ((uint8_t*)&aid), 3);
    }

    return res;
}

// XXX This function ease the MifareDESFireAID creation using a Mifare Classic AID (see MIFARE Application Directory document - section 3.10 MAD and MIFARE DESFire)
MifareDESFireAID
mifare_desfire_aid_new_with_mad_aid (MadAid mad_aid, uint8_t n)
{

    MifareDESFireAID res;

    if (n & 0xf0)
	return errno = EINVAL, NULL;

    if ((res = malloc (sizeof (*res)))) {
	res->data[0] = 0xf0 | (mad_aid.function_cluster_code >> 4);
	res->data[1] = (uint8_t) (((mad_aid.function_cluster_code & 0x0f) << 4) | ((mad_aid.application_code & 0xf0) >> 4));
	res->data[2] = ((mad_aid.application_code & 0x0f) << 4) | n;
    }

    return res;
}
