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

#ifndef __APM_H__
#define __APM_H__

#include "../../mod_chardevice/chardevice.h"


struct apm_fdm {
    double timestamp; // seconds
    double latitude, longitude; // degrees
    double altitude;  // MSL
    double heading;   // radians
    double speedN, speedE, speedD; // m/s
    double xAccel, yAccel, zAccel;       // m/s/s in body frame
    double rollRate, pitchRate, yawRate; // rad/s in body frame
    double roll, pitch, yaw;    // euler angles, radians
    double airspeed; // m/s
};

struct apm_input {
    float roll_rate;  // -0.5 .. 0.5
    float pitch_rate; // -0.5 .. 0.5
    float throttle;   //  0.0 .. 1.0
    float yaw_rate;   // -0.5 .. 0.5
    float col_pitch;  // -0.5 .. 0.5
};

class APM : BufferedCharDevice
{
public:
  void put_state_data(struct apm_fdm *state);
  bool get_input(struct apm_input *input);
  
  APM(char* device)
  {
    init(device, false);
  }

  ~APM()
  {
    cleanup();
  }
};

#endif //__APM_H__
