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
		int square;
		Cell(int i, int j) {
			x = i;
			y = j;
			for(int m=1;m<10;m++)
				domain.push_back(m);
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
			if( ((int)(rows[x] & bitVal)!=0) || ((int)(columns[y] & bitVal) !=0) || ((int)(squares[square] & bitVal) !=0)) {
				//cout<<"Found it !! Deleting from Domain"<<endl;
				--it;
				domain.erase(it+1);
				//cout<<"New domain Size is "<<domain.size()<<endl;
			}
		}
	}

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

void removeSingletonDomains() {
    std::make_heap(collections.begin(),collections.end());
	
	vector<Cell>::iterator abc = collections.begin();
	while((*abc).getDomainSize() == 1) {
		matrix[abc->x][abc->y] = abc->getValue();
		rows[abc->x] |= 1<<abc->getValue();
		columns[abc->y] |= 1<<abc->getValue();
		squares[abc->square] |= 1<<abc->getValue();
	
		cout<<"Setting value "<<abc->getValue()<<" at x = "<<abc->x<<"   y = "<<abc->y<<" in square "<<abc->square<<endl;

		for(vector<Cell>::iterator it = collections.begin(); it != collections.end(); ++it) {
			if((it != abc) && (it->x == abc->x || it->y == abc->y || it->square == abc->square)){
				/*I will re-calculate the domain1 for only those cells for which I know the domain1 has changed; efficiency bitch. */
				//cout<<"Recalculating for "<<it->x<<"  "<<it->y<<"  in square "<<it->square<<endl;
				it->updateDomain();
			}
		}
		collections.erase(abc); /*Deleting the Cell that we have freezed because Don't want it to waste my time. laters*/
		cout<<"NOW WE HEAP THIS SHIT\n";
		std::make_heap(collections.begin(),collections.end());	
		abc = collections.begin();
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

	removeSingletonDomains();

	print_matrix();
	for(vector<Cell>::iterator it = collections.begin(); it != collections.end(); ++it) {
		cout<<endl;
		(*it).print();
	}
}
