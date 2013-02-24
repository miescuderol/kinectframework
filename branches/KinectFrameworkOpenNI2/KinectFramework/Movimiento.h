#pragma once
class Movimiento
{
public:

	const static int SIN_DIRECCION = -1;

	typedef int Direccion;

	Movimiento(void);
	~Movimiento(void);

	void setDireccionVertical(Direccion d);
	void setDireccionHorizontal(Direccion d);

	Direccion getDireccionVertical();
	Direccion getDireccionHorizontal();

	bool operator==(Movimiento & otroMovimiento);
	bool operator!=(Movimiento & otroMovimiento);

private:

	Direccion vertical, horizontal;

};

