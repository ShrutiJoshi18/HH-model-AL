// Created by Shreya Lakhera on 7 February 2018 

#ifndef INCLUDED_N_AL_LN_HH_HPP
#define INCLUDED_N_AL_LN_HH_HPP

#include "insilico/core/engine.hpp"
#include "insilico/neuron/helper/spike_list.hpp"
#include "insilico/core/injector.hpp"

#include "I_K_AL_LN.hpp"
#include "I_CA_AL_LN.hpp"
#include "I_KCA_AL_LN.hpp"
#include "CA_DRIVE_AL_LN.hpp"
#include "I_LEAK_AL_LN.hpp"
#include "I_KLEAK_AL_LN.hpp"
#include "S_2GABAA.hpp"
#include "S_slowGABA.hpp"
#include "S_nACH.hpp"

#include <random>
#include <vector>

namespace insilico {

class LN : public Neuron {
 public:
  void ode_set(state_type &variables, state_type &dxdt,const double t, unsigned index) {

    std::vector<unsigned> g1_indices;
    //std::vector<double> gsyn_values;
    std::vector<unsigned> gsyn_indices;
    std::vector<double> esyn_values;

    std::vector<unsigned> g2_indices;
    std::vector<unsigned> g3_indices;
    std::vector<double> ek_values;
    //std::vector<double> gsyn_slow_values;
    std::vector<unsigned> gsyn_slow_indices;
    /*
    std::vector<unsigned> g2_indices;
    std::vector<double> gsyn2_values;
    std::vector<double> esyn2_values;
    */
    std::vector<double> I_Ext;

    unsigned v_index = engine::neuron_index(index, "v");
    double v = variables[v_index];
   // std::cout << "v_index" << index <<" : " << v_index<< std::endl;

    // incoming synaptic currents
    double I_syn = 0;

    g1_indices = engine::get_pre_neuron_indices(index, "g1");
    gsyn_indices = engine::get_pre_neuron_indices(index, "gsyn");
    //gsyn_values = engine::get_pre_neuron_values(index, "gsyn");
    esyn_values = engine::get_pre_neuron_values(index, "esyn");
    
    for(unsigned iterator = 0; iterator < g1_indices.size(); ++iterator) {
     // I_syn += variables[g1_indices[iterator]] * gsyn_values[iterator] * (v - esyn_values[iterator]);
        I_syn += variables[g1_indices[iterator]] * gsyn_indices[iterator] * (v - esyn_values[iterator]);
    }

   // std::cout << "g1_indices_size" << index <<" : " << g1_indices.size()<< std::endl;
    
    engine::neuron_value(index, "I_syn", I_syn);
    //engine::neuron_value(index, "I_nACH", I_nACH);



    //Synaptic slow GABA current 
    double I_GABA = 0;
   // F_slow = engine::get_pre_neuron_indices(index, "F_slow");
    g2_indices = engine::get_pre_neuron_indices(index, "g2");
    g3_indices = engine::get_pre_neuron_indices(index, "g3");
    gsyn_slow_indices = engine::get_pre_neuron_indices(index, "gsyn_slow");
    //gsyn_slow_values = engine::get_pre_neuron_values(index, "gsyn_slow");
    ek_values = engine::get_pre_neuron_values(index, "ek");
    
    for(unsigned iterator = 0; iterator < g2_indices.size(); ++iterator) {
    //  I_GABA += pow(variables[g2_indices[iterator]],4)/(pow(variables[g2_indices[iterator]],4) + 100) * gsyn_slow_values[iterator] * (v - ek_values[iterator]);
       I_GABA += pow(variables[g2_indices[iterator]],4)/(pow(variables[g2_indices[iterator]],4) + 100) * gsyn_slow_indices[iterator] * (v - ek_values[iterator]);
    }
 
    engine::neuron_value(index, "I_GABA", I_GABA);




    //Adding the external current
    I_Ext = injector::external_current(index, t);

    double current = 0;
    if(!I_Ext.empty()) {
      current = I_Ext[0];
    }

    // ODE set   
    I_CA_LN_::current(variables, dxdt, t, index);
    CA_DRIVE_::current(variables, dxdt, t, index);
    I_KCA_LN_::current(variables, dxdt, t, index);    
    I_K_LN_::current(variables, dxdt, t, index);
    I_LEAK_LN_::current(variables, dxdt, t, index);
    I_KLEAK_LN_::current(variables, dxdt, t, index);
    
    double I_Ca = engine::neuron_value(index, "I_CA");
    double I_KCa = engine::neuron_value(index, "I_KCA");
    double I_K = engine::neuron_value(index, "I_K_LN");
    double I_l = engine::neuron_value(index, "I_LEAK_LN");
    double I_kl = engine::neuron_value(index, "I_KLEAK_LN");
    double I_ext = engine::neuron_value(index, "iext");

    dxdt[v_index] = I_ext + current - I_K - I_Ca - I_KCa - I_l - I_kl - I_syn - I_GABA;

  } // function ode_set
}; // class LN

} // insilico

#endif


    /*
    std::vector<std::vector<unsigned>> pre_synaptic_lists = engine::pre_synaptic_lists;
    std::cout << "size_presynapse_lists : " << pre_synaptic_lists[index].size()<< std::endl;
    
    unsigned i;
    unsigned idx;
    bool error = false;
    for (i=0; i<pre_synaptic_lists[index].size(); ++i) {
	std::cout << "psl_"<<i<<" : "<< pre_synaptic_lists[index][i]<< std::endl;
	idx = engine::synapse_index(pre_synaptic_lists[index][i], "g2", error);
	if (!error) { g2_indices.push_back(idx);	}
    }
    */
    /*
    if (index == 2) {
    	std::cout << "g2_size2 : " << g2_indices.size()<< std::endl;
    	std::cout << "g1_size2 : " << g1_indices.size()<< std::endl;
    	std::cout << "I_GABA2 : " << I_GABA << std::endl;
    	std::cout << "I_ACH2 : " << I_nACH << std::endl;
    }
    */
    /*
    double I_GABA = 0;
    
    g2_indices = engine::get_pre_neuron_indices(index, "g2");
    gsyn2_values = engine::get_pre_neuron_values(index, "gsyn_GABA");
    esyn2_values = engine::get_pre_neuron_values(index, "esyn_GABA");
    
    for(unsigned iterator = 0; iterator < g2_indices.size(); ++iterator) {
      I_GABA += variables[g2_indices[iterator]] * gsyn2_values[iterator] * (v - esyn2_values[iterator]);
    }
    */
    /*
    if (index == 7) {
    	std::cout << "g2_size7 : " << g2_indices.size()<< std::endl;
    	std::cout << "g1_size7 : " << g1_indices.size()<< std::endl;
    	std::cout << "I_GABA7 : " << I_GABA << std::endl;
    	std::cout << "I_ACH7 : " << I_nACH << std::endl;
    }

    if (index == 6) {
    	std::cout << "g2_size6 : " << g2_indices.size()<< std::endl;
    	std::cout << "g1_size6 : " << g1_indices.size()<< std::endl;
    	std::cout << "esyn1_size6 : " << esyn1_values.size()<< std::endl;
    	std::cout << "esyn2_size6 : " << esyn2_values.size()<< std::endl;
    	std::cout << "I_GABA6 : " << I_GABA << std::endl;
    	std::cout << "I_ACH6 : " << I_nACH << std::endl;
    }
    */
