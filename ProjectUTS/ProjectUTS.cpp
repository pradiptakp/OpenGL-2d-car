#include<windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string>

static float speed;
static float moveX;
static boolean reverse;
static boolean pressed;

typedef struct {
	float x, y;
} point2D_t;

typedef struct {
	float r, g, b, opacity;
} color_t;

typedef struct {
	float v[3];
} vector2D_t;

typedef struct {
	float m[3][3];
} matrix2D_t;

typedef struct {
	point2D_t p[100];
} object2D_t;

vector2D_t point2Vector(point2D_t p) {
	vector2D_t vec;
	vec.v[0] = p.x;
	vec.v[1] = p.y;
	vec.v[2] = 1;
	return vec;
}

point2D_t vector2Point(vector2D_t vec) {
	point2D_t p;
	p.x = vec.v[0];
	p.y = vec.v[1];

	return p;
}

vector2D_t operator * (matrix2D_t mat, vector2D_t vec) {
	vector2D_t vec1;
	for (int i = 0; i < 3; i++) {
		vec1.v[i] = 0;
		for (int j = 0; j < 3; j++) {
			vec1.v[i] += mat.m[i][j] * vec.v[j];
		}
	}
	return vec1;
}

matrix2D_t operator * (matrix2D_t mat1, matrix2D_t mat2) {
	matrix2D_t mat;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mat.m[i][j] = 0;
			for (int k = 0; k < 3; k++) {
				mat.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
			}
		}
	}
	return mat;
}

vector2D_t operator + (vector2D_t vec1, vector2D_t vec2) {
	vector2D_t vec;
	
	for (int i = 0; i < 3; i++) {
		vec.v[i] = vec1.v[i] + vec2.v[i];
	}

	return vec;
}

matrix2D_t createIdentity() {
	matrix2D_t mat;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mat.m[i][j] = 0;
		}
		mat.m[i][i] = 1;
	}
	return mat;
}

matrix2D_t translationMTX(float dx, float dy) {
	matrix2D_t mat = createIdentity();
	mat.m[0][2] = dx;
	mat.m[1][2] = dy;
	return mat;
}


matrix2D_t rotationMTX(float a) {
	matrix2D_t mat = createIdentity();
	mat.m[0][0] = cos(a);
	mat.m[0][1] = -sin(a);
	mat.m[1][0] = sin(a);
	mat.m[1][1] = cos(a);
	return mat;
}


void drawPolygon(point2D_t p[], int n) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++) {
		glVertex2f(p[i].x, p[i].y);
	}
	glEnd();
}

void drawCircle(float xp, float yp, float r, int n) {
	point2D_t p[360];
	float teta;
	for (int i = 0; i < n; i++) {
		teta = (float)i * 6.28 / n;
		p[i].x = xp + r * cos(teta);
		p[i].y = yp + r * sin(teta);
	}
	drawPolygon(p, n);
}

void drawInnerBanKiri(float xp, float yp, float r, int n) {
	point2D_t p[360];
	float teta;
	for (int i = 0; i < n; i++) {
		teta = (float)i * 3.35 / n;
		p[i].x = xp + r * -cos(teta);
		p[i].y = yp + r * -sin(teta);
	}
	drawPolygon(p, n);
}

void drawInnerBanKanan(float xp, float yp, float r, int n) {
	point2D_t p[360];
	float teta;
	for (int i = 0; i < n; i++) {
		teta = (float)i * -3.35 / n;
		p[i].x = xp + r * cos(teta);
		p[i].y = yp + r * sin(teta);
	}
	drawPolygon(p, n);
}

void drawPolygon(point2D_t p[], color_t col, int n) {
	glColor3f(col.r, col.g, col.b);
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++) {
		glVertex2f(p[i].x, p[i].y);
	}
	glEnd();
}

