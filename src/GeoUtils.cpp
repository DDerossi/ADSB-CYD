#include "GeoUtils.h"
#include <Arduino.h>
#include <math.h>

static double degToRad(double degrees) {
  return degrees * PI / 180.0;
}

static double radToDeg(double radians) {
  return radians * 180.0 / PI;
}

double calculateDistanceNm(
  double lat1,
  double lon1,
  double lat2,
  double lon2
) {
  const double earthRadiusNm = 3440.065;

  double dLat = degToRad(lat2 - lat1);
  double dLon = degToRad(lon2 - lon1);

  double rLat1 = degToRad(lat1);
  double rLat2 = degToRad(lat2);

  double a =
    sin(dLat / 2.0) * sin(dLat / 2.0) +
    cos(rLat1) * cos(rLat2) *
    sin(dLon / 2.0) * sin(dLon / 2.0);

  double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

  return earthRadiusNm * c;
}

int calculateBearingDeg(
  double lat1,
  double lon1,
  double lat2,
  double lon2
) {
  double rLat1 = degToRad(lat1);
  double rLat2 = degToRad(lat2);
  double dLon = degToRad(lon2 - lon1);

  double y = sin(dLon) * cos(rLat2);
  double x =
    cos(rLat1) * sin(rLat2) -
    sin(rLat1) * cos(rLat2) * cos(dLon);

  double bearing = radToDeg(atan2(y, x));

  int normalized = (int)round(bearing);
  normalized = (normalized + 360) % 360;

  return normalized;
}