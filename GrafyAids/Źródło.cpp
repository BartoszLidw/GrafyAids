#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
//komendy opisujêce graf
class node
{
public:
	int val = 0;
	int elem = 0;
	node* next = nullptr;
};
class node_kraw
{
public:
	int from = 0;
	int val = 0;
	int elem = 0;
	node_kraw* next = nullptr;

};
void Add_node(node*& H, int elem, int x)
{
	node* p = new node;
	p->val = x;
	p->elem = elem;
	p->next = H;
	H = p;
}
void Add_node_kraw(node_kraw*& H, int from, int elem, int val)
{
	node_kraw* p = new node_kraw;
	p->from = from;
	p->elem = elem;
	p->val = val;
	p->next = H;
	H = p;
}
node_kraw* last(node_kraw* H) {
	node_kraw* p = H;
	while (p->next != NULL) {
		p = p->next;
	}
	return p;
}
void connect2node(node_kraw*& H, node_kraw*& P)
{
	last(H)->next = P;
}
//komendy wyœwietlaj¹ce poszczególne grafy
void Show_clearMatrix(int** M, int rozmiar) {
	for (int i = 0; i < rozmiar; i++)
	{
		for (int j = 0; j < rozmiar; j++)
		{
			cout<< M[i][j] << " ";
		}
		cout<< endl;
	}
}
void Show_NeighbourList(node** LN, int rozmiar) {
	for (int i = 0; i < rozmiar; i++)
	{
		cout << i << "->";
		if (LN[i]) {
			node* p = LN[i];
			while (p) {
				cout << p->elem << "," << p->val << "->";
				p = p->next;
			}
			cout << "NULL" << endl;
		};
	}
}
void Show_Edgelist(node_kraw* LE) {
	node_kraw* p = LE;
	while (p) {
		cout << p->from << "," << p->elem << "," << p->val << "->";
		p = p->next;
	}
	cout << "NULL" << endl;
}
//komendy zapisuj¹ce do poszczególnych postaci
int** ToMatrix(string nazwa, int& rozmiar)
{
	fstream czytaj;
	czytaj.open(nazwa, ios::in);
	czytaj >> rozmiar;
	int** M = new int* [rozmiar];
	for (int i = 0; i < rozmiar; i++)
	{
		M[i] = new int[rozmiar];
	}
	for (int i = 0; i < rozmiar; i++)
	{
		for (int j = 0; j < rozmiar; j++)
		{
			czytaj >> M[i][j];
		}
	}
	czytaj.close();
	return M;
}
node** ToNeighbourList(string nazwa, int& rozmiar)
{
	fstream czytaj;
	czytaj.open(nazwa, ios::in);
	czytaj >> rozmiar;
	node** LN = new node * [rozmiar];
	for (int i = 0; i < rozmiar; i++)
	{
		LN[i] = NULL;
	}
	for (int i = 0; i < rozmiar; i++)
	{

		for (int j = 0; j < rozmiar; j++)
		{
			int temp;
			czytaj >> temp;
			if (temp > 0) {
				Add_node(LN[i], j, temp);
			}
		}
	}
	czytaj.close();
	return LN;
}
node_kraw* ToEdge(string nazwa, int& rozmiar) {
	fstream czytaj;
	czytaj.open(nazwa, ios::in);
	czytaj >> rozmiar;
	node_kraw* LE = NULL;
	for (int i = 0; i < rozmiar; i++)
	{
		for (int j = 0;j < rozmiar; j++)
		{
			int temp;
			if (j <= i)
			{
				czytaj >> temp;
			}
			else {
				czytaj >> temp;
				if (temp > 0) {
					Add_node_kraw(LE, i, j, temp);
				}
			}
		}
	}
	czytaj.close();
	return LE;
}
//inne komendy
void spacer()
{
	cout << endl << "__________________________" << endl << endl;
}
void delTop(node_kraw*& H) {
	if (H != NULL) {
		node_kraw* p = H;
		H = p->next;
		delete p;
	}
}
bool allvisited(bool visited[], int rozmiar)
{
	for (int i = 0; i < rozmiar; i++)
	{
		
		if (visited[i] == true)
		{		
			return true;
		}
	}
	return false;
}
void Swap(node_kraw*& H) {
	if (H && H->next) {
		node_kraw* p1 = H;
		H = H->next;
		p1->next = H->next;
		H->next = p1;
	}
}
void Swap(node_kraw*& H, node_kraw* p1) {
	if (H && H->next && p1 == NULL) {
		Swap(H);
		return;
	}
	if (p1 && p1->next && p1->next->next) {
		node_kraw* p2 = p1->next;
		p1->next = p2->next;
		p2->next = p1->next->next;
		p1->next->next = p2;

	}
}
void sort(node_kraw*& H)
{
	if (H && H->next) {
		node_kraw* p = H;
		node_kraw* end = NULL;
		node_kraw* end2 = NULL;
		bool is = true;
		while (is) {
			is = false;
			if (H->val > H->next->val) {
				Swap(H);
				is = true;
				end = H;
			}
			p = H;
			while (p->next && p->next->next && p->next->next != end2) {
				if (p->next->val > p->next->next->val) {
					end = p->next;
					is = true;
					Swap(H, p);
				}
				p = p->next;
			}
			end2 = end;
		}
	}
}
node_kraw* Kruskal(string nazwa, int& rozmiar)
{
	
	node_kraw* LE = ToEdge("grafy.txt", rozmiar);
	sort(LE);
	node_kraw* LER = nullptr;
	node_kraw* p1;
	int* forests = new int[rozmiar];
	bool* visited = new bool[rozmiar];
	int forest = 0;
	for (int i = 0; i < rozmiar; i++)
	{
		forests[i] = 0;
		visited[i] = true;
	}
	while(LE)
	{
	if(visited[LE->elem] == true && visited[LE->from] == true)
	{
		visited[LE->elem] = false;
		visited[LE->from] = false;
		forest = forest + 1;
		forests[LE->elem] = forest;
		forests[LE->from] = forest;
		
		p1 = LE;
		LE = LE->next;
		p1->next = LER;
		LER = p1;
	}
	else if ((visited[LE->elem] == true && visited[LE->from] == false) || (visited[LE->elem] == false && visited[LE->from] == true))
	{
		visited[LE->elem] = false;
		visited[LE->from] = false;

		if (forests[LE->from] == 0)
			forests[LE->from] = forests[LE->elem];
		else
			forests[LE->elem] = forests[LE->from];
		p1 = LE;
		LE = LE->next;
		p1->next = LER;
		LER = p1;
	}
	else if(visited[LE->elem] == false && visited[LE->from] == false)
	{
		if (forests[LE->elem] != forests[LE->from])
		{
			
			forest = forest + 1;
			int las1 = forests[LE->elem];
			int las2 = forests[LE->from];
			for (int i = 0; i < rozmiar; i++)
			{
				if (forests[i] == las1 || forests[i] == las2)
				{
					forests[i] = forest;
				}
			}
			p1 = LE;
			LE = LE->next;
			p1->next = LER;
			LER = p1;
		}
		else {
			p1 = LE;
			LE = p1->next;
			delete p1;
		}
		
	}

	for (int i = 0; i < rozmiar; i++)
	{
		cout << forests[i] << " ";
	}
	cout << endl;
	}

	return LER;

}
node_kraw* Prima(string nazwa, int& rozmiar, int start)
{
	node** LE = ToNeighbourList("grafy.txt", rozmiar);
	node_kraw* LER = nullptr;
	bool* visited = new bool[rozmiar];
	for (int i = 0; i < rozmiar; i++)
	{
		visited[i] = true;
	}

	visited[start] = false;

	do
	{
		int min = -1;
		int kraw_start;
		int kraw_end;
		for (int i = 0; i < rozmiar; i++)
		{
			if (visited[i] == false)
			{
				node* p = LE[i];
				while (p) {
					
					
						if (min == -1 && visited[p->elem] == true)
						{
							min = p->val;
							kraw_end = p->elem;
							kraw_start = i;
							p = p->next;
						}
						else if (min == -1 && visited[p->elem] != true)
						{
							p = p->next;
						}
						else if (min > p->val && visited[p->elem] == true)
						{
							min = p->val;
							kraw_end = p->elem;
							kraw_start = i;
						}
						else
						{
							p = p->next;
						}
					
				}
				
			}
		}
		visited[kraw_end] = false;
		Add_node_kraw(LER, kraw_start, kraw_end, min);

	} while (allvisited(visited, rozmiar));
	
	
		return LER;
}
node** Dijkstry(string nazwa, int& rozmiar, int start)
{
	node** LE = ToNeighbourList("grafy.txt", rozmiar);
	node** dij = new node * [rozmiar];
	int* color = new int[rozmiar];
	int* lenght = new int[rozmiar];
	int* father = new int[rozmiar];
	for (int i = 0; i < rozmiar; i++)
	{
		color[i] = 0;
		lenght[i] = -1;
		father[i] = -1;
	}
	color[start] = 1;
	lenght[start] = 0;

	int shortest = 1000;
	int pomocy;
	while (true)
	{
		for (int i = 0; i < rozmiar; i++)
		{
			if (color[i] == 2)
			{

				while (LE[i])
				{
					if (color[LE[i]->elem] != 2)
					{
						color[LE[i]->elem] = 1;
						lenght[LE[i]->elem] = lenght[LE[i]->elem] + LE[i]->val;
						father[LE[i]->elem] = i;
						if (shortest > LE[i]->val)
						{
							shortest = LE[i]->val;
							pomocy = i;
						}
					}
					LE[i] = LE[i]->next;
				}
			}
			else if(color[i] == 1)
			{

			}

		}
		
	}

	return dij;
}
//funkcja main
int main()
{
	
	int rozmiar = 0;

	int** M = ToMatrix("grafy.txt", rozmiar);
	Show_clearMatrix(M, rozmiar);
	spacer();

	node** LN = ToNeighbourList("grafy.txt", rozmiar);
	Show_NeighbourList(LN, rozmiar);
	spacer();

	node_kraw* LE = ToEdge("grafy.txt", rozmiar);
	Show_Edgelist(LE);
	spacer();

	node_kraw* krus = Kruskal("grafy.txt", rozmiar);
	Show_Edgelist(krus);
	spacer();

	node_kraw* prima = Prima("grafy.txt", rozmiar,2);
	Show_Edgelist(prima);
	spacer();

	node** dij = Dijkstry("grafy.txt", rozmiar,4);
	Show_NeighbourList(dij, rozmiar);
	spacer();

	return 0;

}