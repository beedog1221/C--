#include <iostream>
#include <fstream>
using namespace std;
enum {size = 10};

class Matrix //matrix class
{
public:
	void Initialize(int i, int j); //initializes i and j
	void Add(); // add matrices together
	void Subtract(); // subtract matrix values
	void Print(); // print value of matrix
	void Copy(); // copy matrix to another
	void MakeEmpty(); // change value of matrix to 0
	void StoreValue(int, int, int); // store values in matrix
	const Matrix(int, int); // matrix object
	void Fill(); // fill in values in matrix
private:
	int n, row, col; 
	int data[size][size]; // matrix size
};

Matrix::Matrix(int row, int col)
{

	
	row = 0; // put 0 in row
	col = 0; // put 0 in column
}

void Matrix::StoreValue(int i, int j, int value)
{
	
	cout << "enter value" << endl; // ask for value to store
	cin >> value; // user stores value
	data[i][j] = value; // value is added to matrix
};

void Matrix::MakeEmpty() 
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			data[i][j] = 0; // code changes all values to 0
			
		}
	}
}

void Matrix::Add()
{
	int add;
	int i;
	int j;
	add = data[i][j] + data[i][j]; // add matrix parts together
	cout << add << endl;
}

void Matrix::Subtract()
{
	int subtract;
	int i;
	int j;
	subtract = data[i][j] - data[i][j]; // subtract matrix parts together
	cout << subtract << endl;
}

void Matrix::Print()
{
	for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				cout << data[i][j] << endl; // print matrix value
				
			}
		}

}

void Matrix::Copy()
{
	int copy;
	int row;
	int col;
	int rowcopy;
	int colcopy;
	cout << "enter row: ";
	cin >> row;
	cout << "and column: ";
	cin >> col;
	cout << "of matrix you wish to copy" << endl;
	
	cout << "enter row: ";
	cin >> rowcopy;
	cout << "and column: ";
	cin >> colcopy;
	cout << "of matrix you wish to copy to" << endl;
	
	data[rowcopy][colcopy] = data[row][col];
	
}

void Matrix::Fill()
{
	int i, j, z = 0;
	
	        { 
	         for(int i = 0; i < size; i++)
	         {
	            for(int j = 0; j < size; j++)
	            {
	              data[i][j] = z;
	            }
	            z++;      // fills in values to matrix
	         }
	}
	
}


int main()
{
	
	

}