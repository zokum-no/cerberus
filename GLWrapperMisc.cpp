/*

Outracks GLWrapper
Tiny OpenGL wrapping framework for demo use.

You are free to redistribute, change and use this code without charge.
The code is provided "as is", with no warranties or support.

*/

#include "includes/GLWRAPPER.H"

namespace GLWrapper
{
	vertex crossProd(vertex a, vertex b)
	{
		vertex c;
		c.x = (a.y*b.z)-(a.z*b.y);
		c.y = (a.z*b.x)-(a.x*b.z);
		c.z = (a.x*b.y)-(a.y*b.x);
		return c;
	}
	
	void EnableSEM()
	{
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_S, GL_SPHERE_MAP, 0);
		glTexGeni(GL_T, GL_SPHERE_MAP, 0);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}
	
	void DisableSEM()
	{
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}

	vertex rotatex(vertex v, float theta)
	{
		vertex n;
		n.y = cosf(theta)*v.y - sinf(theta)*v.z;
		n.z = sinf(theta)*v.y + cosf(theta)*v.z;
		n.x = v.x;
		return n;
	}
	vertex rotatey(vertex v, float theta)
	{
		vertex n;
		n.x = cosf(theta)*v.x - sinf(theta)*v.z;
		n.z = sinf(theta)*v.x + cosf(theta)*v.z;
		n.y = v.y;
		return n;
	}
	vertex rotatez(vertex v, float theta)
	{
		vertex n;
		n.y = cosf(theta)*v.y - sinf(theta)*v.x;
		n.x = sinf(theta)*v.y + cosf(theta)*v.x;
		n.z = v.z;
		return n;
	}
}