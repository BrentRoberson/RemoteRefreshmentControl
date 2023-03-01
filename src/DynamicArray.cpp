#include <DynamicArray.h>

 
template <typename T> 
DynamicArray<T>::DynamicArray()
{
    capacity = 5;
    size = 0;
    array= new T[capacity];
}

template <typename T> 
DynamicArray<T>::DynamicArray(int capacity)
{
    this->capacity = capacity;
    this->array= new T[capacity];
    this->size = 0;
}

template <typename T> 
int DynamicArray<T>::getSize() { return size; }

template <typename T> 
int DynamicArray<T>::getCapacity() { return capacity; }

template <typename T> 
void DynamicArray<T>::push_back(T value)
{
    if (size == capacity) {
        growArray();
    }
    array[size] = value;
    size++;
}

template <typename T> 
void DynamicArray<T>::pop_back()
{
    array[size - 1] = T();
    size--;
    if (size == (capacity / 2)) {
        shrinkArray();
    }
}

template <typename T> 
void DynamicArray<T>::growArray()
{
    T* temp = new T[capacity * 2];
    capacity = capacity * 2;
    for (int i = 0; i < size; i++) {
        temp[i] = array[i];
    }

    delete[] array;
    array= temp;
}

template <typename T> 
void DynamicArray<T>::shrinkArray()
{

    capacity = size;
    T* temp = new T[capacity];
    for (int i = 0; i < size; i++) {
        temp[i] = array[i];
    }
    delete[] array;
    array= temp;
}

template <typename T> 
int DynamicArray<T>::search(String id)
{
    for (int i = 0; i < size; i++) {
        if (array[i].ID == id) {
            return i;
        }
    }
    return -1;
}

// edit element at given index
template <typename T> 
void DynamicArray<T>::insertAt(int index, T value)
    {
        if (size == capacity) {
            growArray();
        }
        for (int i = size - 1; i >= index; i--) {
            array[i + 1] = array[i];
        }
        array[index] = value;
        size++;
    }

template <typename T> 
void DynamicArray<T>::deleteAt(int index)
{
    for (int i = index; i < size; i++) {
        array[i] = array[i + 1];
    }
    array[size - 1] = T();
    size--;
    if (size == (capacity / 2)) {
        shrinkArray();
    }
}

//type T must have a print function for this to work
template <typename T> 
void DynamicArray<T>::printArrayDetails()
{ 
    Serial.println();
    Serial.println("Elements of array: ");
    for (int i = 0; i < size; i++) {
        array[i].print();
    }
    Serial.println();
    Serial.print("# of elements in array: "); 
    Serial.print(size); 
    Serial.print(", Capacity of array:");  
    Serial.print(capacity);  
    Serial.println();
}

template <typename T> 
T& DynamicArray<T>:: DynamicArray::operator[](int index)
{
    if (index >= size) {
        Serial.println("Array index out of bound, exiting");
        exit(0);
    }
    return array[index];
}


template <typename T> 
bool DynamicArray<T>::isEmpty()
{
    return(!size);
}

