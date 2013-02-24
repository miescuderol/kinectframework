#include "Wrappers.h"
#include "NiTE.h"

class NiTEArticulacion : public Articulacion {

private:

	nite::SkeletonJoint articulacion;

public:

	NiTEArticulacion(nite::SkeletonJoint articulacion) {
		this->articulacion = articulacion;
	}

	float getConfianza() const
	{
		return articulacion.getPositionConfidence();
	}

	const Punto3f * getPosicion() const
	{
		return new Punto3f(articulacion.getPosition().x,articulacion.getPosition().y,articulacion.getPosition().z);
	}

};

class NiTEEsqueleto : public Esqueleto {

private:

	nite::Skeleton esqueleto;

public:

	typedef enum {
		CABEZA,
		CUELLO,
		HOMBRO_IZQ,
		HOMBRO_DER,
		CODO_IZQ,
		CODO_DER,
		MANO_IZQ,
		MANO_DER,
		TORSO,
		CADERA_IZQ,
		CADERA_DER,
		RODILLA_IZQ,
		RODILLA_DER,
		PIE_IZQ,
		PIE_DER
	} TipoArticulacion ;

	NiTEEsqueleto(nite::Skeleton esqueleto) {
		this->esqueleto = esqueleto;
	}

	const Articulacion * getArticulacion( int articulacion ) const
	{
		return new NiTEArticulacion(esqueleto.getJoint((nite::JointType)articulacion));
	}

};