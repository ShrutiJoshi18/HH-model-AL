/*
 synapse/S_2GABAA

 Copyright (C) 2014 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



//EDITTED BY ADITHYA RAJAGOPALAN ON 14.1.2016


#ifndef INCLUDED_S_2GABAA_HPP
#define INCLUDED_S_2GABAA_HPP

#include "insilico/core/engine.hpp"


namespace insilico {
class S_2GABAA: public Synapse {
 public:
  void ode_set(state_type& variables, state_type& dxdt, const double t, unsigned index) {
    

    double F = 1; 
    double F_i = 1;
    double t_i = 0;//check once, probably wrong

    unsigned g1_index = engine::synapse_index(index, "g1"); // obtain initial value for g1 from ssets.
    unsigned pre_neuron_index = engine::synapse_value(index, "pre"); //obtain pre-neuron index from ssets.
    double v_index = engine::neuron_index(pre_neuron_index,"v"); //obtain the voltage of the pre-neuron
    

    /*unsigned gsyn_index = engine::synapse_index(index, "gsyn");
    double gsyn = variables[gsyn_index];*/

    double gsyn = engine::synapse_value(index, "gsyn");
    
    double g1 = variables[g1_index];
    double v = variables[v_index];
   
    double t_0 = engine::neuron_value(pre_neuron_index, "last_spike");

    //Constants
    double Alpha = 10.0;
    double Beta = 0.2;
    double vthresh = -20.0;
    double sig  = 1.5;
    double tau = 10;
    double dF = 0.15;

    //Facilitation 
    if (t == t_0) 
    {
    engine::synapse_value(index, "F_i", F);
    engine::synapse_value(index, "t_i", t);
    }
    
    F = 1 + (F_i + dF -1)*exp(-(t-t_i)/tau);
    gsyn *= F; 

    engine::synapse_value(index, "gsyn", gsyn); //update gsyn

    double T = 1.0/(1.0 + exp(- (v - vthresh)/sig));
   
   // std::cout << "T : " << T << std::endl;

    // ODE set
    dxdt[g1_index] = (Alpha * T * (1.0 - g1) - Beta * g1);

    engine::synapse_value(index, "g1", g1);


  } // function ode_set
    
}; // class S_2GABAA

} // insilico

#endif



