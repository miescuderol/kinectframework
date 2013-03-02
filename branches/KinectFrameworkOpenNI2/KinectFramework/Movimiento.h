#pragma once
#include <ctime>

class Movimiento
{
public:
	
	const static int SIN_DIRECCION = 0;
	const static int ARRIBA = 1;
	const static int ABAJO = -1;
	const static int DERECHA = 1;
	const static int IZQUIERDA = -1;
	const static int ADELANTE = -1;
	const static int ATRAS = 1;
	const static int NO_IMPORTA = 2;

	Movimiento(void);
	~Movimiento(void);

	void setDireccionZ(int d);
	void setDireccionY(int d);
	void setDireccionX(int d);

	void setTiempo(std::time_t t);
	void setX(float x);
	void setY(float y);
	void setZ(float z);

	const int getDireccionZ() const;
	const int getDireccionY() const;
	const int getDireccionX() const;

	const std::time_t getTiempo() const;
	const float getX() const;
	const float getY() const;
	const float getZ() const;

	bool operator==(Movimiento & otroMovimiento);
	bool operator!=(Movimiento & otroMovimiento);

private:

	std::time_t t;
	int dirZ, dirY, dirX;
	float x, y, z;

};

