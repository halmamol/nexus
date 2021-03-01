// ----------------------------------------------------------------------------
// nexus | MigdalSignalGenerator.h
//
// This class is the a generator of the Migdal Signal from ** in the detector.
// Particle propiertes are taken from an external file defined in the configuration
// file. Probabilites are evaluated in this code. 
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

    /// Generate a random kinetic energy with flat probability in
    //  the interval [energy_min, energy_max].
    G4double RandomEnergy() const;
    
    
    /// Open the Decay0 input file selected by the user
    void OpenInputFile(G4String);
    //void ProcessHeader();

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
