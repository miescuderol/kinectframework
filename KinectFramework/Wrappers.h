#pragma once

class Punto3f
{
private:
	float x, y, z;
public:
	Punto3f()
	{
		x = y = z = 0.0f;
	}
	Punto3f(float x, float y, float z)
	{
		this->set(x, y, z);
	}
	Punto3f(const Punto3f& other)
	{
		*this = other;
	}

	void set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Punto3f& operator=(const Punto3f& other)
	{
		set(other.x, other.y, other.z);

		return *this;
	}
	bool operator==(const Punto3f& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
	bool operator!=(const Punto3f& other) const
	{
		return !operator==(other);
	}

	float X() const { return x; }

	float Y() const { return y; }

	float Z() const { return z; }
};

class Articulacion {
public:
	virtual float getConfianza() const = 0;
	virtual const Punto3f * getPosicion() const = 0;
};

class Esqueleto {
public:
	virtual const Articulacion * getArticulacion(int articulacion) const = 0;
};

typedef int JugadorID;
