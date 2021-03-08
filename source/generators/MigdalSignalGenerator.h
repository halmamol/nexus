// ----------------------------------------------------------------------------
// nexus | MigdalSignalGenerator.cc
//
// This class is the a generator of the Migdal Signal from arxiv:2009.05939 in the detector.
// Particle propiertes are taken from an external file defined in the configuration
// file. This file is generated with 'migdal4NEXT'. Probabilites are evaluated in this code. 
//
// The NEXT Collaboration
// ----------------------------------------------------------------------------

#ifndef MIGDAL_GENERATOR_H
#define MIGDAL_GENERATOR_H

#include <G4VPrimaryGenerator.hh>
#include <fstream>

class G4GenericMessenger;
class G4Event;
class G4ParticleDefinition;


namespace nexus {

  class BaseGeometry;

  class MigdalSignalGenerator: public G4VPrimaryGenerator
  {
  public:
    /// Constructor
    MigdalSignalGenerator();
    /// Destructor
    ~MigdalSignalGenerator();

    /// This method is invoked at the beginning of the event. It sets
    /// a primary vertex (that is, a particle in a given position and time)
    /// in the event.
    void GeneratePrimaryVertex(G4Event*);

  private:
  
    /// Open the input file selected by the user
    void OpenInputFile(G4String);
  private:
      
    G4int atomic_number_, mass_number_;
    G4double energy_level_;
    
    G4bool opened_;
        
    G4GenericMessenger* msg_;

    std::ifstream file_;
    G4ParticleDefinition*  particle_defXe_;
    G4ParticleDefinition*  particle_defME_;
    G4ParticleDefinition*  particle_defXray_;
    G4ParticleDefinition*  particle_defXraye_;

    const BaseGeometry* geom_; ///< Pointer to the detector geometry

    G4String region_;

  };

} // end namespace nexus

#endif