void drawPolygon(point2D_t p[], color_t col[], int n) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++) {
		glColor4f(col[i].r, col[i].g, col[i].b, col[i].opacity);
		glVertex2f(p[i].x, p[i].y);
	}
	glEnd();
}

void drawOval(int n, float rx, float ry, float xp, float yp) {
	point2D_t p[360];
	float teta;
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++) {
		teta = (float)i * 6.28 / n;
		p[i].x = xp + rx * cos(teta);
		p[i].y = yp + ry * sin(teta);
	}
	drawPolygon(p, n);
	glEnd();
}


void drawBan(float x, float y, float time) {
	float cx1 = x - 166;
	float cx2 = x + 186;
	float cy = y + 23;
	float rad = 0.785398;
	matrix2D_t mat = rotationMTX(time);
	matrix2D_t copier;
	vector2D_t vec;

	//sample
	point2D_t velg1Cont[4] = { {cx1 - 15, cy - 3},{cx1 - 15, cy + 3},{ cx1 - 30 , cy + 3}, {cx1 - 30, cy - 3} };
	point2D_t velg2Cont[4] = { {cx2 - 15, cy - 3},{cx2 - 15, cy + 3},{ cx2 - 30 , cy + 3}, {cx2 - 30, cy - 3} };

	// kiri
	vector2D_t tipusVelg1;
	tipusVelg1.v[0] = cx1;
	tipusVelg1.v[1] = cy;
	tipusVelg1.v[2] = 1;

	glColor3f(.12, .12, .12);
	drawCircle(cx1, cy, 45, 300);
	glColor3f(.01, .01, .01);
	drawCircle(cx1, cy, 34, 300);
	glColor3f(.12, .12, .12);
	drawCircle(cx1, cy, 32, 300);
	glColor3f(.01, .01, .01);
	drawCircle(cx1, cy, 30, 300);
	glColor3f(.09, .09, .09);
	drawCircle(cx1, cy, 22, 300);
	glColor3f(.01, .01, .01);
	drawCircle(cx1, cy, 20, 300);
	glColor3f(.38, .38, .38);
	drawCircle(cx1, cy, 12, 300);
	glColor3f(.01, .01, .01);
	drawCircle(cx1, cy, 10, 300);
	glColor3f(.38, .38, .38);
	drawCircle(cx1, cy, 4, 300);
	glColor3f(.38, .38, .38);

	//nggambar
	point2D_t velg1[8][4];
	for (int i = 0; i < 4; i++) {
		velg1[0][i] = velg1Cont[i];
	}

	for (int i = 1; i < 8; i++) {
		copier = rotationMTX(rad * i);
		for (int j = 0; j < 4; j++) {
			vec = point2Vector(velg1[0][j]);
			vec.v[0] = vec.v[0] - tipusVelg1.v[0];
			vec.v[1] = vec.v[1] - tipusVelg1.v[1];
			vec = (copier * vec) + tipusVelg1;
			velg1[i][j] = vector2Point(vec);
		}
	}

	//kanan
	vector2D_t tipusVelg2;
	tipusVelg2.v[0] = cx2;
	tipusVelg2.v[1] = cy;
	tipusVelg2.v[2] = 1;

	glColor3f(.12, .12, .12);
	drawCircle(cx2, cy, 45, 300);
	glColor3f(.01, .01, .01);
	drawCircle(cx2, cy, 34, 300);
	glColor3f(.12, .12, .12);
	drawCircle(cx2, cy, 32, 300);
	glColor3f(.01, .01, .01);
	drawCircle(cx2, cy, 30, 300);
	glColor3f(.09, .09, .09);
	drawCircle(cx2, cy, 22, 300);
	glColor3f(.01, .01, .01);
	drawCircle(cx2, cy, 20, 300);
	glColor3f(.38, .38, .38);
	drawCircle(cx2, cy, 12, 300);
	glColor3f(.01, .01, .01);
	drawCircle(cx2, cy, 10, 300);
	glColor3f(.38, .38, .38);
	drawCircle(cx2, cy, 4, 300);
	glColor3f(.38, .38, .38);

	//nggambar
	point2D_t velg2[8][4];
	for (int i = 0; i < 4; i++) {
		velg2[0][i] = velg2Cont[i];
	}


	for (int i = 1; i < 8; i++) {
		copier = rotationMTX(rad * i);
		for (int j = 0; j < 4; j++) {
			vec = point2Vector(velg2[0][j]);
			vec.v[0] = vec.v[0] - tipusVelg2.v[0];
			vec.v[1] = vec.v[1] - tipusVelg2.v[1];
			vec = (copier * vec) + tipusVelg2;
			velg2[i][j] = vector2Point(vec);
		}
	}


	//mubeng


	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 4; j++) {
			vec = point2Vector(velg1[i][j]);
			vec.v[0] = vec.v[0] - tipusVelg1.v[0];
			vec.v[1] = vec.v[1] - tipusVelg1.v[1];
			vec = (mat * vec) + tipusVelg1;
			velg1[i][j] = vector2Point(vec);

			vec = point2Vector(velg2[i][j]);
			vec.v[0] = vec.v[0] - tipusVelg2.v[0];
			vec.v[1] = vec.v[1] - tipusVelg2.v[1];
			vec = (mat * vec) + tipusVelg2;
			velg2[i][j] = vector2Point(vec);
		}
		drawPolygon(velg1[i], 4);
		drawPolygon(velg2[i], 4);
	}
	
}

