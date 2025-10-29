#pragma once
constexpr auto INITIAL_CAPACITY = 10;
#include <assert.h>
//forward declaration pentru ca avem nevoie sa implementam
//anumite metode in clasa VectorDinamic care se bazeaza pe iterator
template<typename T>
class IteratorVector;

template<typename T>
class VectorDinamic {
private:
	//reprezentare
	T* elems;
	int capacitate;
	int lungime;

	//resize
	void resize();
public:
	//default constructor
	VectorDinamic();
	//copy constructor
	VectorDinamic(const VectorDinamic& ot);
	//assignment operator
	VectorDinamic& operator=(const VectorDinamic& ot);
	//destructor
	~VectorDinamic();

	//add element in vector
	void add(T elem);
	// Șterge elementul de pe poziția specificată
	void removeAt(int poz);
	// Șterge toate aparițiile unui element dat
	void removeElement(const T& elem);
	//get element from position in vector
	T& get(int poz);
	//set element on position in vector
	void set(T elem, int poz);
	//get size - length of vector
	int size();

	//iterator methods
	friend class IteratorVector<T>;

	//ca sa folosim for each, avem nevoie de aceste metode
	IteratorVector<T> begin();
	IteratorVector<T> end();

};

template<typename T>
class IteratorVector {
private:
	const VectorDinamic<T>& vec;
	int poz = 0;
public:
	IteratorVector(const VectorDinamic<T>& v) noexcept;
	IteratorVector(const VectorDinamic<T>& v, int poz) noexcept;
	bool valid() const;
	T& element() const;
	void next();
	T& operator*();
	IteratorVector& operator++();
	bool operator==(const IteratorVector& ot) noexcept;
	bool operator!=(const IteratorVector& ot) noexcept;
	void operator=(const IteratorVector& ot) noexcept;

};

template<typename T>
VectorDinamic<T>::VectorDinamic() {
	this->elems = new T[INITIAL_CAPACITY];
	this->lungime = 0;
	this->capacitate = INITIAL_CAPACITY;
}

template<typename T>
VectorDinamic<T>::VectorDinamic(const VectorDinamic& ot) {
	//constructor de copiere
	//se creeaza un nou vector dinamic 
	//pe baza unuia existent

	//fiindca se creeaza un nou vector dinamic, trebuie sa alocam spatiu
	this->elems = new T[ot.capacitate];

	//copiem elementele si setam lungime, capacitate
	for (int i = 0; i < ot.lungime; i++) {
		this->elems[i] = ot.elems[i];
	}
	this->lungime = ot.lungime;
	this->capacitate = ot.capacitate;
}


template<typename T>
VectorDinamic<T>& VectorDinamic<T>::operator=(const VectorDinamic& ot) {
	//daca se face assignment v1=v1, nu are sens sa facem
	//operatiile de mai jos, returnam direct
	//breakpoint in test pentru a vedea ce se compara aici

	if (this == &ot) {
		return *this;
	}
	//fiindca in obiectul curent punem altceva prin
	//assignment, eliberam memoria alocata anterior

	delete[] this->elems;

	//copiem elementele din ot si setam lungime, capacitate
	this->elems = new T[ot.capacitate];
	for (int i = 0; i < ot.lungime; i++) {
		elems[i] = ot.elems[i];  //utilizeaza operator assignment aferent tipului T
	}
	this->lungime = ot.lungime;
	this->capacitate = ot.capacitate;
	return *this;
}
template<typename T>
void VectorDinamic<T>::resize() {
	int newCapacity = this->capacitate * 2;
	T* newElems = new T[newCapacity];
	for (int i = 0; i < this->lungime; i++) {
		newElems[i] = this->elems[i];
	}
	delete[] this->elems;
	this->elems = newElems;
	this->capacitate = newCapacity;

}
template<typename T>
VectorDinamic<T>::~VectorDinamic() {
	delete[] this->elems;
}
template<typename T>
void VectorDinamic<T>::add(T elem) {
	if (this->lungime == this->capacitate)
		resize();
	this->elems[this->lungime] = elem;
	this->lungime++;
}

template<typename T>
void VectorDinamic<T>::removeAt(int poz) {
	if (poz < 0 || poz >= lungime) {
		throw std::out_of_range("Indexul este în afara intervalului.");
	}

	for (int i = poz; i < lungime - 1; ++i) {
		elems[i] = elems[i + 1];
	}

	lungime--;
}

template<typename T>
void VectorDinamic<T>::removeElement(const T& elem) {
	for (int i = 0; i < lungime; ++i) {
		if (elems[i] == elem) {
			removeAt(i);
			return; // Ieșim din funcție după ștergere
		}
	}
}

template<typename T>
T& VectorDinamic<T>::get(int poz) {
	//verificam daca pozitia este valida

	return this->elems[poz];
}

template<typename T>
void VectorDinamic<T>::set(T elem, int poz) {
	//verificam daca pozitia este valida

	this->elems[poz] = elem;
}

template<typename T>
int VectorDinamic<T>::size() {
	//verificam daca pozitia este valida

	return this->lungime;
}

template<typename T>
IteratorVector<T> VectorDinamic<T>::begin() {
	return IteratorVector<T>(*this);
}
template<typename T>
IteratorVector<T> VectorDinamic<T>::end() {
	return IteratorVector<T>(*this, this->lungime);
}



template<typename T>
IteratorVector<T>::IteratorVector(const VectorDinamic<T>& v) noexcept :vec{ v } { };
template<typename T>
IteratorVector<T>::IteratorVector(const VectorDinamic<T>& v, int poz) noexcept :vec{ v }, poz{ poz } {};

template<typename T>
bool IteratorVector<T>::valid() const {
	return this->poz < this->vec.lungime;
}
template<typename T>
T& IteratorVector<T>::element() const {
	return this->vec.elems[this->poz];
}
template<typename T>
void IteratorVector<T>::next() {
	this->poz++;
}

template<typename T>
T& IteratorVector<T>::operator*() {
	return this->element();
}

template<typename T>
IteratorVector<T>& IteratorVector<T>::operator++() {
	this->next();
	return *this;
}

template<typename T>
bool IteratorVector<T>::operator==(const IteratorVector& ot) noexcept {
	return this->poz == ot.poz;
}
template<typename T>
bool IteratorVector<T>::operator!=(const IteratorVector& ot) noexcept {
	return !(*this == ot);
}
template<typename T>
void IteratorVector<T>::operator=(const IteratorVector& ot) noexcept {
	this->poz = ot.poz;
}



//
//void test() {
//	VectorDinamic<int> v;
//	v.add(1);
//	v.add(2);
//	v.add(3);
//
//	int i = 1;
//	for (auto x : v) {
//		assert(i == x);
//		i++;
//	}
//	i = 1;
//	IteratorVector<int> it{ v};
//	while (it != v.end()) {
//		assert((*it) == i);
//		i++;
//		++it;
//	}
//	i = 1;
//	IteratorVector<int> it2{ v };
//	while (it2.valid()) {
//		assert(it2.element() == i);
//		i++;
//		it2.next();
//	}
//
//
//}
//void testAssignmentOperator() {
//	VectorDinamic<int> v1;
//	VectorDinamic<int> v2;
//
//	v1.add(1);
//	v1.add(2);
//
//	v2 = v2;
//	v2 = v1;
//	assert(v2.size() == 2);
//}