#pragma once
class Movimiento
{
public:

	//typedef int Direccion;

	const static int SIN_DIRECCION = -1;

	Movimiento(void);
	~Movimiento(void);

	void setDireccionVertical(int d);
	void setDireccionHorizontal(int d);

	int getDireccionVertical();
	int getDireccionHorizontal();

	bool operator==(Movimiento & otroMovimiento);
	bool operator!=(Movimiento & otroMovimiento);

private:

	int vertical;
	int horizontal;

};

