#include "lib.hpp"

class Ate
{
public:
	int a;
	int t;
	float e;
};

class Test
{
public:
	vector<Ate> v;
	Test(vector<Ate> &v) {
		this->v = v;
	}
	void print() {
		for (int i = 0; i < v.size(); i++) {
			cout << i << ": " << "(" << v[i].a << ", " << v[i].t << ", " << v[i].e << ")" << endl;
		}
	}
};

int main() {
	/* vector<Ate> v = {{-1, -1, 1}, {1, 0, 0}}; */
	/* Test t(v); */
	/* v[0].a = 9; */
	/* t.print(); */
	/* return 0; */
	FreeType ft("font.ttf");
	Window win("Title");

	Shader s("./vertex.glsl", "./fragment.glsl");

	Object o(s, {
		{3, 5, 0},
		{2, 5, 3},
	}, {
		-1, -1,
		+1, -1,
		+1, +1,
		+1, +1,
	}, {
	  0, 1, 2,
	  });
	cout << "1. " << o.attributes.size() << endl;

	while (!win.quit) {
		win.handleEvents();

		glViewport(0, 0, win.size[0], win.size[1]);
		glClearColor(.9, .7, .4, 1.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		o.draw();

		win.swapWindows();
	}
	return 0;
}
