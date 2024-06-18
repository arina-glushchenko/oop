#pragma once
#include <stdexcept>
#include <string_view>
//� ���� ������ ��� �������� �� ��������� ������ ��������� ���������,
//�� ��� ������ ����������� �� ������� ��������.
typedef char value_type;

class CircularBuffer {
	value_type* buffer;
	int start, end, bufferSize, bufferCapacity;
public:
	CircularBuffer();
	~CircularBuffer();
	CircularBuffer(const CircularBuffer& cb);

	//������������ ����� �������� �������.
	explicit CircularBuffer(int capacity);
	//������������ ����� �������� �������, ������� ��������� ��� ��������� elem.
	CircularBuffer(int capacity, const value_type& elem);
	//������ �� �������. �� ��������� ������������ �������.
	value_type& operator[](int i);
	const value_type& operator[](int i) const;

	//������ �� �������. ������ ������� ���������� � ������ ��������� �������.
	value_type& at(int i);
	const value_type& at(int i) const;

	value_type& front(); //������ �� ������ �������.
	value_type& back();  //������ �� ��������� �������.
	const value_type& front() const;
	const value_type& back() const;

	//������������ - �������� ��������� ����� ���, ��� ��� ������ �������
	//������������ � ������ �������������� ������. ���������� ��������� 
	//�� ������ �������.
	value_type* linearize();
	//���������, �������� �� ����� ���������������.
	bool is_linearized() const;
	//�������� ����� ���, ��� �� �������� ������� �������� ������� 
	//� �������� new_begin.
	void rotate(int new_begin);
	//���������� ���������, ���������� � ������.
	int size() const;
	bool empty() const;
	//true, ���� size() == capacity().
	bool full() const;
	//���������� ��������� ����� � ������.
	int reserve() const;
	//������� ������
	int capacity() const;

	void set_capacity(int new_capacity);
	//�������� ������ ������.
	//� ������ ����������, ����� �������� ����������� ��������� item.
	void resize(int new_size, const value_type& item = value_type());
	//�������� ������������.
	CircularBuffer& operator=(const CircularBuffer& cb);
	//���������� ���������� ������ � ������� cb.
	void swap(CircularBuffer& cb);

	//��������� ������� � ����� ������. 
	//���� ������� ������ ������ ����� ��� �������, �� ��������������
	//������ ������� ������ (�.�., ����� �����������). 
	void push_back(const value_type& item = value_type());
	//��������� ����� ������� ����� ������ ��������� ������. 
	//���������� push_back, ����� ���������� ��������� ������� ������.
	void push_front(const value_type& item = value_type());
	//������� ��������� ������� ������.
	void pop_back();
	//������� ������ ������� ������.
	void pop_front();

	//��������� ������� item �� ������� pos. ������� ������ �������� ����������.
	void insert(int pos, const value_type& item = value_type());
	//������� �������� �� ������ � ��������� [first, last).
	void erase(int first, int last);
	//������� �����.
	void clear();

	//CircularBuffer(CircularBuffer&& other);

	//CircularBuffer& operator=(CircularBuffer&& other);

};

bool operator==(const CircularBuffer& a, const CircularBuffer& b);
bool operator!=(const CircularBuffer& a, const CircularBuffer& b);


