// Azlan Ali Khan 24I-2110 DSA FINAL PROJECT

const int M = 25;
const int N = 40;

const int ts = 18; //tile size

class Grid {
private:
	int* grid;

public:
	Grid(int rows, int cols, int tileSize);

	~Grid();

	// GETTERS
	int getCell(int y, int x) const;
	int getRows() const;
	int getCols() const;
	
	// SETTERS
	void setCell(int y, int x, int value);

	void drop(int y, int x);
	void clear();

private:
	int M, N, ts;

};