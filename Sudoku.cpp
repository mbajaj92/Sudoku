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
		vector<int> tempDomain;
		int square;
		Cell(int i, int j) {
			x = i;
			y = j;
			for(int m=1;m<10;m++) {
				domain.push_back(m);
				tempDomain.push_back(m);
			}
			square = getSquareNumber(x,y);
		}
	
	int getValue() {
		return domain.at(0);
	}

	int getDomainSize() {
		return domain.size();
	}

	int getTempDomainSize() {
		return tempDomain.size();
	}

	vector<int> getDomain() {
		return domain;
	}

	vector<int> getTempDomain() {
		return tempDomain;
	}

	void updateDomain() {
		for(vector<int>::iterator it = domain.begin(),itr = tempDomain.begin();it != domain.end();) {
			int val = (*it);
			int bitVal = 1<<val;
			if(((int)(rows[x] & bitVal)!=0) || ((int)(columns[y] & bitVal) !=0) || ((int)(squares[square] & bitVal) !=0)) {
				/*std::swap(*it,domain.back());
				domain.pop_back();*/
				domain.erase(it);
				/*std::swap(*itr,tempDomain.back());
				tempDomain.pop_back();*/
				tempDomain.erase(itr);
			} else {
				++it;
				++itr;
			}
		}
	}

	void temporaryDomainUpdate() {
		//int old = tempDomain.size();
		tempDomain.clear();
		for(vector<int>::iterator it = domain.begin();it != domain.end();++it) {
			int val = (*it);
			int bitVal = 1<<val;
			if(!(((int)(rows[x] & bitVal)!=0) || ((int)(columns[y] & bitVal) !=0) || ((int)(squares[square] & bitVal) !=0))) {
				tempDomain.push_back(val);
			}
		}
		//cout<<" DIFF = "<<(n-old)<<endl;
	}

	void print() {
		cout<<"X: "<<x<<"  Y: "<<y<<"  SQ: "<<square<<"  DMSIZE: "<<getDomainSize();
	}
	
	bool operator<(Cell &rhs) {
	 	if(getDomainSize() == rhs.getDomainSize()) {
	 		if(x == rhs.x)
	 			return y<rhs.y;
	 		else
	 			return x<rhs.x;
		 }
		 return getDomainSize()>rhs.getDomainSize();
	}
	
	bool operator==(const Cell &check) {
		return (check.x == x && check.y == y);
	}
	
	bool equals(Cell xyz) {
		return (xyz.x == x && xyz.y ==y && xyz.square == square);
	}
};

struct tempDomainComparator{
  bool operator() (Cell &lhs, Cell &rhs) {
    	if(lhs.getTempDomainSize() == rhs.getTempDomainSize()) {
	 		if(lhs.x == rhs.x)
	 			return lhs.y<rhs.y;
	 		else
	 			return lhs.x<rhs.x;
		}
		return lhs.getDomainSize()>rhs.getDomainSize();
  }
};

vector<Cell> collections;
vector<Cell*> reverseSquares[10];
vector<Cell*> reverseRows[10], reverseColumns[10];

void print_matrix() {
	cout<<"The Current Matrix is as follows\n";

	for(int i=1;i<10;i++) {
		for(int j=1;j<10;j++) {
			cout<<" "<<matrix[i][j];
		}
		cout<<endl;
	}
	
}

void updateRelatedDomainsNew(vector<Cell>::iterator abc) {
	vector<Cell*>::iterator it = reverseRows[abc->x].begin();
	for(;it != reverseRows[abc->x].end();) {
		cout<<"Checking for ";
		(*it)->print();
		if((*it)->equals(*abc)) {
			std::swap(*it,reverseRows[abc->x].back());
			reverseRows[abc->x].pop_back();
		} else {
			(*it)->updateDomain();
			++it;
		}
	}
	
	it = reverseColumns[abc->y].begin();
	for(;it != reverseColumns[abc->y].end();) {
		cout<<"Checking for ";
		(*it)->print();
		if((*it)->equals(*abc)) {
			std::swap(*it,reverseColumns[abc->y].back());
			reverseColumns[abc->y].pop_back();
		} else {
			(*it)->updateDomain();
			++it;
		}
	}

	it = reverseSquares[abc->square].begin();
	for(;it != reverseSquares[abc->square].end();) {
		cout<<"Checking for ";
		(*it)->print();
		if((*it)->equals(*abc)) {
			std::swap(*it,reverseSquares[abc->square].back());
			reverseSquares[abc->square].pop_back();
		} else {
			(*it)->updateDomain();
			++it;
		}
	}
}

void updateRelatedDomains(Cell abc, bool temporary) {
	for(vector<Cell>::iterator it = collections.begin(); it != collections.end(); ++it) {
		if(it->x == abc.x || it->y == abc.y || it->square == abc.square) {
			/*I will re-calculate the domain1 for only those cells 
			 *for which I know the domain1 has changed; efficiency bitch.*/
			if(temporary)
				it->temporaryDomainUpdate();
			else
				it->updateDomain();
		}
	}	
}