void drawCar(float x, float y, float time) {

	float cx1 = x - 166;
	float cx2 = x + 186;
	float cy = y + 23;

	point2D_t decor8[4] = { {x + 292, y - 41}, {x + 292, y - 65}, {x + 283, y - 65}, {x + 262, y - 41} };
	glColor3f(.08, .08, .08);
	drawPolygon(decor8, 4);

	point2D_t rangka[10] = {
		{x - 295, y - 20},
		{x - 297, y - 30},
		{x - 297, y - 46},
		{x - 283, y - 80},
		{x - 254, y - 80},
		{x - 132, y - 130},
		{x + 30, y - 125},
		{ x + 112 , y - 75},
		{x + 292, y - 41},
		{x + 295, y - 16},
	};
	glColor3f(.90, .90, .96);
	drawPolygon(rangka, 10);


	point2D_t jendelo1[5] = { { x - 205 , y - 80}, { x - 200 , y - 72}, { x - 94 , y - 70}, { x - 80 , y - 115}, { x - 126 , y - 116} };
	glColor3f(.12, .17, .22);
	drawPolygon(jendelo1, 5);

	point2D_t jendelo2[3] = { { x - 254 , y - 80}, { x - 236 , y - 80}, { x - 155 , y - 121} };
	drawPolygon(jendelo2, 3);

	point2D_t jendelo3[4] = { { x - 65 , y - 115}, { x - 74 , y - 70},  { x + 74 , y - 70}, { x + 13 , y - 115} };
	drawPolygon(jendelo3, 4);


	point2D_t jendelo4[4] = { { x + 34 , y - 115}, { x + 101 , y - 70},  { x + 112 , y - 75}, { x + 47 , y - 115} };
	drawPolygon(jendelo4, 4);

	point2D_t pintu[4] = { {x - 70, y -60 }, {x - 70, y-50 }, {x - 45, y-50 }, {x - 45, y-60 } };
	glColor3f(.04, .04, .04);
	drawPolygon(pintu, 4);

	point2D_t lampu1[4] = {
		{x + 293, y - 38},
		{x + 295, y - 16},
		{x + 260, y - 16},
		{x + 260, y - 38} };
	glColor3f(.8, .2, .2);
	drawPolygon(lampu1, 4);

	point2D_t cahayaCinta[4] = {

		{x + 292, y - 65},
		{x + 292, y - 45},
		{x + 500, y + 15},
		{x + 500, y - 115}
	};
	point2D_t cahayaCinta2[4] = {

		{x + 293, y - 38},
		{x + 295, y - 26},
		{x + 400, y - 6},
		{x + 400, y - 48}
	};
	color_t colorCahaya[4] = {
		{1, 1, 1, .2},
		{1, 1, 1, .2},
		{1, 1, 1, 0},
		{1, 1, 1, 0},
	};
	drawPolygon(cahayaCinta, colorCahaya, 4);
	drawPolygon(cahayaCinta2, colorCahaya, 4);


	point2D_t decor2[4] = { {x - 307, y - 13}, {x - 307, y + 1}, {x + 305, y + 1}, {x + 305, y - 13} };
	glColor3f(.90, .90, .96);
	drawPolygon(decor2, 4);

	point2D_t decor1[5] = { {x - 307, y - 13}, {x - 307, y - 25}, {x - 295, y - 29}, {x + 303, y - 25}, {x + 303, y - 13} };
	glColor3f(.08, .08, .08);
	drawPolygon(decor1, 5);
	drawInnerBanKiri(cx1, cy, 65, 300);
	drawInnerBanKanan(cx2, cy, 65, 300);


	point2D_t decor3[5] = { {x - 307, y + 1}, {x - 166, y + 1}, {x - 166, y + 26}, {x - 288, y + 20}, {x - 288, y + 17} };
	glColor3f(.08, .08, .08);
	drawPolygon(decor3, 5);

	point2D_t decor4[4] = { {x - 166, y + 1}, {x - 166, y + 36}, {x + 170, y + 36}, {x + 170, y + 1} };
	drawPolygon(decor4, 4);

	point2D_t decor5[4] = { {x + 206, y + 1}, {x + 300, y + 1}, {x + 300, y + 24}, {x + 206, y + 24} };
	drawPolygon(decor5, 4);

	point2D_t decor6[4] = { {x + 206, y + 1}, {x + 303, y + 1}, {x + 303, y + 10}, {x + 206, y + 10} };
	drawPolygon(decor6, 4);

	point2D_t decor7[3] = { { x + 101 , y - 70},  { x + 112 , y - 75},{x + 292, y - 41} };
	drawPolygon(decor7, 3);



	glColor3f(.02, .02, .02);
	drawInnerBanKiri(cx1, cy, 53, 300);
	drawInnerBanKanan(cx2, cy, 53, 300);

	glColor4f(0, 0, 0, .3);

	point2D_t shadow[4] = { {x - 307, y + 60}, {x - 307, y + 70}, {x + 303, y + 70}, {x + 303, y + 60} };
	drawPolygon(shadow, 4);

	drawBan(x, y, time);
}

