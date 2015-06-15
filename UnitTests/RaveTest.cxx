#include "RaveTest.h"
#include <rave/Version.h>

RaveTest* RaveTest::fObject=0;

RaveTest* RaveTest::Get()
{ 
   if (!fObject){
      std::cout << "This is Rave Version " << rave::Version() << std::endl;
      fObject = new RaveTest();    
   }
   return fObject;
}


RaveTest::RaveTest(int nvtx) 
      : fTruthVtx(), fTrackList(), fStdCovMatrix(fNCov), norm_dist(0.,1.)
{ 
   Init();
   if (nvtx != 0) fNVtx = nvtx;
}

void RaveTest::InitVtx()
{
   fTruthVtx.clear();
   vector<double> null3(3);
   null3[0]=0;null3[1]=0;null3[2]=0;
   for (int i=0;i<fNVtx;i++) fTruthVtx.push_back(null3);
   if (fNVtx>=2) fTruthVtx[1][2]=0.8*fBeamspotSigmaXY;
   if (fNVtx>=3) fTruthVtx[2][1]=0.55*fBeamspotSigmaXY;
   if (fNVtx>=4) fTruthVtx[3][1]=-1.55*fBeamspotSigmaXY;
   for (int j=5;j<fNVtx;j++){
      fTruthVtx[j][0]=RndmGaus()*fBeamspotSigmaXY;
      fTruthVtx[j][1]=RndmGaus()*fBeamspotSigmaXY;
      fTruthVtx[j][2]=RndmGaus()*fBeamspotSigmaZ;
   }
}

void RaveTest::Init()
{
   DeclareProperty("BeamspotSigmaXY", &fBeamspotSigmaXY, 0.5);
   DeclareProperty("BeamspotSigmaZ", &fBeamspotSigmaZ, 2.);
   DeclareProperty("AvNtrk", &fAvNTrk, 10.);
   DeclareProperty("NVtx", &fNVtx, 2);
   DeclareProperty("Seed",   &fSeed, 2);
   DeclareProperty("Sigmax", &fSigmax,0.05);
   DeclareProperty("SigmapRaw", &fSigmapRaw, 0.1);
   DeclareProperty("Algorithm", &fAlgorithm, "avr");
   DeclareProperty("UseBeamspot", &fUseBeamspot, true);
   DeclareProperty("BField", &fBField, 1.0);
   DeclareProperty("MomentumScale", &fMomentumScale, 1.);
}

void RaveTest::InitRave()
{
   fSigmap = fSigmapRaw / fMomentumScale;
   fR.seed(fSeed);       
   fBObj = new rave::ConstantMagneticField(0.,0.,fBField);
   double eps = 1.e-6;   
   fBeamspotCenter = new rave::Point3D(0,0,0);
   fBeamspotGauss = new rave::Covariance3D(fBeamspotSigmaXY*fBeamspotSigmaXY,
                                           0,0,fBeamspotSigmaXY*fBeamspotSigmaXY,
                                           0,fBeamspotSigmaZ*fBeamspotSigmaZ); //xx,xy,xz,yy,yz,zz
   fBeamspot = new rave::Ellipsoid3D(*fBeamspotCenter,*fBeamspotGauss);
   
   for (int i=0;i<fNCov;i++){
      fStdCovMatrix[i] = new rave::Covariance6D(
          fSigmax*fSigmax,eps*RndmGaus(),eps*RndmGaus(),
          fSigmax*fSigmax,eps*RndmGaus(),fSigmax*fSigmax,
          eps*RndmGaus(),eps*RndmGaus(),eps*RndmGaus(),
          eps*RndmGaus(),eps*RndmGaus(),eps*RndmGaus(),
          eps*RndmGaus(),eps*RndmGaus(),eps*RndmGaus(),
          fSigmap*fSigmap,eps*RndmGaus(),eps*RndmGaus(),
          fSigmap*fSigmap,eps*RndmGaus(),fSigmap*fSigmap
                                                );
   }
   if (fUseBeamspot){
      fFactory =  new rave::VertexFactory(*fBObj, rave::VacuumPropagator(), 
                                          *fBeamspot, fAlgorithm.data(),2); //ToDo fix magic 2
   }
   else {
      fFactory =  new rave::VertexFactory(*fBObj,rave::VacuumPropagator(),
                                          fAlgorithm.data(),2); //ToDo fix magic 2
   }
}

void RaveTest::CreateTrackList()
{
   double pos[3],dir[3];
   const float chi2=12.;
   const float ndof=10;      
   for (int ivtx=0;ivtx<fNVtx; ivtx++){
      int ntrk= fAvNTrk * (0.5 + Rndm());
      for (int i=0; i<ntrk;i++){
         int q = Rndm() > 0.5 ? 1 : -1;
         for (int j=0;j<3;j++){
            pos[j]=fTruthVtx[ivtx][j]+RndmGaus()*fSigmax;
            dir[j]=fMomentumScale*RndmGaus();
         }
         fTrackList.push_back(rave::Track(rave::Vector6D(pos[0],pos[1],pos[2],
                                                         dir[0],dir[1],dir[2]),
                                          *(fStdCovMatrix[i%fNCov]), q, 
                                          chi2, ndof)); 
      }         
   }
}

void RaveTest::Clean()
{
   fTrackList.clear();
   delete fFactory;
   for (int i=0;i<fNCov;i++) delete fStdCovMatrix[i];
   delete fBeamspot;
   delete fBeamspotCenter;
   delete fBObj;
}

bool RaveTest::run()
{
   InitVtx();
   InitRave();
   CreateTrackList();
   std::vector<rave::Vertex> vertices = fFactory->create(fTrackList);
   
   fRate = double(vertices.size())/double(fNVtx);
   bool ok=false;
   if (fNVtx <5){
      ok = (fRate>=0.3) && (fRate<=1.8); // ToDo fix magic numbers
   }
   else{
      ok = (fRate>=0.6) && (fRate<=1.4); // ToDo fix magic numbers
   }
   if (!ok){
      cout << "##########################################"<<endl;
      cout << "Error! vertex rate = "<<fRate<<endl;
      cout << "number of true vertices="<<fNVtx<<endl;
      PrintProperties();
      std::cout << "found "<<vertices.size()<< " vertices"<<std::endl;
      for (int iv = 0 ; iv <  vertices.size(); iv++){
         cout <<"position vertex "<<iv
              <<" x="<<vertices[iv].position().x()
              <<" y="<<vertices[iv].position().y()
              <<" z="<<vertices[iv].position().z()<<endl;         
      }
      cout << "##########################################"<<endl<<endl;
   }
   
   vertices.clear();
   
   Clean();
   return ok;
}


double RaveTest::RndmGaus()
{ 
   return  norm_dist.operator () <boost::lagged_fibonacci19937>((fR));
}
