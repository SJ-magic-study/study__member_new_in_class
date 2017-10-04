/************************************************************
Fix
	デストラクタ、コピーコンストラクタ、代入演算子
	を適切に実装した。
	
	さらに、「vectorがコピーコンストラクタを暗黙的に呼ぶ」ことを確認するため、いくつかのprintfを挿入。
	下記codeのoutputは、
		
		> Enter:main
		> Enter:add
		def contr : ID 0				// add() local
		copy contr : ID 0 -> ID 1
		> Exit:add
		destr : ID 0
		> Enter:add
		def contr : ID 2
		copy contr : ID 2 -> ID 3		// 
		copy contr : ID 1 -> ID 4		// 元のvectorからcopyし
		destr : ID 1					// 元のvector要素は解放.
		> Exit:add
		destr : ID 2
		> Exit:main
		destr : ID 3
		destr : ID 4
		
	vectorにpush_back()した際の動きが、
		あたらに配列を確保し、元の要素からcopyして、元の要素は全て解放する
	と言う点に注意.
************************************************************/
#include <stdio.h>
#include <vector>
using namespace std;


/************************************************************
************************************************************/
/*
class A {
public:
	A() { mX = new int; }
	~A() { delete mX; }
	int* mX;
};
*/

class A {
public:
	A(){
		ID = NewedCounter;
		
		printf("def contr : ID %d\n", ID);
		mX = new int();
		
		NewedCounter++;
	}
	explicit A(const A &a){
		ID = NewedCounter;
		
		printf("copy contr : ID %d -> ID %d\n", a.ID, ID);
		mX = new int(*a.mX);
		
		NewedCounter++;
	}

	virtual ~A(){
		printf("destr : ID %d\n", ID);
		delete mX;
	}
	
	// 代入演算子のオーバーロード
	A &operator=(const A &a){
		printf("ope= : ID %d -> ID %d\n", a.ID, ID);
		
		if(this != &a) {
			*mX = *a.mX;
		}
		return *this;
	}
	
	int *mX;
	
	int ID;						// A objectを作成する度に順に振られるID.
	static int NewedCounter;	// IDを振るため、class共通でcounterを持つ.
};
int A::NewedCounter = 0;


vector< A > gA;

void add() {
	printf("> Enter:%s\n", __FUNCTION__);
	
	A a;
	gA.push_back( a );
	
	printf("> Exit:%s\n", __FUNCTION__);
}


/************************************************************
************************************************************/

/******************************
******************************/
int main() {
	printf("> Enter:%s\n", __FUNCTION__);
	
	int NumAdds = 2;
	for(int i = 0; i < NumAdds; i++){
		add();
		*( gA[ 0 ].mX ) = i;
	}
	
	printf("> Exit:%s\n", __FUNCTION__);
	return 0;
}