void drawRoadTexture(float time) {
	static point2D_t texture1[100];
	static point2D_t texture2[100];
	glPointSize(2);

	if (time == 0) {
		for (int i = 0; i < 100; i++)
		{
			texture1[i].x = (rand() % 961);
			texture1[i].y = (rand() % 141);
		}
		for (int i = 0; i < 100; i++)
		{
			texture2[i].x = 960 + (rand() % 961);
			texture2[i].y = (rand() % 141);
		}
		texture1[0].x = 0;
		texture2[0].x = 961;
	}

	matrix2D_t mat = translationMTX(-speed * 100, 0);
	vector2D_t vec;

	for (int i = 0; i < 100; i++) {
		vec = point2Vector(texture1[i]);
		vec = mat * vec;
		texture1[i] = vector2Point(vec);
	}
	for (int i = 0; i < 100; i++) {
		vec = point2Vector(texture2[i]);
		vec = mat * vec;
		texture2[i] = vector2Point(vec);
	}

	if (texture1[0].x < -960) {
		for (int i = 0; i < 100; i++) {
			vec = point2Vector(texture1[i]);
			vec = translationMTX(1920, 0) * vec;
			texture1[i] = vector2Point(vec);
		}
	}
	if (texture2[0].x < -960) {
		for (int i = 0; i < 100; i++) {
			vec = point2Vector(texture2[i]);
			vec = translationMTX(1920, 0) * vec;
			texture2[i] = vector2Point(vec);
		}
	}


	glColor4f(1, 1, 1, .2);
	glBegin(GL_POINTS);
	for (int i = 0; i < 100; i++)
	{
		if (i == 50)
			glColor4f(.5, .5, .5, .4);
		if (i == 100)
			glColor4f(.7, .7, .7, .4);
		if (i == 150)
			glColor4f(0, 0, 0, .4);
		glVertex2f(texture1[i].x, texture1[i].y + 400);
	}
	for (int i = 0; i < 100; i++)
	{
		if (i == 50)
			glColor3f(.5, .5, .5);
		if (i == 100)
			glColor3f(.7, .7, .7);
		if (i == 150)
			glColor3f(0, 0, 0);
		glVertex2f(texture2[i].x, texture2[i].y + 400);
	}
	glEnd();
}

