#ifndef INCLUDED_batheliersynapsefacilitate_HPP
#define INCLUDED_batheliersynapsefacilitate_HPP

#include "insilico/core/engine.hpp"
#include <cmath>

namespace insilico {


class GABA1 : public Synapse {
 public:
  void ode_set(state_type &variables, state_type &dxdt, const double t, unsigned index) {
    

    
    double xtg=0.0,xtp=0.0;
    int event=0;
	
   

  double factor= engine::synapse_value(index,"factor");
  double p0=engine::synapse_value(index,"p0");
  double deltap=engine::synapse_value(index,"deltap");
  double maxp=engine::synapse_value(index,"maxp");

    // synapse logic for delay for recently spiked neuron
    double tauf=engine::synapse_value(index,"tauf");
    
    //double tevent=engine::synapse_value(index,"tevent");
    
  double delay = engine::synapse_value(index, "delay");
   
  int g1_index = engine::synapse_index(index,"g1");
  int g2_index = engine::synapse_index(index,"g2");
	int p_index=engine::synapse_index(index,"p");

  double g1 = variables[g1_index];
  double g2 = variables[g2_index];
	double p=variables[p_index];
  double spikecount;
  int pre_neuron_index = engine::synapse_value(index, "pre");
  double last_spiked = engine::neuron_value(pre_neuron_index, "last_spike");
   
  double tau1 = engine::synapse_value(index, "taurise");
  double tau2 = engine::synapse_value(index, "taudecay");

    //std::cout<<"time,"<<t<<"\n";
    
    
    if ((t-last_spiked-delay) > 0 && (t-last_spiked-delay) < 0.02)
    {
	xtg=1.0;
    }

    if ((t-last_spiked-delay) > 0 && (t-last_spiked-delay) < 0.027)
    {
        spikecount=1;
       
    }
    else
    {
      spikecount=0;
    }
    
   
    
  // ODE set
    dxdt[g1_index] = g2;
    dxdt[g2_index] = (factor * xtg * (tau1*tau2) - g1 - (tau1+tau2) * g2) / (tau1*tau2);
    dxdt[p_index]=-((p-p0)/tauf)+(deltap*(maxp-p)*spikecount);
  
}
    };
    }


// class S_DefaultSynapse

// insilico

#endif