void removeSingletonDomains() {
    std::make_heap(collections.begin(),collections.end());

	vector<Cell>::iterator abc = collections.begin();
	while((*abc).getDomainSize() == 1) {
		//freezeCell(*abc);
		matrix[abc->x][abc->y] = abc->getValue();
		rows[abc->x] |= 1<<abc->getValue();
		columns[abc->y] |= 1<<abc->getValue();
		squares[abc->square] |= 1<<abc->getValue();
		/*reverseRows[abc->x].erase(abc);
		reverseColumns[abc->y].erase(abc);
		reverseSquares[abc->square].erase(*abc);*/

		cout<<"Setting value "<<abc->getValue()<<" at x = "<<abc->x<<"   y = "<<abc->y<<" in square "<<abc->square<<endl;
		/*std::swap(*abc,collections.back());
		collections.pop_back();*/
		collections.erase(abc);

		updateRelatedDomains(*abc,false);
		//updateRelatedDomainsNew(abc);
		
		/*Deleting the Cell that we have freezed because 
		 *don't want it to waste my time. laters*/
		cout<<"NOW WE HEAP THIS SHIT\n";
		std::make_heap(collections.begin(),collections.end());
		abc = collections.begin();
	}
}

bool canWeFreeze(vector<Cell>::iterator it, int value) {
	if(++it == collections.end())
		return true;

	//std::make_heap(collections.begin(),collections.end(),tempDomainComparator());
	//it = collections.begin();

	//if((*it).getTempDomainSize() == 0)
	//	return false;

	/*std::swap((*it),collections.back());
	collections.pop_back();*/
	//collections.erase(it);

	//vector<int> domain = (*it).getTempDomain();
	vector<int> domain = (*it).getDomain();
	for(vector<int>::iterator interator = domain.begin(); interator != domain.end(); ++interator) {
		int val = *interator;
		int bitVal = 1<<val;
		if(((int)(rows[it->x] & bitVal) ==0) && ((int)(columns[it->y] & bitVal) ==0) && ((int)(squares[it->square] & bitVal) ==0)) {
			matrix[it->x][it->y] = val;
			rows[it->x] |= 1<<(*interator);
			columns[it->y] |= 1<<(*interator);
			squares[it->square] |= 1<<(*interator);
			//cout<<"Calling for "<<it->x<<"  "<<it->y<<"   "<<it->square<<endl;
			//updateRelatedDomains(*it,true);

			if(canWeFreeze(it,(*interator)))
				return true;

			/*No we can't, so revert all the changes*/
			rows[it->x] &= ~(1u<<(*interator));
			columns[it->y] &= ~(1u<<(*interator));
			squares[it->square] &= ~(1u<<(*interator));
			matrix[it->x][it->y] = 0;
			//cout<<"Re-Calling for "<<it->x<<"  "<<it->y<<"   "<<it->square<<endl;
			//updateRelatedDomains(*it,true);
		}
	}
	//collections.push_back(*it);
	return false;
}

bool searchNeedleInHayStack() {

	std::make_heap(collections.begin(),collections.end());
	vector<Cell>::iterator it = collections.begin();
	/*std::swap((*it),collections.back());
	collections.pop_back();*/
	//collections.erase(it);

	vector<int> domain = (*it).getDomain();
	for(vector<int>::iterator interator = domain.begin(); interator != domain.end(); ++interator) {

		matrix[it->x][it->y] = (*interator);
		rows[it->x] |= 1<<(*interator);
		columns[it->y] |= 1<<(*interator);
		squares[it->square] |= 1<<(*interator);
		//cout<<"Calling for "<<it->x<<"  "<<it->y<<"   "<<it->square<<endl;
		//updateRelatedDomains(*it,true);

		if(canWeFreeze(it,(*interator)))
			return true;

		/*No we can't, so revert all the changes*/
		rows[it->x] &= ~(1u<<(*interator));
		columns[it->y] &= ~(1u<<(*interator));
		squares[it->square] &= ~(1u<<(*interator));
		matrix[it->x][it->y] = 0;
		//cout<<"Re-Calling for "<<it->x<<"  "<<it->y<<"   "<<it->square<<endl;
		//updateRelatedDomains(*it,true);
	}
	return false;
}

int main() {
	cout<<"Hello World | We are awesome | Welcome to Suduko\n";
	cout<<"Please enter the values for the Sudoku\n";
	
	for(int i=1;i<10;i++)
		for(int j=1;j<10;j++) {
			cin>>matrix[i][j];
			if(matrix[i][j]==0) {
				Cell *cell = new Cell(i,j);
				collections.push_back(*cell);
				reverseColumns[j].push_back(cell);
				reverseRows[i].push_back(cell);
				reverseSquares[cell->square].push_back(cell);
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
	cout<<"CAN WE "<<searchNeedleInHayStack();

	print_matrix();
	for(vector<Cell>::iterator it = collections.begin(); it != collections.end(); ++it) {
		cout<<endl;
		(*it).print();
	}
}
