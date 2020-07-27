#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 
#include <cctype>  // for isalpha()
#include <iomanip>   
#include "strutils.h"
#include "prompt.h"

using namespace std;

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// 22-Feb-2017
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// BURAK KUZ 20647
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@

bool checkMatrixElement(ifstream& input)
{ 
    char ch;
	int checkInt;
	int p = 0;

	while ( input >> checkInt ) //to check whether it fits the 0 - 255 range
	{
		if (checkInt < 0 || checkInt > 255) 
		{
			cout << "Pixel value with " << checkInt << " is out of range [0, 255]" << endl;
			return false;
		}
	}
	input.clear();
	input.seekg(0);
    while ( input.get(ch) ) //checks whether there is invalid characters like alpha. or symbols
	{  
		if ((ch!=' ')&&(ch!='\n')&&(ch!='\t'))
			if (!(isdigit(ch)))  
			{
				cout << "Error: Input file contains invalid characters." << endl;
				return false;
			}
				
	}
	input.clear();
	input.seekg(0);
	while ( input.get(ch) )   //checks whether there is consetutively multiple lines
	{  
		if ((ch =='\n'))
		{
		input.get(ch);
			if ((ch =='\n'))
			{
				cout << "Input file is not in matrix format" << endl;
				return false;
			}		
		}
	}
	input.clear();
	input.seekg(0);
	string checkString;
	vector<int>colsCount;
	while(getline(input,checkString)) //checks whether there exists same number of element in each rows
	{
		int count = 0;
		stringstream checkStream(checkString);
		while(checkStream >> checkInt)
		{
			count++;
		}
		colsCount.push_back(count);
	}
	for(int s = 0; s < colsCount.size(); s++)
	{
		if(colsCount[0] != colsCount[s])
		{
			cout << "Input file is not in matrix format" << endl;
			return false;
		}
	}
	input.clear();
	input.seekg(0);

	return true;
}

void PrintMatrix(const vector<vector<int>> & mat) 
{
    for (int j=0; j < mat.size(); j++)
    {   
		for (int k=0; k < mat[0].size(); k++)
        {   
			cout << setw(4) << mat[j][k]; //creates 4 digit long room for elements of matrix then print one by one
        }
        cout << endl;
    }
}

bool inputFileCheck(ifstream & inputFile) //checks names of input files
{
	if (inputFile.fail()) 
	{
		cout << "File cannot be found, Please enter again." << endl;
		return false;
	}
	else 
	{	
		return true;
	}
}

bool checkFilterSize(string filterSize) //to be sure filter size is numerical postive integer
{
	char* p;
	int filterSizeConverted = strtol(filterSize.c_str(), &p, 10);
	if (*p) 
	{
	return false; // leads here when conversion fails because it wasn't a number
	}
	else //proceed and it is converted to an integer
	{
		if(filterSizeConverted > 0)
		{
			if(filterSizeConverted % 2 != 0)
			return true;
			else
			return false;
		}
		else
		return false;
	}
}

void matrixConstract(vector<vector<int>>& mat, ifstream& input ) //constracts the matrix by adding the elements from the input file
{
	string line;
	while(!input.eof())
	{
		getline(input, line);
		stringstream rowByRow(line); 
		int element;
		vector<int> tempMat;
		while(rowByRow >> element)
		{
			tempMat.push_back(element);		
		}
		mat.push_back(tempMat);
	}
	
}

void SelectSort(vector<int> & a) //taken from CS201 slides

{
    int j, k, temp, minIndex, numElts = a.size();
    for(k=0; k < numElts - 1; k++)
    {   minIndex = k;             
        for(j=k+1; j < numElts; j++)
        {   if (a[j] < a[minIndex])
            {   minIndex = j;  
            }
        }
        temp = a[k];        
        a[k] = a[minIndex];
        a[minIndex] = temp;
    } 
}


