
 #include "insilico/core.hpp"

#include "N_AL_PN.hpp"
#include "N_AL_LN.hpp"

#include <boost/numeric/odeint.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <omp.h>
#include <boost/numeric/odeint/external/openmp/openmp.hpp>

using namespace boost;
using namespace insilico;
using namespace std;

int main(int argc, char **argv) {
  // Commands to keep track of particular variables
  configuration::initialize(argc, argv);
  configuration::observe("v");  
  //configuration::observe_synapse(45,"gsyn");
  //configuration::observe_synapse(3100,"gsyn_slow"); 
  
  // Generate 90 PN neuron
  engine::generate_neuron<PN>(90);

  // Generate 30 LN neuron
  engine::generate_neuron<LN>(30);

  // Generate 1 S_nACH synapse between our neurons
  engine::generate_synapse<S_nACH>(1334);

  // Generate 1 S_2GABAA synapse between our neurons
  engine::generate_synapse<S_2GABAA>(1760);
  
  // Generate 1 S_slowGABA synapse between our neurons
  engine::generate_synapse<S_slowGABA>(1330);
 
  typedef numeric::odeint::runge_kutta4<
                      state_type , double ,
                      state_type , double ,
                      boost::numeric::odeint::openmp_range_algebra
                    >elr;
                    
                    
  omp_set_num_threads(6);

  //allowing the system to evolve
   state_type variables = engine::get_variables();
   integrate_const(elr(), engine::driver(), variables,
   0.0, 3000.0, 0.01, configuration::observer());
  configuration::finalize();
}


//#include "insilico/neuron/helper/spike_list.hpp"
//#include "S_nACH.hpp"
  //configuration::observe("ma");        
  //configuration::observe("ha");        
  //configuration::observe("I_A_ANTENNAL_LOBE_PN_HH");        
  //configuration::observe("I_KLEAK_ANTENNAL_LOBE_PN_HH");        

  // Generate 0 S_nACH synapse between our PN neurons
  //engine::generate_synapse<S_nACH>(0);

  // Spike list maintains neuron specific spike information (for the number of neurons in brackets)
  //engine::spike_list.resize(1);

