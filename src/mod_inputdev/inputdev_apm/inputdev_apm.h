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

#ifndef TX_INTERFACE_APM_H
#define TX_INTERFACE_APM_H

#include "apm.h"
#include "../inputdev.h"
#include "../../mod_misc/SimpleXMLTransfer.h"

class T_TX_InterfaceAPM : public T_TX_Interface
{
  public:
   T_TX_InterfaceAPM();
   virtual ~T_TX_InterfaceAPM();
   
   /**
    * Get input method
    */
   virtual int inputMethod() { return(T_TX_Interface::eIM_apm); }
   
   /**
    * Initialize interface. Read further config data from a file, if necessary.
    */
   int init(SimpleXMLTransfer* config);
   
   /**
    * Write configuration back
    */
   virtual void putBackIntoCfg(SimpleXMLTransfer* config);
      
   /**
    * Get current input data. If some value is not available, the value 
    * is not overwritten and false is returned. In order to support
    * lock-step scheduling with ArduPilot this function blocks until
    * new input is available
    *
    * This function also outputs the current FDM state to APM
    */
   bool getInputData(TSimInputs* inputs);
   
private:
   APM*              input;
   uint8_t           reverse; // channel reverse flags
   std::string       device;
};

#endif