int medianValueSubMatrix(vector<vector<int>>& mat, int minRow,int maxRow,int minCol,int maxCol) //finds median value given any subvector
{
	vector<int> temp;
	int i;
	int k, m;
	for (k = minRow ; k < (maxRow+1); k++) 
	{   
		for (m = minCol ; m < (maxCol+1); m++) 
		{   
		i = mat[k][m]; //create linear vector to add elements of subvector
		temp.push_back(i);
		}
	} 
	int medianValue;
	SelectSort(temp); //sort it in non-decreasing order to later find the median
	int sizeTemp = temp.size();
	i = (sizeTemp/2); 
	if(sizeTemp % 2 == 0)
	{
		medianValue = (temp[i-1] + temp[i])/2; //if there is two median elements
	}
	else
	{
		medianValue = temp[i]; 
	}
	return medianValue;
}

int minPos(int dist, int i) //finds minimum posible postion for subvector
{
	int minValue;
	if((i - dist) == 0 || (i - dist) > 0)
	minValue = i - dist;
	else
	{
		while(!((i - dist) == 0 || (i - dist) > 0)) //to avoid position becoming to small to fit inside matrix
		{
			dist--;
		}
		minValue = i - dist;
	}
	return minValue;
}

int maxPos(int dist, int pos, int i) //find maximum posible position for subvector
{
	int maxValue;
	if((i + dist) < pos )
	maxValue = i + dist;
	else
	{
		while(!((i + dist) < pos)) //to avoid position becoming to large to fit inside matrix
		{
			dist--;
		}
		maxValue = i + dist;
	}
	return maxValue;
}

void medianFiltering(vector<vector<int>>& mat, int filterSize, vector<vector<int>>& newMat ) //construct a new matrix with filtered elements
{
	vector<vector<int>> subMat;
	int rows = mat.size();
	int cols = mat[0].size();
	int dist = (filterSize - 1)/2;
	int minRow, maxRow, minCol, maxCol, med;
	for (int j=0; j < (rows); j++)
	{   
		for (int k=0; k < (cols); k++)
		{   
			minRow = minPos(dist, j); //find apporite position for submatrix
			maxRow = maxPos(dist, rows, j);
			minCol = minPos(dist, k);
			maxCol = maxPos(dist, cols, k); 
			med = medianValueSubMatrix(mat, minRow, maxRow, minCol, maxCol); //finds the median value for submatrix
			newMat[j][k] = med; //added over new filtered matrix inputs to a new matrix
		} 
	}
}

int main()
{
	string fileMatrixName,filterSizeS;
	vector<vector<int>> mat;
	ifstream inputMatrix;

	cout << "This program perform median filtering operation on an input\nmatrix given in a text file.\n\n";
	
	int filterSize;
	cout << "Enter the filter size(must be an odd positive integer): ";
	cin >> filterSizeS;
	while(checkFilterSize(filterSizeS) == false)
	{
		cout << "Error. Enter the filter size(must be an odd positive integer): ";
		cin >> filterSizeS;
	}
	filterSize = atoi(filterSizeS);

	cout << "Please enter the name of the input file(with .txt): ";
	cin >> fileMatrixName;
	inputMatrix.open(fileMatrixName.c_str());
	while(inputFileCheck(inputMatrix) != true)
	{
	fileMatrixName = PromptString("Enter the input file name(with .txt): ");
    inputMatrix.open(fileMatrixName.c_str());
	}

	if(checkMatrixElement(inputMatrix) == false)
	{
		cout << "End of execution" << endl;
		system("pause");
		return 0;
	}
	
	matrixConstract(mat, inputMatrix); //constructs matrix by adding the elements from the input
	
	int mainRows = mat.size();
	int mainCols = mat[0].size();

	vector<vector<int>> newMat(mainRows, vector<int>(mainCols, 1));

	medianFiltering(mat, filterSize, newMat); 
	cout << endl << "FILTERED MATRIX" << endl<< endl;

	PrintMatrix(newMat);
	cout << endl;

	return 0;
}

