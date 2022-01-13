#include "objects.h"

void Objects::drawSkyBox(GLuint texture) {

	static float skyboxVertices[] = {

		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f

	};

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, skyboxVertices);

	glEnableVertexAttribArray(0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);

}

void Objects::drawSquare(GLuint texture) {

	static GLfloat vVertices[] = {

		-1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f

	};


	static GLfloat vColors[] = {

		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f

	};

	static GLfloat vTexCoords[] = {

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f

	};

	glBindTexture(GL_TEXTURE_2D, texture);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, vColors);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, vTexCoords);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

}

/*

void Objects::genPlane(void) {

	const float width = 2.0f;
	const float height = 2.0f;

	const float halfWidth = width * 0.5f;
	const float halfHeight = height * 0.5f;

	const float texMul = 1.0f;

	int currentVert = -1;
	int currentIndex = -1;

	for (int h = 0; h < RECT_VERTICE_H; h++) {

		for (int w = 0; w < RECT_VERTICE_W; w++) {

			//========= 6 vertices to form one sub-rectangle

			//1st vertex
			int vertex1 = ++currentVert;

			mRectVertices[vertex1 * 3] = -halfWidth + (float)(w) / (float)RECT_VERTICE_W * width;
			mRectVertices[vertex1 * 3 + 1] = -halfHeight + (float)(h) / (float)RECT_VERTICE_H * height;
			mRectVertices[vertex1 * 3 + 2] = 0.0f;

			mRectUV[vertex1 * 2] = (float)(w) / (float)RECT_VERTICE_W * texMul;
			mRectUV[vertex1 * 2 + 1] = (float)(h) / (float)RECT_VERTICE_H * texMul;

			//2nd vertex
			int vertex2 = ++currentVert;

			mRectVertices[vertex2 * 3] = -halfWidth + (float)(w) / (float)RECT_VERTICE_W * width;
			mRectVertices[vertex2 * 3 + 1] = -halfHeight + (float)(h + 1) / (float)RECT_VERTICE_H * height;
			mRectVertices[vertex2 * 3 + 2] = 0.0f;

			mRectUV[vertex2 * 2] = (float)(w) / (float)RECT_VERTICE_W * texMul;
			mRectUV[vertex2 * 2 + 1] = (float)(h + 1) / (float)RECT_VERTICE_H * texMul;


			//3rd vertex
			int vertex3 = ++currentVert;

			mRectVertices[vertex3 * 3] = -halfWidth + (float)(w + 1) / (float)RECT_VERTICE_W * width;
			mRectVertices[vertex3 * 3 + 1] = -halfHeight + (float)(h + 1) / (float)RECT_VERTICE_H * height;
			mRectVertices[vertex3 * 3 + 2] = 0.0f;

			mRectUV[vertex3 * 2] = (float)(w + 1) / (float)RECT_VERTICE_W * texMul;
			mRectUV[vertex3 * 2 + 1] = (float)(h + 1) / (float)RECT_VERTICE_H * texMul;


			//4th vertex
			int vertex4 = ++currentVert;

			mRectVertices[vertex4 * 3] = mRectVertices[vertex3 * 3];
			mRectVertices[vertex4 * 3 + 1] = mRectVertices[vertex3 * 3 + 1];
			mRectVertices[vertex4 * 3 + 2] = mRectVertices[vertex3 * 3 + 2];

			mRectUV[vertex4 * 2] = mRectUV[vertex3 * 2];
			mRectUV[vertex4 * 2 + 1] = mRectUV[vertex3 * 2 + 1];


			//5th vertex
			int vertex5 = ++currentVert;

			mRectVertices[vertex5 * 3] = -halfWidth + (float)(w + 1) / (float)RECT_VERTICE_W * width;
			mRectVertices[vertex5 * 3 + 1] = -halfHeight + (float)(h) / (float)RECT_VERTICE_H * height;
			mRectVertices[vertex5 * 3 + 2] = 0.0f;

			mRectUV[vertex5 * 2] = (float)(w + 1) / (float)RECT_VERTICE_W * texMul;
			mRectUV[vertex5 * 2 + 1] = (float)(h) / (float)RECT_VERTICE_H * texMul;

			//6th vertex
			int vertex6 = ++currentVert;

			mRectVertices[vertex6 * 3] = mRectVertices[vertex1 * 3];
			mRectVertices[vertex6 * 3 + 1] = mRectVertices[vertex1 * 3 + 1];
			mRectVertices[vertex6 * 3 + 2] = mRectVertices[vertex1 * 3 + 2];

			mRectUV[vertex6 * 2] = mRectUV[vertex1 * 2];
			mRectUV[vertex6 * 2 + 1] = mRectUV[vertex1 * 2 + 1];

		}

	}
}

*/