void drawGedung(float time) {
	float x = 0;
	float y = 350;
	glColor3f(.16, .20, .30);
	point2D_t bgGedung[4] = {
		{0, y + 100}, {0, y - 30}, {960, y - 30}, {960, y + 100}
	};
	drawPolygon(bgGedung, 4);

	static point2D_t tipeGedung[10][7] = {
		{{x - 15, y}, {x - 15, y - 120}, {x + 15, y - 120}, {x + 15, y }, {0,0}, {0,0},{0,0}},
		{{x - 20, y}, {x - 20, y - 110}, {x + 20, y - 110}, {x + 20, y }, {0,0}, {0,0},{0,0}},
		{{x - 23, y}, {x - 23, y - 90}, {x + 23, y - 90}, {x + 23, y }, {0,0}, {0,0},{0,0}},
		{{x - 26, y}, {x - 26, y - 80}, {x + 26, y - 80}, {x + 26, y }, {0,0}, {0,0},{0,0}},
		{{x - 17, y}, {x - 17, y - 50}, {x + 17, y - 50}, {x + 17, y }, {0,0}, {0,0},{0,0}},
		{{x - 19, y}, {x - 19, y - 70}, {x + 19, y - 70}, {x + 19, y }, {0,0}, {0,0},{0,0}},
		{{x - 17, y}, {x - 17, y - 100}, {x + 17, y - 100}, {x + 17, y }, {0,0}, {0,0},{0,0}},
		{{x - 20, y}, {x - 20, y - 130}, {x + 20, y - 130}, {x + 20, y }, {0,0}, {0,0},{0,0}},
		{{x - 21, y}, {x - 21, y - 140}, {x + 21, y - 140}, {x + 21, y }, {0,0}, {0,0},{0,0}},
		{{x - 15, y}, {x - 15, y - 150}, {x + 15, y - 150}, {x + 15, y }, {0,0}, {0,0},{0,0}},
	};
	static point2D_t gedungs1[30][7];
	static point2D_t gedungs2[30][7];
	int gedungnya = 0, vertexCount, gedungBefore = 0;

	point2D_t tessss[7] = { {x - 10, y}, {x - 10, y - 40}, {x + 10, y - 40}, {x + 10, y }, {0,0}, {0,0},{0,0} };

	matrix2D_t mat;
	vector2D_t vec;

	if (time == 0) {
		for (int i = 0; i < 30; i++) {
			vertexCount = 0;
			while(gedungnya == gedungBefore)
				gedungnya = (int)rand() % 10;
			mat = translationMTX(i * ( (int)(rand() % 2 + rand() % 1 ) + 37 ), 0);
			//mat = translationMTX(i * 40, 0);
			for (int j = 0; j < 7; j++) {
				if (tipeGedung[gedungnya][j].x != 0 && tipeGedung[gedungnya][j].y != 0) {
					gedungs1[i][j] = tipeGedung[gedungnya][j];
					vec = point2Vector(gedungs1[i][j]);
					vec = mat * vec;
					gedungs1[i][j] = vector2Point(vec);
					vertexCount++;

					gedungs2[i][j] = gedungs1[i][j];
					vec = point2Vector(gedungs2[i][j]);
					vec = translationMTX(920, 0) * vec;
					gedungs2[i][j] = vector2Point(vec);
				}
			}
			gedungBefore = gedungnya;

		}
	}

	if (gedungs1[0][0].x < -1180) {
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 7; j++) {
				vec = point2Vector(gedungs1[i][j]);
				vec = translationMTX(2160, 0) * vec;
				gedungs1[i][j] = vector2Point(vec);
			}

		}
	}
	if (gedungs2[0][0].x < -1180) {
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 7; j++) {
				vec = point2Vector(gedungs2[i][j]);
				vec = translationMTX(2160, 0) * vec;
				gedungs2[i][j] = vector2Point(vec);
			}

		}
	}


	for (int i = 0; i < 30; i++) {
		vertexCount = 0;
		for (int j = 0; j < 7; j++) {
			if (gedungs1[i][j].x != 0 && gedungs1[i][j].y != 0) {
				vec = point2Vector(gedungs1[i][j]);
				vec = translationMTX(-speed, 0) * vec;
				gedungs1[i][j] = vector2Point(vec);

				vec = point2Vector(gedungs2[i][j]);
				vec = translationMTX(-speed, 0) * vec;
				gedungs2[i][j] = vector2Point(vec);

				vertexCount++;
			}
		}
		drawPolygon(gedungs1[i], vertexCount);
		drawPolygon(gedungs2[i], vertexCount);
	}

}

