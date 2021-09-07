#include<iostream>
#include<vector>
#include<random>
#include<chrono>
using namespace std;

bool flood_fill(const vector<vector<bool>>& v, const int& x, const int& y, const int& w, const int& h, vector<vector<bool>>& visited) {
	if (x < 0 || x >= w || y < 0 || y >= h)
		return false;
	if (v[x][y] || visited[x][y])
		return false;
	visited[x][y] = true;
	if (x == w - 1 && y == h - 1)
		return true;
	return flood_fill(v, x - 1, y, w, h, visited) || flood_fill(v, x, y - 1, w, h, visited) || flood_fill(v, x + 1, y, w, h, visited) || flood_fill(v, x, y + 1, w, h, visited);
}

bool check_hole(const vector<vector<bool>>& v, const vector<vector<bool>>& visited, const int& h, const int &w) {
	for (int r = 0; r < h; r++)
		for (int c = 0; c < w; c++)
			if (!(v[r][c] || visited[r][c]))
				return false;
	return true;
}

bool check_lump(const vector<vector<bool>>& v, const int& x, const int& y, const int& h, const int& w) {
	if (x == w - 1)
		return (y > 0 ? v[x][y - 1] : false) || (y < w - 1 ? v[x][y + 1] : false);
	if (y == h - 1)
		return (x > 0 ? v[x - 1][y] : false) || (x < h - 1 ? v[x + 1][y] : false);
	if (x == 0)
		return (y > 0 ? v[x][y - 1] : false) || (y < w - 1 ? v[x][y + 1] : false);
	if (y == 0)
		return (y > 0 ? v[x][y - 1] : false) || (y < w - 1 ? v[x][y + 1] : false);
	return v[x + 1][y] && v[x][y + 1] && v[x + 1][y + 1] ||
		v[x + 1][y] && v[x][y - 1] && v[x + 1][y - 1] ||
		v[x - 1][y] && v[x][y + 1] && v[x - 1][y + 1] ||
		v[x - 1][y] && v[x][y - 1] && v[x - 1][y - 1];
}

bool check_edge(const vector<vector<bool>>& v, const int& x, const int& y, const int& h, const int& w) {
	if (x == h - 1 && y == 0 || x == 0 && y == w - 1)
		return true;
	if (x > 0 && x < h - 1 && y > 0 && y < w - 1)
		return false;
	return (x == 0 || x == h - 1) && (v[x][y - 1] || v[x][y + 1]) || (y == 0 || y == w - 1) && (v[x - 1][y] || v[x + 1][y]);
}

void draw_map(const vector<vector<bool>>& v, const int& h, const int& w, const bool& flag = true) {
	if (flag) {
		for (int c = 0; c < w + 2; c++)
			cout << "■";
		cout << endl;
	}
	for (int r = 0; r < h; r++) {
		cout << (r ? "■" : "  ");
		for (int c = 0; c < w; c++)
			cout << (v[r][c] ? "■" : "  ");
		cout << ((r < h - 1) ? "■" : "  ");
		cout << endl;
	}
	if (flag) {
		for (int c = 0; c < w + 2; c++)
			cout << "■";
		cout << endl;
	}
	cout << endl;
}

int main() {
	int w, h;
	cin >> w >> h;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dist(1, w * h - 1); // 시작점과 맨 끝점은 뺌
	vector<vector<bool>> v(h);
	vector<bool> check(w * h, false);
	for (int r = 0; r < h; r++) {
		v[r] = vector<bool>(w);
		for (int c = 0; c < w; c++)
			v[r][c] = false;
	}

	chrono::system_clock::time_point start = chrono::system_clock::now();

	for (int i = 0; i < 10000; i++) {
		vector<vector<bool>> visited(h, vector<bool>(w, false));
		int block = dist(gen);
		if (check[block])
			continue;
		int r = block / w, c = block % w;
		v[r][c] = true;
		// 1. 도착점까지 못가면 안되고..
		// 2. 구멍이 생기면 안되고..
		// 3. 너무 튼튼하게(?) 막으면 안되고..
		// 4. 가장자리에 연속해서 있으면 안되고..
		if (!(flood_fill(v, 0, 0, w, h, visited) && check_hole(v, visited, h, w)) || check_lump(v, r, c, h, w) || check_edge(v, r, c, h, w)) {
			v[r][c] = false;
			continue;
		}
		check[block] = true;
	}

	chrono::duration<double> duration = chrono::system_clock::now() - start;

	draw_map(v, h, w);

	cout << "elapsed time : " << duration.count() << " seconds" << endl;	
}