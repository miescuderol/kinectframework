#pragma once
class Movimiento
{
public:

	typedef enum Direccion {SIN_DIRECCION,a,b,c};

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