void drawPager(float time) {
	vector2D_t vec;
	static point2D_t pager2[4] = { {0, 350}, {960, 350}, {960, 340}, {0, 340} };
	static point2D_t pager1[4] = { {0, 367}, {960, 367}, {960, 360}, {0, 360} };
	glColor3f(.02, .02, .02);

	static point2D_t pager3[4] = { {0, 384}, {960, 384}, {960, 377}, {0, 377} };
	static point2D_t pager4[4] = { {0, 400}, {960, 400}, {960, 395}, {0, 395} };

	point2D_t tiangAwal[4] = { { -5, 400 }, { 5, 400 }, { 5, 340 }, { -5, 340 } };


	//drawPolygon(tiangAwal, 4);

	static point2D_t tiang[6][4];
	static point2D_t tiang2[6][4];
	static point2D_t tiang3[6][4];
	if (time == 0) {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				vec = point2Vector(tiangAwal[j]);
				vec = translationMTX((float)i * 192, 0) * vec;
				tiang[i][j] = vector2Point(vec);
			}
		}

		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				vec = point2Vector(tiangAwal[j]);
				vec = translationMTX((float)i * 192 + 960, 0) * vec;
				tiang2[i][j] = vector2Point(vec);
			}
		}

		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				vec = point2Vector(tiangAwal[j]);
				vec = translationMTX((float)i * -192, 0) * vec;
				tiang3[i][j] = vector2Point(vec);
			}
		}
	}
	
	drawPolygon(pager1, 4);
	drawPolygon(pager2, 4);
	drawPolygon(pager3, 4);
	glColor3f(.02, .02, .02);

	if (tiang[0][0].x < -960) {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				vec = point2Vector(tiang[i][j]);
				vec = translationMTX(1920, 0) * vec;
				tiang[i][j] = vector2Point(vec);
			}
		}
	}
	if (tiang2[0][0].x < -960) {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				vec = point2Vector(tiang2[i][j]);
				vec = translationMTX(1920, 0) * vec;
				tiang2[i][j] = vector2Point(vec);
			}
		}
	}
	if (tiang3[0][0].x < -960) {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				vec = point2Vector(tiang3[i][j]);
				vec = translationMTX(1920, 0) * vec;
				tiang3[i][j] = vector2Point(vec);
			}
		}
	}

	if (reverse && tiang[5][1].x > 1920) {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				vec = point2Vector(tiang[i][j]);
				vec = translationMTX(-1920, 0) * vec;
				tiang[i][j] = vector2Point(vec);
			}
		}
	}
	if (reverse && tiang2[5][1].x > 1920) {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				vec = point2Vector(tiang2[i][j]);
				vec = translationMTX(-1920, 0) * vec;
				tiang2[i][j] = vector2Point(vec);
			}
		}
	}
	if (reverse && tiang3[5][1].x > 1920) {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				vec = point2Vector(tiang3[i][j]);
				vec = translationMTX(-1920, 0) * vec;
				tiang3[i][j] = vector2Point(vec);
			}
		}
	}


	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			vec = point2Vector(tiang[i][j]);
			vec = translationMTX(-speed * 100, 0) * vec;
			tiang[i][j] = vector2Point(vec);

			vec = point2Vector(tiang2[i][j]);
			vec = translationMTX(-speed * 100, 0) * vec;
			tiang2[i][j] = vector2Point(vec);

			vec = point2Vector(tiang3[i][j]);
			vec = translationMTX(-speed * 100, 0) * vec;
			tiang3[i][j] = vector2Point(vec);
		}
		drawPolygon(tiang[i], 4);
		drawPolygon(tiang2[i], 4);
		drawPolygon(tiang3[i], 4);
	}
	glColor3f(.4, .4, .4);
	drawPolygon(pager4, 4);
	/*
	for (int j = 0; j < 4; j++) {
		vec = point2Vector(tiang[i][j]);
		vec = translationMTX(-speed, 0) * vec;
		tiang[i][j] = vector2Point(vec);
	}*/

}

