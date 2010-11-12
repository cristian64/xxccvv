/// Fichero de cabecera (y definición) de la clase Heap.
/** \file Heap.h
 */

#ifndef _HEAP_H
#define	_HEAP_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "NodoAEstrella.h"

using namespace std;


/// La clase Heap representa un montículo (pila binaria o cola de prioridad).
/** Esta clase está construida con "templates". Es necesario que el tipo de datos con el que se defina el
 * montículo tenga sobrecargados el operador < y el operador ==.
 */
template <class Tipo>
class Heap
{
	public:

		/// Constructor por defecto.
		Heap();

		/// Destructor.
		/** Deja el montículo vacío.
		 */
		~Heap();

		/// Inserta un elemento al montículo.
		/** Dado un elemento, lo introduce en el montículo y lo recalcula.
		 * \param elemento Elemento que se va a introducir.
		 */
		void push(Tipo elemento);

		/// Elimina el elemento máximo del montículo y lo devuelve.
		/** \return Devuelve una referencia al elemento máximo del montículo.
		 */
		Tipo pop();

		/// Obtiene el elemento máximo del montículo pero no elimina el elemento.
		/** \return Devuelve una referencia al elemento máximo del montículo.
		 */
		Tipo top();

		/// Comprueba si el montículo está vacío.
		/** \return Devuelve verdadero si el montículo está vacío.
		 */
		bool empty() const;

		/// Deja el montículo vacío.
		void clear();

		/// Obtiene el número de elementos del montículo.
		/** \return Devuelve un entero con el número de elementos del montículo.
		 */
		int size() const;

		/// Comprueba si un elemento está en el montículo.
		/** Dado un elemento, comprueba si está en el montículo.
		 * \param elemento Elemento que se va a comprobar.
		 * \return Devuelve verdadero si el elemento está en el montículo.
		 */
		bool contains(Tipo elemento) const;

		/// Elimina un elemento si está en el montículo.
		/** Dado un elemento, lo elimina del montículo.
		 * \param elemento Elemento que se va a comprobar.
		 * \return Devuelve verdadero si el elemento se ha eliminado.
		 */
		bool remove(Tipo elemento);

		/// Ordena el montículo.
		/** Se ordenan de menor a mayor los elementos del montículo.
		 * \return Devuelve un vector con los elementos del montículo ordenados de menor a mayor.
		 */
		vector<Tipo> sort() const;

	private:

		/// Vector interno sobre el que se construye el montículo.
		vector<Tipo> monticulo;
};


template <class Tipo>
Heap<Tipo>::Heap() {}

template <class Tipo>
Heap<Tipo>::~Heap()
{
	monticulo.clear();
}

template <class Tipo>
void Heap<Tipo>::push(Tipo elemento)
{
	monticulo.push_back(elemento);
	push_heap(monticulo.begin(), monticulo.end());
}

template <class Tipo>
Tipo Heap<Tipo>::pop()
{
	Tipo cima = monticulo[0];
	pop_heap(monticulo.begin(), monticulo.end());
	monticulo.pop_back();
	return cima;
}

template <class Tipo>
Tipo Heap<Tipo>::top()
{
	return monticulo[0];
}

template <class Tipo>
bool Heap<Tipo>::empty() const
{
	return monticulo.empty();
}

template <class Tipo>
void Heap<Tipo>::clear()
{
	monticulo.clear();
}

template <class Tipo>
int Heap<Tipo>::size() const
{
	return monticulo.size();
}

template <class Tipo>
bool Heap<Tipo>::contains(Tipo elemento) const
{
	//TODO
	return false;
}

template <class Tipo>
bool Heap<Tipo>::remove(Tipo elemento)
{
	//TODO
	return false;
}

template <class Tipo>
vector<Tipo> Heap<Tipo>::sort() const
{
	vector<Tipo> copia = monticulo;
	sort_heap(copia.begin(), copia.end());
	return copia;
}


#endif	/* _HEAP_H *//// Fichero de cabecera (y definición) de la clase Heap.
