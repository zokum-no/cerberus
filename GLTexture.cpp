/*

Outracks GLWrapper
Tiny OpenGL wrapping framework for demo use.

You are free to redistribute, change and use this code without charge.
The code is provided "as is", with no warranties or support.

*/


#include "includes/GLWRAPPER.H"
#include <atlimage.h>

namespace GLWrapper
{

	Texture::Texture(char* fileName)
	{
		fileTex = false;
		loadFromFile(fileName);
	}
/*
	Texture::Texture(char* fileName, char *alphaName)
	{
		fileTex = false;
		loadFromFile(fileName, alphaName);
	}
*/

	void Texture::loadFromFile(char* fileName)
	{
		fileTex = true;
		handle = 0;

		// finner filetternavn
		int i = 0;
		char period = '.';
		while (1) { if (fileName[i++] == period) break; }
		char* fileExt = &fileName[i];

		ATL::CImage img;
		img.Load(fileName);
		int compoentns = img.GetBPP()/8;
		width = img.GetWidth();
		height = img.GetHeight();
		unsigned char* data = new unsigned char[width*height*compoentns];

		unsigned char* imgdata = (unsigned char*)img.GetBits();
		int pitch = img.GetPitch();
		for (int y = 0; y < img.GetHeight(); y++)
		{
			for (int x = 0; x < img.GetWidth(); x++)
			{
				if (compoentns == 3)
				{
					data[3*(x+y*img.GetWidth())+0] = imgdata[x*3+2];
					data[3*(x+y*img.GetWidth())+1] = imgdata[x*3+1];
					data[3*(x+y*img.GetWidth())+2] = imgdata[x*3+0];
				}
				else
				{
					((unsigned int*)data)[x+y*img.GetWidth()] = *(unsigned int*)&imgdata[x*4];
				}
			}
			imgdata += pitch;
		}

		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		if (compoentns == 3)
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, compoentns, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, compoentns, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

		delete [] data;
		img.Destroy();
	}
/*
	void Texture::loadFromFile(char* fileName, char alphaName)
	{

		fileTex = true;
		handle = 0;

		// finner filetternavn
		int i = 0;
		char period = '.';
		while (1) { if (fileName[i++] == period) break; }
		char* fileExt = &fileName[i];

		ATL::CImage img;
		ATL::CImage alpha;

		img.Load(fileName);
		alpha.Load(alphaName);

		int compoentns = img.GetBPP()/8;	

		if (compontns == 3 ) compoentns ++; // add alpha channel

		width = img.GetWidth();
		height = img.GetHeight();
		unsigned char* data = new unsigned char[width*height*compoentns];

		unsigned char* imgdata = (unsigned char*)img.GetBits();
		int pitch = img.GetPitch();
		for (int y = 0; y < img.GetHeight(); y++)
		{
			for (int x = 0; x < img.GetWidth(); x++)
			{
				if (compoentns == 3)
				{
					data[3*(x+y*img.GetWidth())+0] = imgdata[x*3+2];
					data[3*(x+y*img.GetWidth())+1] = imgdata[x*3+1];
					data[3*(x+y*img.GetWidth())+2] = imgdata[x*3+0];
				}
				else
				{	/*
					((unsigned int*)data)[x+y*img.GetWidth()] = *(unsigned int*)&imgdata[x*4];
					*/				/*
				
					data[3*(x+y*img.GetWidth())+0] = imgdata[x*3+2];
					data[3*(x+y*img.GetWidth())+1] = imgdata[x*3+1];
					data[3*(x+y*img.GetWidth())+2] = imgdata[x*3+0];
					data[3*(x+y*img.GetWidth())+3] = imgdata[x*3+0];

				
				}
			}
			imgdata += pitch;
		}

		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		if (compoentns == 3)
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, compoentns, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, compoentns, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

		delete [] data;
		img.Destroy();
		alpha.Destroy();

	}
*/



	Texture::~Texture()
	{
		// Do nothing
	}
	void Texture::setWrapModeRepeat()
	{
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	void Texture::setWrapModeClamp()
	{
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	void Texture::enable()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, handle);
	}
	void Texture::disable()
	{
		glDisable(GL_TEXTURE_2D);
	}
	Texture::Texture()
	{
		fileTex = false;
	}
	Texture::Texture(int w, int h)
	{
		fileTex = false;
		char* data = new char[w*h*4];
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		width = w;
		height = h;
	}
}