void drawBackground(float time) {
	point2D_t sky[4] = { {0, 470}, {960, 470}, {960, 0}, {0, 0} };
	glColor3f(.10, .13, .20);
	drawPolygon(sky, 4);
	int radius;
	vector2D_t vec;
	static point2D_t bulan = { 900, 30 };


	vec = point2Vector(bulan);
	vec = translationMTX(-speed / 100, 0) * vec;
	bulan = vector2Point(vec);

	glColor4f(1, 1, 1, 1);
	drawCircle(bulan.x, bulan.y, 40, 300);
	glColor4f(1, 1, 1, .1);
	drawCircle(bulan.x, bulan.y, 50, 300);
	glColor4f(1, 1, 1, .05);
	drawCircle(bulan.x, bulan.y, 60, 300);
	glColor4f(1, 1, 1, .01);
	drawCircle(bulan.x, bulan.y, 70, 300);


	static point2D_t bintang[50];

	if (time == 0) {
		for (int i = 0; i < 50; i++) {
			bintang[i].x = rand() % 1920;
			bintang[i].y = rand() % 270;
		}
	}

	for (int i = 0; i < 50; i++) {
		if (i < 10) {
			radius = 1;
			glColor4f(1, 1, 1, 1);
		}
		else if (i < 20) {
			radius = 1;
			glColor4f(1, 1, 1, .8);
		}
		else if (i < 30) {
			radius = 1;
			glColor4f(1, 1, 1, .5);
		}
		else if (i < 40) {
			radius = 1;
			glColor4f(1, 1, 1, .3);
		}
		else if (i < 50) {
			radius = 2;
			glColor4f(1, 1, 1, 1);
		}


		vec = point2Vector(bintang[i]);
		vec = translationMTX(-speed / 100, 0) * vec;
		bintang[i] = vector2Point(vec);

		drawCircle(bintang[i].x, bintang[i].y, radius, 50);
	}

	drawGedung(time);

	drawPager(time);

	point2D_t ground[4] = { {0, 540}, {960, 540}, {960, 400}, {0, 400} };
	glColor3f(.2, .2, .2);
	drawPolygon(ground, 4);

	point2D_t stripes[4] = { {0, 415}, {960, 415}, {960, 410}, {0, 410} };
	glColor3f(.8, .8, .8);
	drawPolygon(stripes, 4);

	point2D_t ystripes[4] = { {0, 480}, {960, 480}, {960, 485}, {0, 485} };
	glColor3f(.91, .80, .07);
	drawPolygon(ystripes, 4);

	point2D_t ystripes2[4] = { {0, 495}, {960, 495}, {960, 500}, {0, 500} };
	drawPolygon(ystripes2, 4);

	drawRoadTexture(time);

	

}


