// copied from CLHEP's SystemOfUnits

#ifndef RaveUnits_H
#define RaveUnits_H

namespace raveunits {
  static const float centimeter  = 1.;
  static const float millimeter  = 1.e-1*centimeter;
  static const float meter  = 100.*centimeter;
  static const float micrometer = 1.e-6 *meter;
  static const float nanometer  = 1.e-9 *meter;
  static const float nm  = nanometer;
  static const float um  = micrometer;
  static const float mm  = millimeter;
  static const float cm  = centimeter;
  static const float m  = meter;

  static const float second      = 1.;
  static const float nanosecond  = 1.e-9 * second;
  static const float millisecond = 1.e-3 * second;
  static const float microsecond = 1.e-6 * second;
  static const float ns = nanosecond;
  static const float  s = second;
  static const float ms = millisecond;

  static const float gigaelectronvolt = 1;
  static const float megaelectronvolt = 1.e-3*gigaelectronvolt;
  static const float kiloelectronvolt = 1.e-6*gigaelectronvolt;
  static const float     electronvolt = 1.e-9*gigaelectronvolt;
  static const float teraelectronvolt = 1.e+3*megaelectronvolt;

  static const float MeV = megaelectronvolt;
  static const float  eV = electronvolt;
  static const float keV = kiloelectronvolt;
  static const float GeV = gigaelectronvolt;
  static const float TeV = teraelectronvolt;

  static const float rad = 1.;
  static const float millirad = 1.e-3;
  static const float mrad = millirad;

  static const float speedoflight = 1.;
  static const float c = speedoflight;

  static const float Tesla = 1.;
  static const float Gauss = 0.0001;
  static const float kGauss = 0.1;
}

#endif /* RaveUnits_H */
