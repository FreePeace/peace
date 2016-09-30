#pragma once
class GpsTransform
{
private:
	GpsTransform();
	~GpsTransform();

public:
	static void wgs2gcj(double wgsLat, double wgsLng, double *gcjLat, double *gcjLng);
	static void gcj2wgs(double gcjLat, double gcjLng, double *wgsLat, double *wgsLng);
	static void gcj2wgs_exact(double gcjLat, double gcjLng, double *wgsLat, double *wgsLng);
	static double distance(double latA, double lngA, double latB, double lngB);
private:
	inline static int outOfChina(double lat, double lng);
	inline static double __ev_fabs(double x) { return x > 0.0 ? x : -x; }
	inline static void transform(double x, double y, double *lat, double *lng);
	inline static void delta(double lat, double lng, double *dLat, double *dLng);
};