void drawBitmapText(char* string, float x, float y, float z)
{
	char* c;
	glRasterPos3f(x, y, z);

	for (c = string; *c; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}
}

void drawBitmapText(char* string, float x, float y, float z, char* textSpeed)
{
	char* c;
	char* a;
	glRasterPos3f(x, y, z);

	for (a = textSpeed; *a; a++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *a);
	}
	for (c = string; *c; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}
}


void drawDesbor() {
	point2D_t desbor[4] = { {0, 0}, {960, 0}, {960, 30}, {0, 30} };
	glColor4f(0, 0, 0, .75);
	drawPolygon(desbor, 4);
	glColor3f(1, 1, 1);

	signed int number = round(speed * 1500.0);
	char textSpeed[10];
	sprintf_s(textSpeed, "%ld", number);

	drawBitmapText((char *) " KM/H", 20, 20, 0, (char *) textSpeed);
	drawBitmapText((char*) "D to accelerate, A to brake/reverse", 650, 20, 0);
}


void userdraw(void) {
	static float time = 0;
	static float posX = 480;
	
	drawBackground(time);
	drawCar(posX, 380, time);
	drawDesbor();

	if (speed > 0.001) {
		if (posX > 350) {
			moveX -= .00001;
		}
		else if (posX < 350) {
			moveX += .0001;
		}
		else if (posX > 440) {
			moveX -= .001;
		}
		else if (posX > 460) {
			moveX -= .003;
		}
		else {
			moveX = 0;
		}
		posX += moveX;
	}
	else {
		if (posX < 480)
			posX += .0001;
		else if (posX > 480)
			posX -= .0001;
	}

	if (!pressed) {
		if (speed > 0) {
			speed -= .000005;
		}
		else if (speed < 0) {
			speed += .000005;
		}
	}

	time += speed;
}


void keyboard(unsigned char key, int x, int y) {
	if (key == 'a') {
		pressed = true;
		if (speed > 0) {
			reverse = false;
			speed -= 0.001;
			if (speed < 0)
				speed = 0;
		}
		else if (reverse && speed > -0.0065) {
			speed -= 0.0001;

		}
	}
	else if (key == 'd') {
		reverse = false;
		pressed = true;
		if (speed < 0.1) {
			speed += 0.0001;
		}
		
	}
	
}

void keyup(unsigned char key, int x, int y) {
	if (key == 'a') {
		if (speed == 0) {
			reverse = true;
		}
		pressed = false;
	}
	else if (key == 'd') {
		pressed = false;
		reverse = true;
	}
	else if (key == 27) {
		exit(0);
	}
}


void display(void) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT);
	userdraw();
	glutSwapBuffers();
}


int main(int argc, char** argv)
{
	speed = 0.0005;
	moveX = 0;
	reverse = true;
	pressed = false; 

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(960, 540);
	glutCreateWindow("Tukang Tahu by Angga");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(0, 960, 540, 0);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glutIdleFunc(display);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutKeyboardFunc(keyboard); 
	glutKeyboardUpFunc(keyup);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
