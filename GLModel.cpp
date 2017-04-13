#define _CRT_SECURE_NO_WARNINGS

/*

Outracks GLWrapper
Tiny OpenGL wrapping framework for demo use.

You are free to redistribute, change and use this code without charge.
The code is provided "as is", with no warranties or support.

*/

#include "includes/GLWRAPPER.H"

namespace GLWrapper
{
	Mesh::Mesh(int vc, int tc)
	{
		vertexCount = vc;
		triangleCount = tc;
		vertexArray = new vertex[vc];
		normalArray = new vertex[vc];
		texcoordArray = new texcoord[vc];
		triangleArray = new triangle[tc];
	}
	Mesh::~Mesh()
	{
		delete vertexArray, normalArray, texcoordArray, triangleArray;
	}
	void Mesh::offset(float x, float y, float z)
	{
		for (int i = 0; i < vertexCount; i++)
		{
			vertexArray[i].x += x;
			vertexArray[i].y += y;
			vertexArray[i].z += z;
		}
	}
	void Mesh::scale(float x, float y, float z)
	{
		for (int i = 0; i < vertexCount; i++)
		{
			vertexArray[i].x *= x;
			vertexArray[i].y *= y;
			vertexArray[i].z *= z;
		}
	}
	void Mesh::calcNormals()
	{
		for (int i = 0; i < vertexCount; i++)
		{
			normalArray[i].x = 0.0f;
			normalArray[i].y = 0.0f;
			normalArray[i].z = 0.0f;
		}

		for (int i = 0; i < triangleCount; i++)
		{
			vertex a, b, c;
			a = vertexArray[triangleArray[i].a];
			b = vertexArray[triangleArray[i].b];
			c = vertexArray[triangleArray[i].c];
			b.x = b.x-a.x;
			b.y = b.y-a.y;
			b.z = b.z-a.z;
			c.x = c.x-a.x;
			c.y = c.y-a.y;
			c.z = c.z-a.z;
			a = crossProd(b, c);
			normalArray[triangleArray[i].a].x += a.x;
			normalArray[triangleArray[i].a].y += a.y;
			normalArray[triangleArray[i].a].z += a.z;
			normalArray[triangleArray[i].b].x += a.x;
			normalArray[triangleArray[i].b].y += a.y;
			normalArray[triangleArray[i].b].z += a.z;
			normalArray[triangleArray[i].c].x += a.x;
			normalArray[triangleArray[i].c].y += a.y;
			normalArray[triangleArray[i].c].z += a.z;
		}

		for (int i = 0; i < vertexCount; i++)
		{
			float l = sqrtf(normalArray[i].x*normalArray[i].x + normalArray[i].y*normalArray[i].y + normalArray[i].z*normalArray[i].z);
			normalArray[i].x /= l;
			normalArray[i].y /= l;
			normalArray[i].z /= l;
		}
	}
	void Mesh::draw()
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3,GL_FLOAT,0,vertexArray);
		glNormalPointer(GL_FLOAT,0,normalArray);
		glTexCoordPointer(2,GL_FLOAT,0,texcoordArray);

		glDrawElements(GL_TRIANGLES, triangleCount*3, GL_UNSIGNED_INT, triangleArray);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	Model::Model(char *fileName)
	{
		FILE *f = fopen(fileName, "rb");
		if (!f) 
		{
			MessageBox(0,"Error: .ebr file not found",fileName,0);
		}
		else
		{
			fread(&meshCount, 4, 1, f);
			meshArray = new Mesh*[meshCount];
			for (int i = 0; i < meshCount; i++)
			{
				int vc, tc, ml;
				char buf[100];
				fread(&vc, 4, 1, f);
				fread(&tc, 4, 1, f);
				fread(&ml, 4, 1, f);
				fread(buf, 1, ml, f);
				meshArray[i] = new Mesh(vc, tc);
				for (int n = 0; n < vc; n++)
				{
					fread(&meshArray[i]->vertexArray[n], 4, 3, f);
					fread(&meshArray[i]->texcoordArray[n], 4, 2, f);
				}
				for (int n = 0; n < tc; n++)
				{
					fread(&meshArray[i]->triangleArray[n], 4, 3, f);
				}
				meshArray[i]->vertexCount = vc;
				meshArray[i]->triangleCount = tc;
				meshArray[i]->calcNormals();
			}
			fclose(f);
		}
	}
	Model::~Model()
	{
		delete [] meshArray;
	}
	void Model::draw()
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		for (int i = 0; i < meshCount; i++)
		{
			glVertexPointer(3,GL_FLOAT,0,meshArray[i]->vertexArray);
			glNormalPointer(GL_FLOAT,0,meshArray[i]->normalArray);
			glTexCoordPointer(2,GL_FLOAT,0,meshArray[i]->texcoordArray);

			glDrawElements(GL_TRIANGLES, meshArray[i]->triangleCount*3, GL_UNSIGNED_INT, meshArray[i]->triangleArray);
		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	void Model::calcNormals()
	{
		for (int i = 0; i < meshCount; i++)
		{
			meshArray[i]->calcNormals();
		}
	}

}