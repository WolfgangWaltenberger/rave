import at.hephy.Rave.*;
import at.hephy.*;

public class Test
{
  static {
    System.loadLibrary("rave");
  }

  public static void main( String[] args )
  {
    System.out.println( "This is Rave "+ rave.RaveVersion() );
  }
}
