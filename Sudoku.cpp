#include<iostream>
#include<stdio.h>
#include<vector>
#include<algorithm>

using namespace std;
int matrix[10][10];
unsigned int squares[10],rows[10],columns[10];

int getSquareNumber(int x, int y) {
	if((x >= 1 && x <4) && (y >= 1 && y <4))
		return 1;
	else if((x >= 1 && x <4) && (y >=4 && y<7))
		return 2;
	else  if((x >= 1 && x <4) && (y >=7 && y<10))
		return 3;
	else if((x >= 4 && x <7) && (y >= 1 && y <4))
		return 4;
	else if((x >= 4 && x <7) && (y >=4 && y<7))
		return 5;
	else  if((x >= 4 && x <7) && (y >=7 && y<10))
		return 6;
	else if((x >= 7 && x <10) && (y >= 1 && y <4))
		return 7;
	else if((x >= 7 && x <10) && (y >=4 && y<7))
		return 8;
	else
		return 9;
}

class Cell {
	public:
		int x, y;
		vector<int> domain;
		int domain_size;
		int square;
		Cell(int i, int j) {
			x = i;
			y = j;
			for(int m=1;m<10;m++)
				domain.push_back(m);
			domain_size = 9;
			square = getSquareNumber(x,y);
		}
	
	int getValue() {
		return domain.at(0);
	}

	int getDomainSize() {
		return domain.size();
	}

	void updateDomain() {
		
		for(vector<int>::iterator it = domain.begin();it != domain.end();++it) {
			int val = (*it);
			int bitVal = 1<<val;
			//cout<<"Trying to check for "<<val<<" ...."<<(rows[x] & bitVal)<<"  "<<(columns[y] & bitVal != 0)<<"  "<<(squares[square] & bitVal != 0)<<endl;
			if(((rows[x] & bitVal) != 0) || ((columns[y] & bitVal) != 0) || ((squares[square] & bitVal) != 0)) {
				cout<<"Found it !! Deleting from Domain"<<endl;
				domain.erase(it);
				cout<<"New domain Size is "<<domain.size()<<endl;
			}
		}
	}

	/*void reCalculateDomain1() {
		int i,j;
		for(i=1;i<10;i++) {
			if(matrix[x][i] != 0 && domain1[matrix[x][i]] != 0) {
				domain1[matrix[x][i]] = 0;
				domain_size--;
			}
		
			if(matrix[i][y] != 0 && domain1[matrix[i][y]] != 0) {
				domain1[matrix[i][y]] = 0;
				domain_size--;
			}
		}
		
		switch(square) {
			case 1:
				i = 1;j=1;
				break;
			case 2:
				i = 1;j=4;
				break;
			case 3:
				i = 1;j=7;
				break;
			case 4:
				i = 4;j=1;
				break;
			case 5:
				i = 4;j=4;
				break;
			case 6:
				i = 4;j=7;
				break;
			case 7:
				i = 7;j=1;
				break;
			case 8:
				i = 7;j=4;
				break;
			case 9:
				i = 7;j=7;
				break;
			
		}

		for(int c = i;c<i+3;c++)
			for(int b=j;b<j+3;b++) {
				if(matrix[c][b] !=0) {
					if(domain1[matrix[c][b]] != 0) {
						domain1[matrix[c][b]] = 0;
						domain_size--;
					}
				}
			}
	}*/

	void print() {
		cout<<x<<"   "<<y<<"   "<<getDomainSize();
	}
	
	bool operator<(Cell &rhs) {
	 	if(getDomainSize() == rhs.getDomainSize()) {
	 		if(x == rhs.x)
	 			return y>rhs.y;
	 		else
	 			return x>rhs.x;
		 }
		 return getDomainSize()>rhs.getDomainSize();
	}
	
	bool operator==(const Cell &check) {
		return (check.x == x && check.y == y);
	}
};

vector<Cell> collections;
void print_matrix() {
	cout<<"The Current Matrix is as follows\n";

	for(int i=1;i<10;i++) {
		for(int j=1;j<10;j++) {
			cout<<" "<<matrix[i][j];
		}
		cout<<endl;
	}
	
}

int main() {
	cout<<"Hello World | We are awesome | Welcome to Suduko\n";
	cout<<"Please enter the values for the Sudoku\n";
	
	for(int i=1;i<10;i++)
		for(int j=1;j<10;j++) {
			cin>>matrix[i][j];
			if(matrix[i][j]==0) {
				Cell* cell = new Cell(i,j);
				collections.push_back(*cell);
			} else {
				//cout<<"the value to be appended is = "<<(1<<matrix[i][j])<<endl;
				rows[i] |= 1<<matrix[i][j];
				columns[j] |= 1<<matrix[i][j];
				squares[getSquareNumber(i,j)] |= 1<<matrix[i][j];
				//cout<<"Final answer is "<<rows[i]<<"   "<<columns[j]<<"  "<<squares[getSquareNumber(i,j)]<<endl;
			}
		}

	for(vector<Cell>::iterator it = collections.begin(); it != collections.end(); ++it)
		(*it).updateDomain();

	std::make_heap(collections.begin(),collections.end());
	
	vector<Cell>::iterator abc = collections.begin();
	while((*abc).getDomainSize() == 1) {
		matrix[(*abc).x][(*abc).y] = (*abc).getValue();
		cout<<"Setting value "<<(*abc).getValue()<<" at x = "<<(*abc).x<<"   y = "<<(*abc).y<<" in square "<<abc->square<<endl;

		for(vector<Cell>::iterator it = collections.begin(); it != collections.end(); ++it) {
			if((it != abc) && (it->x == abc->x || it->y == abc->y || it->square == abc->square)){
				/*I will re-calculate the domain1 for only those cells for which I know the domain1 has changed; efficiency bitch. */
				//cout<<"Recalculating for "<<it->x<<"  "<<it->y<<"  in square "<<it->square<<endl;
				(*it).updateDomain();
			}
		}
		collections.erase(abc); /*Deleting the Cell that we have freezed because Don't want it to waste my time. laters*/
		cout<<"NOW WE HEAP THIS SHIT\n";
		std::make_heap(collections.begin(),collections.end());	
		abc = collections.begin();
	}

	print_matrix();
	for(vector<Cell>::iterator it = collections.begin(); it != collections.end(); ++it) {
		cout<<endl;
		(*it).print();
	}
}
