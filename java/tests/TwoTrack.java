import at.hephy.Rave.*;
import java.lang.Math.*;

public class TwoTrack
{
  public static RaveTrack trackOne()
  {
    RaveVector6D v = new RaveVector6D ( 0.00025, 0.0014, 0.5303, 1.0104, -0.1750, -0.07375 );
    RaveCovariance6D cov = new RaveCovariance6D(7.2e-7, 3.5e-6, 1.5e-6, 2.e-5, -2.6e-7, 2.e-5,
                                               -1.4e-7, -2.5e-7, -1.e-7,-7.7e-7,-1.5e-6, 5.e-8,
                                               -1.1e-7, 2.2e-8,-1.6e-6,5.e-6,-5.3e-7,-3.2e-7,
                                               4.4e-7,3.8e-8,4.e-7 );
    return new RaveTrack(v,cov,-1,0.,0., 1 );
  }

  public static RaveTrack trackTwo()
  {
    RaveVector6D v = new RaveVector6D ( -0.0008, -0.0039, 0.5209, 3.525, -0.788, 1.758 );
    RaveCovariance6D cov = new RaveCovariance6D(5.2e-6, 3.9e-6,-8.7e-6, 2.2e-5, -1.9e-6, -8.7e-6,
                                               -1.4e-7, -2.5e-7, -1.e-7,-7.7e-7,-1.5e-6, 5.e-8,
                                               -1.1e-7, 2.2e-8,-1.6e-6,5.e-6,-5.3e-7,-3.2e-7,
                                               4.4e-7,3.8e-8,4.e-7 );
    return new RaveTrack(v,cov,-1,0.,0., 1 );
  }

  public static RaveTrackContainer createTracks()
  {
   RaveTrackContainer cont = new RaveTrackContainer();
   cont.add ( trackOne() );
   cont.add ( trackTwo() );
   return cont;
  }

  public static void main( String[] args )
  {
    System.out.println( "" );
    System.out.println( "================================================" );
    System.out.println( "                 TwoTrack.java" );
    RaveConstantMagneticField field = new RaveConstantMagneticField(0.,0.,4.);
    
	  System.out.println( "Instantiate Factory.");

    RaveVertexFactory fac = new RaveVertexFactory( field );
    System.out.println("Rave Magnetic field is " + fac.getMagneticField().inTesla( new RavePoint3D() ) );

    RaveTrackContainer cont = createTracks();
    RaveVertexContainer v = fac.create ( cont, "avf" );
    System.out.println( "Produced " + v.size() + " vertices." );
    if ( v.size() > 0 )
    {
     RaveVertex primary=v.get(0);
     System.out.println( "Primary vertex at " + primary.position() );
     
     RavePoint3D right = new RavePoint3D (0.0168, -0.0045, 0.5288);
     
     if ( Math.abs ( primary.position().x() - right.x() ) < 1e-4 &&
          Math.abs ( primary.position().y() - right.y() ) < 1e-4 &&
          Math.abs ( primary.position().z() - right.z() ) < 1e-4 )
     {
       System.out.println ("           Test passed!");
     } else {
       System.out.println ("ERROR: Vertex expected elsewhere!!!" );
     }
    }
    System.out.println( "================================================" );
    System.out.println( "" );
  }
}
