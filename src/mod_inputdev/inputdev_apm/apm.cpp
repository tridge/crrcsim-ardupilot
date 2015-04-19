/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "apm.h"

void APM::put_state_data(struct apm_fdm *apm_fdm)
{
    charDevice->write((const void*)apm_fdm, sizeof(*apm_fdm));
}

bool APM::get_input(struct apm_input *apm_input)
{
    return charDevice->read((void*)apm_input, sizeof(*apm_input)) == sizeof(*apm_input);
}
