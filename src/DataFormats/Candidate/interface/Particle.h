#ifndef _Particle_H_
#define _Particle_H_

namespace reco {
class Particle {
  public:
    typedef char Charge;
    class LorentzVector 
    {
      public:
        LorentzVector ( double px, double py, double pz, double E );
        double x() const;
        double y() const;
        double z() const;
        double t() const;
      private:
        double x_;
        double y_;
        double z_;
        double t_;
    };
    /** old constructor that mimics cmssw-1.1.x ParticleKinematics */
    Particle ( double px, double py, double pz, double E );

    /* mimics cmssw-1.2.x Particle */
    Particle ( char charge, LorentzVector p );
    Particle ();
    double px() const;
    double py() const;
    double pz() const;
    double pt() const;
    double p() const;
    double eta() const;
  private:
    double px_;
    double py_;
    double pz_;
    double E_;
};
}

#endif 
