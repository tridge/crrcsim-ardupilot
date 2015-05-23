/*
 * APM input interface
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 (or later) as published by the Free Software Foundation.
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
#include "../../crrc_main.h"
#include "../../global.h"
#include "../../aircraft.h"
#include "../../SimStateHandler.h"
#include "../../mod_fdm/fdm.h"
#include "../../mod_misc/ls_constants.h"
#include "inputdev_apm.h"

#include <stdio.h>

#define PI 3.141592653589793
#define FEET2METERS 0.3048

#pragma GCC optimize("O0")

T_TX_InterfaceAPM::T_TX_InterfaceAPM()
{
    input = NULL;
}

T_TX_InterfaceAPM::~T_TX_InterfaceAPM()
{
    if (input != NULL)
        delete input;
}

int T_TX_InterfaceAPM::init(SimpleXMLTransfer* config)
{
    char devicestr[100];
    
    T_TX_Interface::init(config);
    
    device   = config->getString("inputMethod.apm.device", "udpserver,127.0.0.1/0.0.0.0,9002");
    strncpy(devicestr, device.c_str(), 100); 
    devicestr[99] = '\0';
    input = new APM(devicestr);
    reverse = 0;
    
    return 0;
}

void T_TX_InterfaceAPM::putBackIntoCfg(SimpleXMLTransfer* config)
{
    T_TX_Interface::putBackIntoCfg(config);
    config->setAttributeOverwrite("inputMethod.apm.device",   device);  
}

#define degrees(r) ((r)*RAD_TO_DEG)

bool T_TX_InterfaceAPM::getInputData(TSimInputs* inputs)
{
    struct apm_input apm_input;
    struct apm_fdm apm_fdm;

    // Read input data
    if (!input->get_input(&apm_input)) {
        // no input available
        return false;
    }

    inputs->elevator = apm_input.pitch_rate;
    inputs->aileron  = apm_input.roll_rate;
    inputs->throttle = apm_input.throttle;
    inputs->rudder   = apm_input.yaw_rate;
    inputs->pitch    = apm_input.col_pitch;
  
    if (Global::aircraft->getFDM() == NULL) {
        return true;
    }

    CRRCMath::Vector3 vel, waccel, accel, pqr;

    vel    = Global::aircraft->getFDM()->getVel();
    waccel = Global::aircraft->getFDM()->getAccel();
    waccel.r[2] -= 9.80665 / FEET2METERS; // include acceleration due to gravity
    pqr    = Global::aircraft->getFDM()->getPQR();

    accel = Global::aircraft->getFDM()->WorldToBody(waccel);

    apm_fdm.timestamp = Global::dt * Global::Simulation->SimSteps();
    apm_fdm.latitude = degrees(Global::aircraft->getFDM()->getLat());
    apm_fdm.longitude = degrees(Global::aircraft->getFDM()->getLon());
    apm_fdm.altitude = Global::aircraft->getFDM()->getAlt() * FEET2METERS;

    apm_fdm.heading = atan2(-vel.r[1], vel.r[0]);
    apm_fdm.speedN = vel.r[0] * FEET2METERS;
    apm_fdm.speedE = vel.r[1] * FEET2METERS;
    apm_fdm.speedD = vel.r[2] * FEET2METERS;
    apm_fdm.xAccel = accel.r[0] * FEET2METERS;
    apm_fdm.yAccel = accel.r[1] * FEET2METERS;
    apm_fdm.zAccel = accel.r[2] * FEET2METERS;

    apm_fdm.rollRate = pqr.r[0];
    apm_fdm.pitchRate = pqr.r[1];
    apm_fdm.yawRate = pqr.r[2];

    apm_fdm.roll = Global::aircraft->getFDM()->getPhi();
    apm_fdm.pitch = Global::aircraft->getFDM()->getTheta();
    apm_fdm.yaw = Global::aircraft->getFDM()->getPsi();

    apm_fdm.airspeed = Global::aircraft->getFDM()->getVRelAirmass() * FEET2METERS;

    input->put_state_data(&apm_fdm);
  
    return true;
}
