

#ifndef INCLUDED_S_slowGABA_HPP
#define INCLUDED_S_slowGABA_HPP

#include "insilico/core/engine.hpp"


namespace insilico {
class S_slowGABA: public Synapse {
 public:
  void ode_set(state_type& variables, state_type& dxdt, const double t, unsigned index) {
    
    unsigned g2_index = engine::synapse_index(index, "g2"); // obtain initial value for g1 from ssets.
    unsigned g3_index = engine::synapse_index(index, "g3"); // obtain initial value for g2 from ssets.
    unsigned pre_neuron_index = engine::synapse_value(index, "pre"); //obtain pre-neuron index from ssets.
    double v_index = engine::neuron_index(pre_neuron_index,"v"); //obtain the voltage of the pre-neuron

    double g2 = variables[g2_index];
    double g3 = variables[g3_index];
    double v = variables[v_index];

    //Constants
    double r1 = 0.5;
    double r2 = 0.0013;
    double r3 = 0.1;
    double r4 = 0.0033;
    double vthresh = -20.0;
    double sig  = 1.5;

    double T = 1.0/(1.0 + exp(- (v - vthresh)/sig));
   
   // std::cout << "T : " << T << std::endl;

    // ODE set
   // dxdt[g1_index] = (Alpha * T * (1.0 - g1) - Beta * g1);
    dxdt[g2_index] = r1 * T * (1 - g3) - r2 * g3;
    dxdt[g3_index] = r3 * g3 - r4 * g2;
    engine::synapse_value(index, "g2", g2);
    engine::synapse_value(index, "g3", g3);


  } // function ode_set
    
}; // class S_2GABAA

} // insilico

#endif


